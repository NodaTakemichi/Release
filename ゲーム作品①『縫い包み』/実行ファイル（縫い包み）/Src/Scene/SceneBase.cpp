#include <DxLib.h>
#include "../Utility/Utility.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "SceneBase.h"
#include "SceneBase.h"


SceneBase::SceneBase(SceneManager& scene) :sceneManager_(scene)
{
	loadingTime_ = 0.0f;
	buttnFontHandle_ = 0;
	cursolNum_ = 0;

}

SceneBase::~SceneBase(void)
{
}

void SceneBase::BtnSelectProcess(const int& btnNum)
{
	auto& input = InputManager::GetInstance();
	//選択肢操作
	if (input.IsTrgDown(KEY_INPUT_S) || input.IsTrgDown(KEY_INPUT_DOWN))
	{
		cursolNum_++;
	}
	else if (input.IsTrgDown(KEY_INPUT_W) || input.IsTrgDown(KEY_INPUT_UP))
	{
		cursolNum_--;
	}
	else
	{
		return;
	}

	//カーソル番号をボタン数に収める
	cursolNum_ = Utility::Wrap(cursolNum_, 0, btnNum);
}

void SceneBase::DrawNowLoading(void)
{
	//ロード中
	auto time = sceneManager_.GetTotalTime();
	int count = static_cast<int>(time / COMMA_TIME);
	count %= COMMA_MAX_NUM;

	std::wstring loadStr = L"now loading";
	std::wstring dotStr = L".";

	for (int i = 0; i < count; i++)
	{
		loadStr += dotStr;
	}
	DrawStringToHandle(LOADING_STRING_POS_X, LOADING_STRING_POS_Y, loadStr.c_str(), 0xffffff, buttnFontHandle_);

}

