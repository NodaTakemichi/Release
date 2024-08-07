#include <DxLib.h>
#include "../../Manager/DataManager/SoundManager.h"
#include "../../Manager/InputManager.h"
#include "../../Utility/DrawShader.h"
#include "ButtonUI.h"

ButtonUI::ButtonUI()
{
	pos_ = Vector2{};
	backImg_=0;
	frontImg_ = 0;
	frameColorDepth_=0.0f;
	onBtnBefor_ = 0.0f;
	onBtn_ = 0;
	clickSE_ = 0;
	onBtnSE_= 0;

}

ButtonUI::~ButtonUI()
{
}

bool ButtonUI::PushButton(void)
{
	auto& ins = InputManager::GetInstance();
	if (MouseOnButton() && ins.IsClickMouseLeft())return true;

	return false;
}

bool ButtonUI::ButtonDecision(void)
{

	auto& ins = InputManager::GetInstance();
	if (MouseOnButton() && ins.IsTrgMouseLeftUp())return true;

	return false;
}

void ButtonUI::PlayOnBtnSE(void)
{
	if ((onBtn_ && !onBtnBefor_))
	{
		//�}�E�X���{�^����ɏ�����u�Ԃ̂݁ASE�Đ�
		auto& snd = SoundManager::GetInstance();
		snd.PlaySoundBack(onBtnSE_);
	}

	onBtnBefor_ = onBtn_;

}

void ButtonUI::PlayDecSE(void)
{
	if (!ButtonDecision())return;

	//����{�^�����������ꂽ�Ƃ��ASE�Đ�
	auto& snd = SoundManager::GetInstance();
	snd.PlaySoundBack(clickSE_);
}
