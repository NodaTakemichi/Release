#include <chrono>
#include <DxLib.h>
#include "../Application.h"
#include "../Common/Fader.h"
#include "../Manager//SoundManager.h"
#include "../Manager/Camera.h"
#include "../Scene/TitleScene.h"
#include "../Scene/GameScene.h"
#include "../Scene/ResultScene.h"
#include "SceneManager.h"

//�f���^�^�C��
constexpr float DELTA_TIME = 1.0f / 60.0f;
//�t�F�[�h�J���[�F��
constexpr int FADE_COLOR_BLACK = 0x0a0a0a;	//��
constexpr int FADE_COLOR_WHITE = 0xf0f0f0;	//��

void SceneManager::Init(void)
{

	sceneId_ = SCENE_ID::TITLE;
	waitSceneId_ = SCENE_ID::NONE;

	fader_ = std::make_unique<Fader>();
	fader_->Init();

	camera_ = std::make_unique<Camera>();
	camera_->Init();

	isSceneChanging_ = false;

	// �f���^�^�C��
	preTime_ = std::chrono::system_clock::now();

	// �����̃V�[�h�̏�����
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	//�}�E�X���\���ɂ���
	SetMouseDispFlag(false);

	//�E�B���h�E���A�N�e�B�u��ԂłȂ��Ƃ��������s��
	SetAlwaysRunFlag(true);

	//���C���X�N���[���̍쐬
	mainScreen_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);
	//�X�N���[�����W
	screenPos_ = { 0.0f,0.0f };

	//3D��񏉊���
	Init3D();
}

void SceneManager::Init3D(void)
{
	//�w�i�F
	SetBackgroundColor(0, 0, 0); 

	//Z�o�b�t�@��L���ɂ���
	SetUseZBuffer3D(true);

	//Z�o�b�t�@�ւ̏������݂�L���ɂ���
	SetWriteZBuffer3D(true);

	//�o�b�N�J�����O��L���ɂ���i�J�����̉f��Ȃ�������\�������Ȃ��j
	SetUseBackCulling(true);

	//���C�e�B���O�v�Z���s��
	SetUseLighting(true);

	//�W�����C�g�̌v�Z���s��Ȃ�
	SetLightEnable(false);

	//�t�H�O�̐ݒ�
	SetFogEnable(true);
	SetFogColor(0,0, 0);
	SetFogStartEnd(FOG_START_DISTANCE, FOG_END_DISTANCE);

}

void SceneManager::CreateScene(std::shared_ptr<SceneBase> scene)
{
	if (scenes_.empty())
	{
		scenes_.push_back(scene);
	}
	else
	{
		scenes_.front() = scene;
	}

	//�f�[�^�̃��[�h
	scenes_.front()->LoadData();
}

void SceneManager::ChangeAllScene(std::shared_ptr<SceneBase> scene)
{
	//�t�F�[�h�J�n
	StartFadeIn();

	scenes_.clear();
	scenes_.push_back(scene);
	//�f�[�^�̃��[�h
	scenes_.front()->LoadData();
}

void SceneManager::PushScene(std::shared_ptr<SceneBase> scene)
{
	scene->Init();
	scenes_.push_front(scene);
}

void SceneManager::PopScene()
{
	if (scenes_.size() >= 1)
	{
		scenes_.pop_front();
	}
}

void SceneManager::Updata(InputManager& input)
{
	if (scenes_.empty())return;

	// �f���^�^�C��
	auto nowTime = std::chrono::system_clock::now();
	deltaTime_ = static_cast<float>(
		std::chrono::duration_cast<std::chrono::nanoseconds>(nowTime - preTime_).count() / 1000000000.0);
	preTime_ = nowTime;
	totalTime_ += GetDeltaTime();

	//�X�V
	camera_->Update();

	//��ʗh��v�Z
	CalcScreenShake();

	//�t�F�[�h�X�V
	Fade();

	//�V�[���X�V
	scenes_.front()->Update(input);

}

void SceneManager::Draw()
{
	// �`���O���t�B�b�N�̈�̎w��
	SetDrawingScreen(mainScreen_);

	//�V�[���`��(�Ō������`��)
	auto rit = scenes_.rbegin();
	for (; rit != scenes_.rend(); rit++)
	{
		(*rit)->Draw();
	}

	// �Ó]�E���]
	fader_->Draw();

	// �`���O���t�B�b�N�̈�̎w��
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();
	DrawGraph(static_cast<int>(screenPos_.x), static_cast<int>(screenPos_.y), mainScreen_, true);

	// �f�o�b�N�p�`��
#ifdef _DEBUG
	//camera_->Draw();
	DrawFormatString(0, 0, 0xffffff, L"FPS:%.2f", GetFPS());
#endif // _DEBUG

}

