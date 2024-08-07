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

//デルタタイム
constexpr float DELTA_TIME = 1.0f / 60.0f;
//フェードカラー：黒
constexpr int FADE_COLOR_BLACK = 0x0a0a0a;	//黒
constexpr int FADE_COLOR_WHITE = 0xf0f0f0;	//白

void SceneManager::Init(void)
{

	sceneId_ = SCENE_ID::TITLE;
	waitSceneId_ = SCENE_ID::NONE;

	fader_ = std::make_unique<Fader>();
	fader_->Init();

	camera_ = std::make_unique<Camera>();
	camera_->Init();

	isSceneChanging_ = false;

	// デルタタイム
	preTime_ = std::chrono::system_clock::now();

	// 乱数のシードの初期化
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	//マウスを非表示にする
	SetMouseDispFlag(false);

	//ウィンドウがアクティブ状態でなくとも処理を行う
	SetAlwaysRunFlag(true);

	//メインスクリーンの作成
	mainScreen_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);
	//スクリーン座標
	screenPos_ = { 0.0f,0.0f };

	//3D情報初期化
	Init3D();
}

void SceneManager::Init3D(void)
{
	//背景色
	SetBackgroundColor(0, 0, 0); 

	//Zバッファを有効にする
	SetUseZBuffer3D(true);

	//Zバッファへの書き込みを有効にする
	SetWriteZBuffer3D(true);

	//バックカリングを有効にする（カメラの映らない裏側を表示させない）
	SetUseBackCulling(true);

	//ライティング計算を行う
	SetUseLighting(true);

	//標準ライトの計算を行わない
	SetLightEnable(false);

	//フォグの設定
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

	//データのロード
	scenes_.front()->LoadData();
}

void SceneManager::ChangeAllScene(std::shared_ptr<SceneBase> scene)
{
	//フェード開始
	StartFadeIn();

	scenes_.clear();
	scenes_.push_back(scene);
	//データのロード
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

	// デルタタイム
	auto nowTime = std::chrono::system_clock::now();
	deltaTime_ = static_cast<float>(
		std::chrono::duration_cast<std::chrono::nanoseconds>(nowTime - preTime_).count() / 1000000000.0);
	preTime_ = nowTime;
	totalTime_ += GetDeltaTime();

	//更新
	camera_->Update();

	//画面揺れ計算
	CalcScreenShake();

	//フェード更新
	Fade();

	//シーン更新
	scenes_.front()->Update(input);

}

void SceneManager::Draw()
{
	// 描画先グラフィック領域の指定
	SetDrawingScreen(mainScreen_);

	//シーン描画(最後尾から描画)
	auto rit = scenes_.rbegin();
	for (; rit != scenes_.rend(); rit++)
	{
		(*rit)->Draw();
	}

	// 暗転・明転
	fader_->Draw();

	// 描画先グラフィック領域の指定
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();
	DrawGraph(static_cast<int>(screenPos_.x), static_cast<int>(screenPos_.y), mainScreen_, true);

	// デバック用描画
#ifdef _DEBUG
	//camera_->Draw();
	DrawFormatString(0, 0, 0xffffff, L"FPS:%.2f", GetFPS());
#endif // _DEBUG

}

void SceneManager::ChangeScene(SCENE_ID nextId)
{
	//フェード中は実行しない
	if (!fader_->IsEnd())return;

	DeleteLightHandleAll();


	// フェード処理が終わってからシーンを変える場合もあるため、
	// 遷移先シーンをメンバ変数に保持
	waitSceneId_ = nextId;

	// フェードアウト(暗転)を開始する
	fader_->SetFade(Fader::STATE::FADE_OUT);
	isSceneChanging_ = true;

	//フェードカラーの指定
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
	// フェードインを開始する
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

	// デルタタイム
	deltaTime_ = DELTA_TIME;

	//初期化
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
	// シーンを変更する
	sceneId_ = sceneId;

	//音源の全停止
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

	//フェード中か判断
	Fader::STATE fState = fader_->GetState();
	switch (fState)
	{
	case Fader::STATE::FADE_IN:
		// 明転中
		if (fader_->IsEnd())
		{
			// 明転が終了したら、フェード処理終了
			fader_->SetFade(Fader::STATE::NONE);
		}
		break;
	case Fader::STATE::FADE_OUT:
		// 暗転中
		if (fader_->IsEnd())
		{
			//カメラモードのリセット
			camera_->ChangeMode(Camera::MODE::FIXED_POINT);

			// 明転が終了したら、フェード処理終了
			fader_->SetFade(Fader::STATE::NONE);

			// 完全に暗転してからシーン遷移
			DoChangeScene(waitSceneId_);
		}
		break;
	}

}


void SceneManager::CalcScreenShake(void)
{
	//スクリーンの座標
	if (amplitude_ > 0.0f) {
		//スクリーン揺らし
		screenPos_.x = ((frame_ % 3) - 1) * amplitude_;
		screenPos_.y = (((frame_ + rand()) % 3) - 1) * amplitude_;
	}

	//揺れを小さくしていく
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

	//カメラ設定
	camera_->SetBeforeDraw();
}


