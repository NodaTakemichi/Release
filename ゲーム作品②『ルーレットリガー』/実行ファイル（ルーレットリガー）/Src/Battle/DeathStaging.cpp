#include <DxLib.h>
#include "../Application.h"
#include "../Manager/DataManager/SoundManager.h"
#include "../Manager/SceneManager.h"
#include "../Utility/DrawShader.h"
#include "../Utility/Utility.h"
#include "../Common/Vector2.h"

#include"../_debug/_DebugDispOut.h"
#include "DeathStaging.h"

//�}�X�N�摜�g�呬�x
constexpr float MASK_EXPANSION_SPEED = 2.5f;
//���o�I������
constexpr float STAGING_FINISH_TIME = 1.5f;

//���j�b�g�摜
const Vector2 UNIT_IMG_SHOW_POS = Vector2{ 700,150 };	//�\�����W
const Vector2 UNIT_IMG_SHOW_SIZE = Vector2{ 400,400 };	//�\���T�C�Y

DeathStaging* DeathStaging::instance_ = nullptr;
using PsType = DrawShader::PS_TYPE;

void DeathStaging::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new DeathStaging();
	}
	instance_->Init();
}

DeathStaging& DeathStaging::GetInstance(void)
{
	return *instance_;
}

void DeathStaging::Init(void)
{
	//�摜�̓o�^
	frameImg_ = LoadGraph("./Data/Image/UI/Dead_Frame.png");
	maskImg_ = LoadGraph("./Data/Image/UI/Dead_Mask.png");
	unitImg_ = LoadGraph("./Data/Image/Unit/FairySnake.png");
	backImg_ = LoadGraph("./Data/Image/bg/DeadBack.png");

	//�X�N���[���̍쐬
	maskScreen_ = MakeScreen(
		Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);
	//�X�N���[���̍쐬
	deathStagingScreen_ = MakeScreen(
		Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);

	//������
	totalTime_ = 0.0f;
	mSizeX_ = 0;
	isPlaying_ = false;
	isPlayingBefor_ = false;
	revers_ = 0.0f;

	//SE�o�^
	auto& snd = SoundManager::GetInstance();
	deathSE_ = snd.LoadSound(SoundManager::SOUND_TYPE::DEATH);

}

void DeathStaging::Update(void)
{
	if (isPlaying_)
	{
		isPlayingBefor_ = isPlaying_;

		//�}�X�N�摜�AX�T�C�Y�g��
		int x,y;
		GetGraphSize(maskImg_, &x, &y);
		mSizeX_ = min(MASK_EXPANSION_SPEED * totalTime_ * x, x);

		//���Ԍo��
		bool finish = Utility::OverTime(totalTime_, STAGING_FINISH_TIME);
		if (finish)
		{
			mSizeX_ = 0;
			totalTime_ = 0.0f;
			isPlaying_ = false;
			isPlayingBefor_ = false;
		}
	}
}

void DeathStaging::Release(void)
{
	DeleteGraph(frameImg_);
	DeleteGraph(maskImg_);
	DeleteGraph(backImg_);

}

void DeathStaging::PlayDeathStaging(void)
{
	if (isPlaying_)
	{
		auto& ds = DrawShader::GetInstance();

		//�`���w��
		SetDrawScreen(maskScreen_);
		ClearDrawScreen();

		//�w�i�`��
		ds.DrawGraphToShader({ 0,0 }, backImg_, PsType::REVERSE_TEXTURE, COLOR_F{});
		//���j�b�g�`��
		ds.DrawExtendGraphToShader(
			UNIT_IMG_SHOW_POS, UNIT_IMG_SHOW_SIZE,
			unitImg_, PsType::DEATH_UNIT, COLOR_F{});

		//�`���w��
		SetDrawScreen(deathStagingScreen_);
		ClearDrawScreen();

		//�}�X�N�`��
		ds.DrawGraphAndSubToShader(
			{ 0,0 }, {mSizeX_,Application::SCREEN_SIZE_Y },
			maskScreen_, maskImg_, PsType::MASK, COLOR_F{});
		//�t���[���`��
		ds.DrawExtendGraphToShader(
			{ 0,0 }, { mSizeX_,Application::SCREEN_SIZE_Y },
			frameImg_, PsType::REVERSE_TEXTURE, COLOR_F{});

		//�`���w��
		SetDrawScreen(DX_SCREEN_BACK);


		//���S���o�`��i�G�����ɂ���Ĕ��]�j
		COLOR_F re = COLOR_F{
			revers_
		};
		ds.DrawGraphToShader({ 0,0 }, deathStagingScreen_, PsType::REVERSE_TEXTURE, re);
	}
}

void DeathStaging::SetDeathUnit(const int& img, const float& r)
{
	revers_ = r;
	unitImg_ = img;
	isPlaying_ = true;

	//���SSE�Đ�
	SoundManager::GetInstance().PlaySoundBack(deathSE_);
}

bool DeathStaging::PlayingStaging(void)
{
	return isPlaying_;
}

bool DeathStaging::CheckStartStanging(void)
{
	if (isPlaying_ && !isPlayingBefor_)
	{
		return true;
	}
	return false;
}

DeathStaging::DeathStaging()
{
	isPlaying_=false;
	isPlayingBefor_ = false;
	unitImg_= 0;
	mSizeX_= 0;
	maskImg_= 0;
	frameImg_= 0;
	backImg_= 0;
	deathSE_= 0;
	maskScreen_= 0;
	deathStagingScreen_= 0;
	revers_=0.0f;
	totalTime_=0.0f;

}

DeathStaging::~DeathStaging()
{
}
