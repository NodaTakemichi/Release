#include <DxLib.h>
#include "../Application.h"
#include "../Manager/DataManager/SoundManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Utility/DrawShader.h"
#include "../Utility/Utility.h"
#include "../Common/Vector2.h"

#include "TitleScene.h"

using PsType = DrawShader::PS_TYPE;
//�t�H���g�T�C�Y
constexpr int FONT_SIZE = 12;
//�t�H���g�̑���
constexpr int FONT_THICK = 20;
//�t�H���g�J���[
constexpr int FONT_COLOR = 0xdddddd;

//���Ԃɑ΂��Ēx�点��l
constexpr float SLOW_TIME = 1.5f;
// �摜�o�ꎞ�ԊԊu
constexpr float APPEARANCE_TIME_INTERVAL = 1.0f;
//�w�i�摜���WY
constexpr int BACKGRAOUND_POS_Y = 60;
//�w���R�����g���WY
constexpr int OPERATION_COMENT_POS_Y = 600;
//�w���R�����g�\���Ԋu
constexpr float OPERATION_COMENT_SHOE_INTERVAL = 3.0f;

TitleScene::TitleScene(void)
{
	titleImg_=0;
	titleImgNor_ = 0;
	titleBackImg_ = 0;
	fontHandle_ = 0;
	fontColor_ = 0;
	uiScreen_ = 0;

}

TitleScene::~TitleScene(void)
{
}

void TitleScene::Init(void)
{
	//�摜�̓o�^
	titleImg_ = LoadGraph("Data/Image/UI/�^�C�g��.png");
	titleImgNor_ = LoadGraph("Data/Image/UI/�^�C�g��_n.png");
	titleBackImg_ = LoadGraph("Data/Image/UI/�^�C�g���o�b�N.png");
	bgImg_ = LoadGraph("Data/Image/bg/title.jpg");

	//���j�b�g�摜
	unitsImg_.push_back(LoadGraph("Data/Image/UI/Units_3.png"));
	unitsImg_.push_back(LoadGraph("Data/Image/UI/Units_2.png"));
	unitsImg_.push_back(LoadGraph("Data/Image/UI/Units_1.png"));

	//�t�H���g�n���h��
	fontHandle_ = CreateFontToHandle("HGS�n�p�p�޼��UB", 40, -1,
		DX_FONTTYPE_ANTIALIASING_EDGE, -1, 1);
	fontColor_ = GetColor(230, 230, 250);

	//�X�N���[���쐬
	auto& cx = Application::SCREEN_SIZE_X;
	auto& cy = Application::SCREEN_SIZE_Y;
	uiScreen_ = MakeScreen(cx, cy, true);

	//�����֘A
	auto& snd = SoundManager::GetInstance();

	//BGM�Đ�
	bgmHandle_= snd.LoadSound(SoundManager::SOUND_TYPE::TITLE_BGM);
	snd.PlaySoundBGM(bgmHandle_);
}

void TitleScene::Update(void)
{
	//�V�[���J��
	auto& ins = InputManager::GetInstance();
	if (ins.IsClickMouseLeft())
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::SELECT, true);

		//SE�Đ�
		auto& snd = SoundManager::GetInstance();

		return;
	}

	//�f���^�^�C��
	auto delta = SceneManager::GetInstance().GetDeltaTime();
	totalTime_ += delta;
}

void TitleScene::Draw(void)
{
	auto& ds = DrawShader::GetInstance();
	auto& cx = Application::SCREEN_SIZE_X;
	auto& cy = Application::SCREEN_SIZE_Y;

	//�w�i�`��
	DrawGraph(0, 0, bgImg_, true);

	//�w�i�����X�^�[�`��
	int appearanceTime = 0;		//�o�ꎞ��
	for (auto& unitImg : unitsImg_)
	{
		COLOR_F mBuf = COLOR_F{
			totalTime_ - appearanceTime - APPEARANCE_TIME_INTERVAL,
			SLOW_TIME
		};
		ds.DrawGraphToShader({ 0,0 }, unitImg, PsType::APPEARANCE, mBuf);
		appearanceTime+= APPEARANCE_TIME_INTERVAL;
	}


	//UI�֘A�i��莞�Ԍ�j
	appearanceTime += APPEARANCE_TIME_INTERVAL;
	if (totalTime_ <= appearanceTime)return;
	//UI�`��
	DrawUI();
	//�^�C�g�����S�`��
	COLOR_F buf = COLOR_F{
		totalTime_ - appearanceTime,
		SLOW_TIME
	};
	ds.DrawGraphToShader({ 0,0 }, uiScreen_, PsType::APPEARANCE, buf);

}

void TitleScene::Release(void)
{
	//�T�E���h�̏I��
	StopSoundMem(bgmHandle_);

	DeleteGraph(titleImg_);
	DeleteGraph(titleImgNor_);
	DeleteGraph(titleBackImg_);
	DeleteGraph(bgImg_);
	for (auto& img : unitsImg_)
	{
		DeleteGraph(img);
	}
	unitsImg_.clear();
}

void TitleScene::DrawUI(void)
{
	auto& ds = DrawShader::GetInstance();
	auto& cx = Application::SCREEN_SIZE_X;
	auto& cy = Application::SCREEN_SIZE_Y;

	SetDrawScreen(uiScreen_);
	ClearDrawScreen();


	//�^�C�g���w�i�`��
	int posY = BACKGRAOUND_POS_Y;
	DrawGraph(0, posY, titleBackImg_, true);
	//�^�C�g�����S�`��i�m�[�}���}�b�v�j
	COLOR_F mBuf = COLOR_F{
		totalTime_,
		SLOW_TIME
	};
	ds.DrawGraphAndSubToShader(
		{ 0,posY }, titleImg_, titleImgNor_, PsType::NORMAL_MAP, mBuf);


	//�w���`��
	if (fmodf(totalTime_, OPERATION_COMENT_SHOE_INTERVAL) >= 1.0f)
	{
		std::string s = "�N���b�N�ŃX�^�[�g�I";
		auto len = Utility::CalcStringLength(s, fontHandle_) / 2;
		DrawStringToHandle(cx / 2 - len, OPERATION_COMENT_POS_Y, s.c_str(), fontColor_, fontHandle_);
	}

	SetDrawScreen(DX_SCREEN_BACK);
}
