#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "Manager/InputManager.h"
#include "Manager/SceneManager.h"
#include "Manager/ResourceManager.h"
#include "Manager/SoundManager.h"
#include "Scene/TitleScene.h"
#include "Scene/GameScene.h"
#include "Scene/ResultScene.h"
#include "Scene/GameOverScene.h"
#include "Application.h"

Application* Application::instance_ = nullptr;

void Application::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new Application();
	}
	instance_->Init();
}

Application& Application::GetInstance(void)
{
	return *instance_;
}

void Application::Init(void)
{

	// アプリケーションの初期設定
	SetWindowText(L"縫い包み");

	// ウィンドウサイズ
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 32);
	ChangeWindowMode(true);

	//Xaudio2の初期化
	SetEnableXAudioFlag(TRUE);

	// DxLibの初期化
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	isInitFail_ = false;
	if (DxLib_Init() == -1)
	{
		isInitFail_ = true;
		return;
	}

	//Effekseerの初期化
	InitEffekseer();
	// キー制御初期化
	SetUseDirectInputFlag(true);

	//リソース管理クラス
	ResourceManager::GetInstance();

	//ウィンドウの終了判断
	isExitWindows_ = false;

}

void Application::Run(void)
{

	auto& inputManager = InputManager::GetInstance();
	auto& soundManager = SoundManager::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();

	//初期シーン
	sceneManager.CreateScene(std::make_shared<TitleScene>(sceneManager));


	// ゲームループ
	while (ProcessMessage() == 0 && isExitWindows_ == false)
	{
		inputManager.Update();
		soundManager.Update();
		sceneManager.Updata(inputManager);

		sceneManager.Draw();

		ScreenFlip();

	}

}

void Application::Release(void)
{
	//リソースの解放
	ResourceManager::GetInstance().Destroy();

	// Effekseerを終了する。
	Effkseer_End();

	// DxLib終了
	if (DxLib_End() == -1)
	{
		isReleaseFail_ = true;
	}

}

void Application::Destroy(void)
{
	delete instance_;
}

bool Application::IsInitFail(void) const
{
	return isInitFail_;
}

bool Application::IsReleaseFail(void) const
{
	return isReleaseFail_;
}

void Application::CloseWindows(void)
{
	isExitWindows_ = true;
}

Application::Application(void)
{
	isInitFail_ = false;
	isReleaseFail_ = false;
	isExitWindows_ = false;
}

Application::~Application(void)
{
}

void Application::InitEffekseer(void)
{
	if (Effekseer_Init(8000) == -1)
	{
		DxLib_End();
	}

	SetChangeScreenModeGraphicsSystemResetFlag(false);
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();
}