void SceneManager::ChangeScene(SCENE_ID nextId)
{
	//�t�F�[�h���͎��s���Ȃ�
	if (!fader_->IsEnd())return;

	DeleteLightHandleAll();


	// �t�F�[�h�������I����Ă���V�[����ς���ꍇ�����邽�߁A
	// �J�ڐ�V�[���������o�ϐ��ɕێ�
	waitSceneId_ = nextId;

	// �t�F�[�h�A�E�g(�Ó])���J�n����
	fader_->SetFade(Fader::STATE::FADE_OUT);
	isSceneChanging_ = true;

	//�t�F�[�h�J���[�̎w��
	switch (waitSceneId_)
	{
	case SCENE_ID::TITLE:
		SetFadeColor(FADE_COLOR_BLACK);
		break;
	case SCENE_ID::GAME:
		SetFadeColor(FADE_COLOR_BLACK);
		break;
	case SCENE_ID::RESULT:
		SetFadeColor(FADE_COLOR_WHITE);
		break;
	}

}

SceneManager::SCENE_ID SceneManager::GetSceneID(void)
{
	return sceneId_;
}

Camera& SceneManager::GetCamera(void)
{
	return *camera_;
}

const float& SceneManager::GetDeltaTime(void) const
{
	return DELTA_TIME;
}

const float& SceneManager::GetTotalTime(void) const
{
	return totalTime_;
}

void SceneManager::StartFadeIn(void)
{
	// �t�F�[�h�C�����J�n����
	fader_->SetFade(Fader::STATE::FADE_IN);
}

void SceneManager::SetFadeColor(unsigned int color)
{
	fader_->SetFadeColor(color);
}

SceneManager::SceneManager(void)
{
	sceneId_ = SCENE_ID::NONE;
	waitSceneId_ = SCENE_ID::NONE;

	scenes_.clear();
	fader_ = nullptr;

	isSceneChanging_ = false;

	// �f���^�^�C��
	deltaTime_ = DELTA_TIME;

	//������
	Init();
}

SceneManager::~SceneManager(void)
{
}

void SceneManager::ResetDeltaTime(void)
{
	deltaTime_ = DELTA_TIME;
	preTime_ = std::chrono::system_clock::now();
}

void SceneManager::DoChangeScene(SCENE_ID sceneId)
{
	// �V�[����ύX����
	sceneId_ = sceneId;

	//�����̑S��~
	SoundManager::GetInstance().StopAllSound();

	switch (sceneId_)
	{
	case SCENE_ID::TITLE:
		ChangeAllScene(std::make_shared<TitleScene>(*this));
		break;
	case SCENE_ID::GAME:
		ChangeAllScene(std::make_shared<GameScene>(*this));
		break;
	case SCENE_ID::RESULT:
		ChangeAllScene(std::make_shared<ResultScene>(*this));
		break;
	}

	ResetDeltaTime();

	waitSceneId_ = SCENE_ID::NONE;

}

void SceneManager::Fade(void)
{
	fader_->Update();

	//�t�F�[�h�������f
	Fader::STATE fState = fader_->GetState();
	switch (fState)
	{
	case Fader::STATE::FADE_IN:
		// ���]��
		if (fader_->IsEnd())
		{
			// ���]���I��������A�t�F�[�h�����I��
			fader_->SetFade(Fader::STATE::NONE);
		}
		break;
	case Fader::STATE::FADE_OUT:
		// �Ó]��
		if (fader_->IsEnd())
		{
			//�J�������[�h�̃��Z�b�g
			camera_->ChangeMode(Camera::MODE::FIXED_POINT);

			// ���]���I��������A�t�F�[�h�����I��
			fader_->SetFade(Fader::STATE::NONE);

			// ���S�ɈÓ]���Ă���V�[���J��
			DoChangeScene(waitSceneId_);
		}
		break;
	}

}


void SceneManager::CalcScreenShake(void)
{
	//�X�N���[���̍��W
	if (amplitude_ > 0.0f) {
		//�X�N���[���h�炵
		screenPos_.x = ((frame_ % 3) - 1) * amplitude_;
		screenPos_.y = (((frame_ + rand()) % 3) - 1) * amplitude_;
	}

	//�h������������Ă���
	amplitude_ *= 0.9f;
	if (amplitude_ < 1.0f) {
		amplitude_ = 0.0f;
	}
	++frame_;

}

void SceneManager::SetDrawingScreen(const int& screenID)
{
	SetDrawScreen(screenID);
	ClearDrawScreen();

	//�J�����ݒ�
	camera_->SetBeforeDraw();
}


