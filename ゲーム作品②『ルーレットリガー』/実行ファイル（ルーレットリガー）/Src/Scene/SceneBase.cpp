#include <DxLib.h>
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Application.h"

#include "SceneBase.h"

SceneBase::SceneBase(void)
{
	bgImg_=0;
	bgmHandle_=0;
	totalTime_=0.0f;
}

SceneBase::~SceneBase()
{
}

void SceneBase::DrawPosition(void)
{
#ifdef _DEBUG
	//デバック用
	auto cx = Application::SCREEN_SIZE_X;
	auto cy = Application::SCREEN_SIZE_Y;
	auto span = 20;
	for (size_t i = 0; i < 100; i++)
	{
		//X
		DrawLine(0, i * span, cx, i * span, 0xff0000);

		//Y
		DrawLine(i * span, 0, i * span, cy, 0x0000ff);
	}
#endif // DEBUG

}

