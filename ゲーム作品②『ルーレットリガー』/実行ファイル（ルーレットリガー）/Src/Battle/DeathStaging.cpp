#include <DxLib.h>
#include "../Application.h"
#include "../Manager/DataManager/SoundManager.h"
#include "../Manager/SceneManager.h"
#include "../Utility/DrawShader.h"
#include "../Utility/Utility.h"
#include "../Common/Vector2.h"

#include"../_debug/_DebugDispOut.h"
#include "DeathStaging.h"

//マスク画像拡大速度
constexpr float MASK_EXPANSION_SPEED = 2.5f;
//演出終了時間
constexpr float STAGING_FINISH_TIME = 1.5f;

//ユニット画像
const Vector2 UNIT_IMG_SHOW_POS = Vector2{ 700,150 };	//表示座標
const Vector2 UNIT_IMG_SHOW_SIZE = Vector2{ 400,400 };	//表示サイズ

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
	//画像の登録
	frameImg_ = LoadGraph("./Data/Image/UI/Dead_Frame.png");
	maskImg_ = LoadGraph("./Data/Image/UI/Dead_Mask.png");
	unitImg_ = LoadGraph("./Data/Image/Unit/FairySnake.png");
	backImg_ = LoadGraph("./Data/Image/bg/DeadBack.png");

	//スクリーンの作成
	maskScreen_ = MakeScreen(
		Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);
	//スクリーンの作成
	deathStagingScreen_ = MakeScreen(
		Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);

	//初期化
	totalTime_ = 0.0f;
	mSizeX_ = 0;
	isPlaying_ = false;
	isPlayingBefor_ = false;
	revers_ = 0.0f;

	//SE登録
	auto& snd = SoundManager::GetInstance();
	deathSE_ = snd.LoadSound(SoundManager::SOUND_TYPE::DEATH);

}

void DeathStaging::Update(void)
{
	if (isPlaying_)
	{
		isPlayingBefor_ = isPlaying_;

		//マスク画像、Xサイズ拡大
		int x,y;
		GetGraphSize(maskImg_, &x, &y);
		mSizeX_ = min(MASK_EXPANSION_SPEED * totalTime_ * x, x);

		//時間経過
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

		//描画先指定
		SetDrawScreen(maskScreen_);
		ClearDrawScreen();

		//背景描画
		ds.DrawGraphToShader({ 0,0 }, backImg_, PsType::REVERSE_TEXTURE, COLOR_F{});
		//ユニット描画
		ds.DrawExtendGraphToShader(
			UNIT_IMG_SHOW_POS, UNIT_IMG_SHOW_SIZE,
			unitImg_, PsType::DEATH_UNIT, COLOR_F{});

		//描画先指定
		SetDrawScreen(deathStagingScreen_);
		ClearDrawScreen();

		//マスク描画
		ds.DrawGraphAndSubToShader(
			{ 0,0 }, {mSizeX_,Application::SCREEN_SIZE_Y },
			maskScreen_, maskImg_, PsType::MASK, COLOR_F{});
		//フレーム描画
		ds.DrawExtendGraphToShader(
			{ 0,0 }, { mSizeX_,Application::SCREEN_SIZE_Y },
			frameImg_, PsType::REVERSE_TEXTURE, COLOR_F{});

		//描画先指定
		SetDrawScreen(DX_SCREEN_BACK);


		//死亡演出描画（敵味方によって反転）
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

	//死亡SE再生
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
