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

//デルタタイム
constexpr float DELTA_TIME = 1.0f / 60.0f;
//クリックエフェクトサイズ
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
	//ウインドがアクティブ状態じゃなくても、処理を続行させる
	//SetAlwaysRunFlag(false);

	//マウスの非表示
	SetMouseDispFlag(false);

	// シェーダ―描画クラス初期化
	DrawShader::CreateInstance();

	// 音声管理初期化
	SoundManager::CreateInstance();

	// ユニットデータ管理初期化
	UnitDataManager::CreateInstance();

	// エフェクトデータ管理初期化
	EffectManager::CreateInstance();

	// コマンドデータ管理初期化
	CmdManager::CreateInstance();

	// バトルデータ管理初期化
	BattleDataManager::CreateInstance();

	// デッキ管理初期化
	DeckManager::CreateInstance();


	fader_ = new Fader();
	fader_->Init();

	scene_ = new TitleScene();
	scene_->Init();

	sceneID_ = SCENE_ID::TITLE;
	waitSceneID_ = SCENE_ID::NONE;

	isSceneChanging_ = false;

	// デルタタイム
	preTime_ = std::chrono::system_clock::now();
	totalTime_ = 0.0f;

	//乱数生成器の初期化
	gen_ = std::mt19937(rd_());

	//マウス画像
	mouseImg_ = LoadGraph("./Data/Image/UI/mouse.png");
	//クリックエフェクトハンドル
	clickEfHandle_ = LoadEffekseerEffect(
		"Data/Effect/Click/ClickEf.efkefc");

	//シーン遷移SE
	sceneMoveSE_ = SoundManager::GetInstance().LoadSound(
		SoundManager::SOUND_TYPE::SCENE_MOVE);

	//スクリーン作成
	int sx = Application::SCREEN_SIZE_X;
	int sy = Application::SCREEN_SIZE_Y;
	screen_ = MakeScreen(sx, sy, true);
	//振幅
	amplitude_ = 0.0f;
	//フレーム
	frame_ = 0;

}


void SceneManager::Update(void)
{

	if (scene_ == nullptr)
	{
		return;
	}

	// デルタタイム
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

	//クリックエフェクト
	DrawClickEf();

	// Effekseerにより再生中のエフェクトを更新する。
	UpdateEffekseer2D();

}

void SceneManager::Draw(void)
{
	// 描画先グラフィック領域の指定
	SetDrawScreen(DX_SCREEN_BACK);
	// 画面を初期化
	ClearDrawScreen();

	// 描画
	scene_->Draw();

	// Effekseerにより再生中のエフェクトを描画する。
	DrawEffekseer2D();
	
	fader_->Draw();

	//マウス描画
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

	//音声開放
	InitSoundMem();

	// シェーダ―描画の解放
	DrawShader::GetInstance().Release();

	//音声管理クラスの解放
	SoundManager::GetInstance().Release();

	//ユニットデータ管理クラスの開放
	UnitDataManager::GetInstance().Release();

	//コマンドデータ管理クラスの開放
	CmdManager::GetInstance().Release();

	//エフェクトデータ管理クラスの開放
	EffectManager::GetInstance().Release();

	//バトルデータ管理クラスの開放
	BattleDataManager::GetInstance().Release();

	//デッキ管理クラスの開放
	DeckManager::GetInstance().Release();

}

void SceneManager::ChangeScene(SCENE_ID nextId, bool isFading)
{

	//シーン遷移SE
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
	//一様分布
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

	// デルタタイム
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
	//フォントの総削除
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
		//ロードしたエフェクトの解放
		EffectManager::GetInstance().LoadedEffectRelease();
		break;
	default:
		break;
	}

	scene_->Init();

	// 計測リセット
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
		//エフェクト再生
		efPlayHandle_ = PlayEffekseer2DEffect(clickEfHandle_);

		//エフェクトの大きさ
		SetScalePlayingEffekseer2DEffect(
			efPlayHandle_, CLICK_EFFECT_SIZE, CLICK_EFFECT_SIZE, CLICK_EFFECT_SIZE);
		//エフェクトの位置
		Vector2 mousePos = ins.GetMousePos();
		SetPosPlayingEffekseer2DEffect(
			efPlayHandle_, mousePos.x, mousePos.y, 0);

	}
}

