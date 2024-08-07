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
//フォントサイズ
constexpr int FONT_SIZE = 12;
//フォントの太さ
constexpr int FONT_THICK = 20;
//フォントカラー
constexpr int FONT_COLOR = 0xdddddd;

//時間に対して遅らせる値
constexpr float SLOW_TIME = 1.5f;
// 画像登場時間間隔
constexpr float APPEARANCE_TIME_INTERVAL = 1.0f;
//背景画像座標Y
constexpr int BACKGRAOUND_POS_Y = 60;
//指示コメント座標Y
constexpr int OPERATION_COMENT_POS_Y = 600;
//指示コメント表示間隔
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
	//画像の登録
	titleImg_ = LoadGraph("Data/Image/UI/タイトル.png");
	titleImgNor_ = LoadGraph("Data/Image/UI/タイトル_n.png");
	titleBackImg_ = LoadGraph("Data/Image/UI/タイトルバック.png");
	bgImg_ = LoadGraph("Data/Image/bg/title.jpg");

	//ユニット画像
	unitsImg_.push_back(LoadGraph("Data/Image/UI/Units_3.png"));
	unitsImg_.push_back(LoadGraph("Data/Image/UI/Units_2.png"));
	unitsImg_.push_back(LoadGraph("Data/Image/UI/Units_1.png"));

	//フォントハンドル
	fontHandle_ = CreateFontToHandle("HGS創英角ｺﾞｼｯｸUB", 40, -1,
		DX_FONTTYPE_ANTIALIASING_EDGE, -1, 1);
	fontColor_ = GetColor(230, 230, 250);

	//スクリーン作成
	auto& cx = Application::SCREEN_SIZE_X;
	auto& cy = Application::SCREEN_SIZE_Y;
	uiScreen_ = MakeScreen(cx, cy, true);

	//音声関連
	auto& snd = SoundManager::GetInstance();

	//BGM再生
	bgmHandle_= snd.LoadSound(SoundManager::SOUND_TYPE::TITLE_BGM);
	snd.PlaySoundBGM(bgmHandle_);
}

void TitleScene::Update(void)
{
	//シーン遷移
	auto& ins = InputManager::GetInstance();
	if (ins.IsClickMouseLeft())
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::SELECT, true);

		//SE再生
		auto& snd = SoundManager::GetInstance();

		return;
	}

	//デルタタイム
	auto delta = SceneManager::GetInstance().GetDeltaTime();
	totalTime_ += delta;
}

void TitleScene::Draw(void)
{
	auto& ds = DrawShader::GetInstance();
	auto& cx = Application::SCREEN_SIZE_X;
	auto& cy = Application::SCREEN_SIZE_Y;

	//背景描画
	DrawGraph(0, 0, bgImg_, true);

	//背景モンスター描画
	int appearanceTime = 0;		//登場時間
	for (auto& unitImg : unitsImg_)
	{
		COLOR_F mBuf = COLOR_F{
			totalTime_ - appearanceTime - APPEARANCE_TIME_INTERVAL,
			SLOW_TIME
		};
		ds.DrawGraphToShader({ 0,0 }, unitImg, PsType::APPEARANCE, mBuf);
		appearanceTime+= APPEARANCE_TIME_INTERVAL;
	}


	//UI関連（一定時間後）
	appearanceTime += APPEARANCE_TIME_INTERVAL;
	if (totalTime_ <= appearanceTime)return;
	//UI描画
	DrawUI();
	//タイトルロゴ描画
	COLOR_F buf = COLOR_F{
		totalTime_ - appearanceTime,
		SLOW_TIME
	};
	ds.DrawGraphToShader({ 0,0 }, uiScreen_, PsType::APPEARANCE, buf);

}

void TitleScene::Release(void)
{
	//サウンドの終了
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


	//タイトル背景描画
	int posY = BACKGRAOUND_POS_Y;
	DrawGraph(0, posY, titleBackImg_, true);
	//タイトルロゴ描画（ノーマルマップ）
	COLOR_F mBuf = COLOR_F{
		totalTime_,
		SLOW_TIME
	};
	ds.DrawGraphAndSubToShader(
		{ 0,posY }, titleImg_, titleImgNor_, PsType::NORMAL_MAP, mBuf);


	//指示描画
	if (fmodf(totalTime_, OPERATION_COMENT_SHOE_INTERVAL) >= 1.0f)
	{
		std::string s = "クリックでスタート！";
		auto len = Utility::CalcStringLength(s, fontHandle_) / 2;
		DrawStringToHandle(cx / 2 - len, OPERATION_COMENT_POS_Y, s.c_str(), fontColor_, fontHandle_);
	}

	SetDrawScreen(DX_SCREEN_BACK);
}
