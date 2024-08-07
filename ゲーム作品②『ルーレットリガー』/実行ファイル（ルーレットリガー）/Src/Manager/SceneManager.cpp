#include <chrono>
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../Application.h"
#include "../Common/Fader.h"
#include "../Scene/TitleScene.h"
#include "../Scene/SelectScene.h"
#include "../Scene/EditScene.h"
#include "../Scene/GameScene.h"
#include "../Scene/ResultScene.h"
#include "../Utility/Measure.h"
#include "../Utility/DrawShader.h"
#include"../Battle/DeathStaging.h"
#include "InputManager.h"
#include "DataManager/CmdManager.h"
#include "DataManager/SoundManager.h"
#include "DataManager/UnitDataManager.h"
#include "DataManager/EffectManager.h"
#include "DataManager/BattleDataManager.h"
#include "DeckManager.h"
#include "SceneManager.h"

//�f���^�^�C��
constexpr float DELTA_TIME = 1.0f / 60.0f;
//�N���b�N�G�t�F�N�g�T�C�Y
constexpr int CLICK_EFFECT_SIZE = 15;


SceneManager* SceneManager::instance_ = nullptr;

void SceneManager::CreateInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new SceneManager();
	}
	instance_->Init();
}

SceneManager& SceneManager::GetInstance(void)
{
	return *instance_;
}

void SceneManager::Init(void)
{
	//�E�C���h���A�N�e�B�u��Ԃ���Ȃ��Ă��A�����𑱍s������
	//SetAlwaysRunFlag(false);

	//�}�E�X�̔�\��
	SetMouseDispFlag(false);

	// �V�F�[�_�\�`��N���X������
	DrawShader::CreateInstance();

	// �����Ǘ�������
	SoundManager::CreateInstance();

	// ���j�b�g�f�[�^�Ǘ�������
	UnitDataManager::CreateInstance();

	// �G�t�F�N�g�f�[�^�Ǘ�������
	EffectManager::CreateInstance();

	// �R�}���h�f�[�^�Ǘ�������
	CmdManager::CreateInstance();

	// �o�g���f�[�^�Ǘ�������
	BattleDataManager::CreateInstance();

	// �f�b�L�Ǘ�������
	DeckManager::CreateInstance();


	fader_ = new Fader();
	fader_->Init();

	scene_ = new TitleScene();
	scene_->Init();

	sceneID_ = SCENE_ID::TITLE;
	waitSceneID_ = SCENE_ID::NONE;

	isSceneChanging_ = false;

	// �f���^�^�C��
	preTime_ = std::chrono::system_clock::now();
	totalTime_ = 0.0f;

	//����������̏�����
	gen_ = std::mt19937(rd_());

	//�}�E�X�摜
	mouseImg_ = LoadGraph("./Data/Image/UI/mouse.png");
	//�N���b�N�G�t�F�N�g�n���h��
	clickEfHandle_ = LoadEffekseerEffect(
		"Data/Effect/Click/ClickEf.efkefc");

	//�V�[���J��SE
	sceneMoveSE_ = SoundManager::GetInstance().LoadSound(
		SoundManager::SOUND_TYPE::SCENE_MOVE);

	//�X�N���[���쐬
	int sx = Application::SCREEN_SIZE_X;
	int sy = Application::SCREEN_SIZE_Y;
	screen_ = MakeScreen(sx, sy, true);
	//�U��
	amplitude_ = 0.0f;
	//�t���[��
	frame_ = 0;

}


void SceneManager::Update(void)
{

	if (scene_ == nullptr)
	{
		return;
	}

	// �f���^�^�C��
	auto nowTime = std::chrono::system_clock::now();
	deltaTime_ = static_cast<float>(
		std::chrono::duration_cast<std::chrono::nanoseconds>(nowTime - preTime_).count() / 1000000000.0);
	preTime_ = nowTime;
	totalTime_ += deltaTime_;

	fader_->Update();
	if (isSceneChanging_)
	{
		Fade();
	}
	else
	{
		scene_->Update();
	}

	//�N���b�N�G�t�F�N�g
	DrawClickEf();

	// Effekseer�ɂ��Đ����̃G�t�F�N�g���X�V����B
	UpdateEffekseer2D();

}

void SceneManager::Draw(void)
{
	// �`���O���t�B�b�N�̈�̎w��
	SetDrawScreen(DX_SCREEN_BACK);
	// ��ʂ�������
	ClearDrawScreen();

	// �`��
	scene_->Draw();

	// Effekseer�ɂ��Đ����̃G�t�F�N�g��`�悷��B
	DrawEffekseer2D();
	
	fader_->Draw();

	//�}�E�X�`��
	auto& ins = InputManager::GetInstance();
	Vector2 m = ins.GetMousePos();
	DrawGraph(m.x, m.y, mouseImg_, true);

}

