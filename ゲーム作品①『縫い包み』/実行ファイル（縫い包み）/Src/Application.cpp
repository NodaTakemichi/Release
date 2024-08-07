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

	// �A�v���P�[�V�����̏����ݒ�
	SetWindowText(L"�D�����");

	// �E�B���h�E�T�C�Y
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 32);
	ChangeWindowMode(true);

	//Xaudio2�̏�����
	SetEnableXAudioFlag(TRUE);

	// DxLib�̏�����
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	isInitFail_ = false;
	if (DxLib_Init() == -1)
	{
		isInitFail_ = true;
		return;
	}

	//Effekseer�̏�����
	InitEffekseer();
	// �L�[���䏉����
	SetUseDirectInputFlag(true);

	//���\�[�X�Ǘ��N���X
	ResourceManager::GetInstance();

	//�E�B���h�E�̏I�����f
	isExitWindows_ = false;

}

void Application::Run(void)
{

	auto& inputManager = InputManager::GetInstance();
	auto& soundManager = SoundManager::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();

	//�����V�[��
	sceneManager.CreateScene(std::make_shared<TitleScene>(sceneManager));


	// �Q�[�����[�v
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
	//���\�[�X�̉��
	ResourceManager::GetInstance().Destroy();

	// Effekseer���I������B
	Effkseer_End();

	// DxLib�I��
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
