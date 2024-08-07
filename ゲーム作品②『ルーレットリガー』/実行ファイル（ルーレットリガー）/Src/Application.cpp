#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "_debug/_DebugConOut.h"
#include "_debug/_DebugDispOut.h"
#include "Manager/InputManager.h"
#include "Manager/SceneManager.h"
#include "Utility/Measure.h"

#include"_debug/_DebugConOut.h"
#include"_debug/_DebugDispOut.h"

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
	SetWindowText("���[���b�g���K�[");

	// �E�B���h�E�T�C�Y
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 32);
	ChangeWindowMode(true);

	// DxLib�̏�����
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	isInitFail_ = false;
	if (DxLib_Init() == -1)
	{
		TRACE("DxLib �̏��������s");
		isInitFail_ = true;
		return;
	}

	_dbgSetup(SCREEN_SIZE_X, SCREEN_SIZE_Y, 255);

	// Effekseer�̏�����
	InitEffekseer();

	// �L�[���䏉����
	SetUseDirectInputFlag(true);
	InputManager::CreateInstance();

	// �f�o�b�O�@�\������
	Measure::CreateInstance();

	// �V�[���Ǘ�������
	SceneManager::CreateInstance();

	// ���͑��u�Ǘ�������
	InputManager::CreateInstance();
}

void Application::Run(void)
{

	auto& intpuManager = InputManager::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();

	// �Q�[�����[�v
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{

		Measure::GetInstance().Start();

		_dbgStartDraw();

		intpuManager.Update();
		sceneManager.Update();

 		sceneManager.Draw();

		_dbgDraw();


		ScreenFlip();
	}

}

void Application::Release(void)
{

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

Application::Application(void)
{
	isInitFail_ = false;
	isReleaseFail_ = false;
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

	//Effekseer��2D�\���̐ݒ�ɂ���
	Effekseer_Set2DSetting(SCREEN_SIZE_X, SCREEN_SIZE_Y);

	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	Effekseer_SetGraphicsDeviceLostCallbackFunctions();
}