void SceneManager::Release(void)
{
	scene_->Release();
	delete scene_;

	delete fader_;

	DeleteGraph(mouseImg_);
	DeleteEffekseerEffect(clickEfHandle_);

	//�����J��
	InitSoundMem();

	// �V�F�[�_�\�`��̉��
	DrawShader::GetInstance().Release();

	//�����Ǘ��N���X�̉��
	SoundManager::GetInstance().Release();

	//���j�b�g�f�[�^�Ǘ��N���X�̊J��
	UnitDataManager::GetInstance().Release();

	//�R�}���h�f�[�^�Ǘ��N���X�̊J��
	CmdManager::GetInstance().Release();

	//�G�t�F�N�g�f�[�^�Ǘ��N���X�̊J��
	EffectManager::GetInstance().Release();

	//�o�g���f�[�^�Ǘ��N���X�̊J��
	BattleDataManager::GetInstance().Release();

	//�f�b�L�Ǘ��N���X�̊J��
	DeckManager::GetInstance().Release();

}

void SceneManager::ChangeScene(SCENE_ID nextId, bool isFading)
{

	//�V�[���J��SE
	SoundManager::GetInstance().PlaySoundBack(sceneMoveSE_);

	waitSceneID_ = nextId;

	if (isFading)
	{
		fader_->SetFade(Fader::STATE::FADE_OUT);
		isSceneChanging_ = true;
	}
	else
	{
		DoChangeScene();
	}
}

SceneManager::SCENE_ID SceneManager::GetmSceneID(void)
{
	return sceneID_;
}

float SceneManager::GetDeltaTime(void) const
{
	return deltaTime_;
}

float SceneManager::GetTotalTime(void) const
{
	return totalTime_;
}

int SceneManager::GetRand(const int& min, const int& max)
{
	//��l���z
	std::uniform_int_distribution<> dist(min, max);
	return	dist(gen_);
}

SceneManager::SceneManager(void)
{

	sceneID_ = SCENE_ID::NONE;
	waitSceneID_ = SCENE_ID::NONE;

	scene_ = nullptr;
	fader_ = nullptr;

	isSceneChanging_ = false;

	// �f���^�^�C��
	deltaTime_ = DELTA_TIME;
	totalTime_ = 0.0f;

}

SceneManager::~SceneManager(void)
{
	delete instance_;
}

void SceneManager::ResetDeltaTime(void)
{
	deltaTime_ = DELTA_TIME;
	preTime_ = std::chrono::system_clock::now();
}

void SceneManager::DoChangeScene(void)
{
	//�t�H���g�̑��폜
	InitFontToHandle();


	scene_->Release();
	delete scene_;

	sceneID_ = waitSceneID_;

	switch (sceneID_)
	{
	case SCENE_ID::TITLE:
		scene_ = new TitleScene();
		break;
	case SCENE_ID::SELECT:
		scene_ = new SelectScene();
		break;
	case SCENE_ID::DECK_EDIT:
		scene_ = new EditScene();
		break;
	case SCENE_ID::GAME:
		scene_ = new GameScene();
		break;
	case SCENE_ID::RESULT:
		scene_ = new ResultScene();
		//���[�h�����G�t�F�N�g�̉��
		EffectManager::GetInstance().LoadedEffectRelease();
		break;
	default:
		break;
	}

	scene_->Init();

	// �v�����Z�b�g
	Measure::GetInstance().Reset();

	ResetDeltaTime();

	waitSceneID_ = SCENE_ID::NONE;

}

void SceneManager::Fade(void)
{

	Fader::STATE fState = fader_->GetState();
	switch (fState)
	{
	case Fader::STATE::FADE_IN:
		if (fader_->IsEnd())
		{
			fader_->SetFade(Fader::STATE::NONE);
			isSceneChanging_ = false;
		}
		break;
	case Fader::STATE::FADE_OUT:
		if (fader_->IsEnd())
		{
			DoChangeScene();
			fader_->SetFade(Fader::STATE::FADE_IN);
		}
		break;
	}

}

void SceneManager::DrawClickEf(void)
{
	auto& ins = InputManager::GetInstance();
	if (ins.IsClickMouseLeft())
	{
		//�G�t�F�N�g�Đ�
		efPlayHandle_ = PlayEffekseer2DEffect(clickEfHandle_);

		//�G�t�F�N�g�̑傫��
		SetScalePlayingEffekseer2DEffect(
			efPlayHandle_, CLICK_EFFECT_SIZE, CLICK_EFFECT_SIZE, CLICK_EFFECT_SIZE);
		//�G�t�F�N�g�̈ʒu
		Vector2 mousePos = ins.GetMousePos();
		SetPosPlayingEffekseer2DEffect(
			efPlayHandle_, mousePos.x, mousePos.y, 0);

	}
}

