#include <DxLib.h>
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/SoundManager.h"
#include "../Manager/InputManager.h"
#include "../Utility/Utility.h"
#include "../Graphics/Drawable/PostEffect/PostEffectMaterial.h"
#include "../Graphics/Drawable/PostEffect/PostEffectRenderer.h"
#include "../Graphics/Drawable/2DImage/PixelRenderer.h"
#include "../Sound/SoundController.h"
#include "../UI/Common/Button.h"
#include "TitleScene.h"
#include "GameOverScene.h"
#include "GameScene.h"

//ノイズSEの音量
constexpr int NOISE_SE_VOLUME = 200;
//ボタンUIの描画座標
constexpr int BUTTN_DRAWING_POS_X = 0;
constexpr int BUTTN_DRAWING_POS_Y = 500;

//シーン状態変更間隔
constexpr float SCENE_STATE_CHANGE_INTERVAL = 5.0f;

GameOverScene::GameOverScene(SceneManager& manager):SceneBase(manager)
{
	updataFunc_ = [&](InputManager& input) {
		BeginUpdate(input);
	};

	drawFunc_ = std::bind(&GameOverScene::BeginDraw, this);

}

GameOverScene::~GameOverScene(void)
{
}

void GameOverScene::LoadData(void)
{
}

void GameOverScene::Init(void)
{
	auto& res = ResourceManager::GetInstance();

	//背景画像
	bgImg_ = res.Load(ResourceManager::SRC::GAMEOVER_IMG).handleId_;

	// ノイズマテリアル
	postEffectMaterial_ = std::make_unique<PostEffectMaterial>();
	postEffectMaterial_->DataLoad(PostEffectMaterial::PS_TYPE::NOISE);
	postEffectMaterial_->Init();
	// ポストエフェクトレンダラー
	postEffectRenderer_ = std::make_unique<PostEffectRenderer>(*postEffectMaterial_);

	//フォントの登録
	res.Load(ResourceManager::SRC::SELECT_CMD_FONT);
	buttnFontHandle_ = CreateFontToHandle(L"EbiharaNoKuseji", FONT_SIZE, 0);


	//ボタン選択肢
	buttons_.push_back(Button{ L"やり直す",std::bind(&GameOverScene::RestartBtnProcess, this) });
	buttons_.push_back(Button{ L"ゲームを終了する",std::bind(&GameOverScene::ExitBtnProcess, this) });
	buttons_.push_back(Button{ L"タイトルへ戻る",std::bind(&GameOverScene::TitleBtnProcess, this) });

	//ノイズSE
	noiseSE_ = std::make_shared<SoundController>();
	noiseSE_->DataLoad(ResourceManager::SRC::NOISE_SND, false);
	noiseSE_->Init(NOISE_SE_VOLUME);
	noiseSE_->PlayLoop();

	
}

void GameOverScene::Update(InputManager& input)
{
	//更新関数の呼び出し
	updataFunc_(input);
	return;
}

void GameOverScene::Draw(void)
{
	//描画関数の呼び出し
	drawFunc_();
	return;
}

void GameOverScene::BeginUpdate(InputManager& input)
{
	bool isTimeOver = Utility::TimeOver(frameTime_, SCENE_STATE_CHANGE_INTERVAL);
	float progress = frameTime_ / SCENE_STATE_CHANGE_INTERVAL;
	//マテリアル定数更新                                 
	postEffectMaterial_->SetConstBuf(0, FLOAT4{ frameTime_,progress,0.0f,0.0f });

	//ノイズSEの音量変更
	noiseSE_->SetVolume(NOISE_SE_VOLUME * static_cast<int>(1.0f - progress));

	if (isTimeOver)
	{
		noiseSE_->Stop();

		updataFunc_ = [&](InputManager& input) {NormalUpdate(input); };
		drawFunc_ = std::bind(&GameOverScene::NormalDraw, this);
	}
}

void GameOverScene::NormalUpdate(InputManager& input)
{
	BtnSelectProcess(static_cast<int>(buttons_.size()));

	// シーン遷移
	if (input.IsTrgDown(KEY_INPUT_SPACE))
	{
		updataFunc_ = [&](InputManager& input) {
			EndUpdate(input);
		};

		drawFunc_ = std::bind(&GameOverScene::EndDraw, this);
		frameTime_ = 0;
	}
}

void GameOverScene::EndUpdate(InputManager& input)
{
	//ボタン処理
	buttons_[cursolNum_].func_();
}

void GameOverScene::BeginDraw(void)
{
	//背景画像
	SetDrawMode(DX_DRAWMODE_BILINEAR);
	DrawExtendGraph(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, bgImg_, true);
	SetDrawMode(DX_DRAWMODE_NEAREST);

	//ポストエフェクト
	postEffectRenderer_->Draw();
}

void GameOverScene::NormalDraw(void)
{
	//背景画像
	SetDrawMode(DX_DRAWMODE_BILINEAR);
	DrawExtendGraph(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, bgImg_, true);
	SetDrawMode(DX_DRAWMODE_NEAREST);

	//ボタンテキストの描画
	int idx = 0;
	for (const auto& btn : buttons_)
	{	
		//X座標
		auto halfLen = Utility::GetStringLength(btn.text_, buttnFontHandle_) / 2;
		int posX = Application::SCREEN_SIZE_X / 2 - halfLen;

		//フォント色
		int color = 0xeeeeee;
		if (idx == cursolNum_)color = 0xff0f0f;
		DrawStringFToHandle(
			static_cast<float>(BUTTN_DRAWING_POS_X + posX),
			static_cast<float>(BUTTN_DRAWING_POS_Y + idx * LINE_SPACING),
			btn.text_.c_str(),
			color, buttnFontHandle_);
		idx++;
	}

}

void GameOverScene::EndDraw(void)
{
	NormalDraw();
}

void GameOverScene::RestartBtnProcess(void)
{
	sceneManager_.ChangeScene(SceneManager::SCENE_ID::GAME);
}

void GameOverScene::ExitBtnProcess(void)
{
	//ゲームの終了
	Application::GetInstance().CloseWindows();
}

void GameOverScene::TitleBtnProcess(void)
{
	sceneManager_.ChangeScene(SceneManager::SCENE_ID::TITLE);
}
