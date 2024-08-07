#include <DxLib.h>
#include "../../Manager/DataManager/SoundManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/InputManager.h"
#include "../../Utility/Utility.h"
#include "../../Utility/DrawShader.h"
#include "../../_debug/_DebugDispOut.h"
#include "RectButton.h"

using PsType = DrawShader::PS_TYPE;

//�{�^���ړ�����
constexpr float BUTTON_MOVE_TIME = 0.3f;


RectButton::RectButton()
{
	size_ = Vector2{};
	beforMovePos_ =Vector2{};
	afterMovePos_ =Vector2{};
	movingTime_=0.0f;

}

RectButton::~RectButton()
{
}

void RectButton::Init(void)
{
	frameColorDepth_ = FRAME_COLOR_DEPTH_NORMAL;
	movingTime_ = 0.0f;

	//SE
	auto& snd = SoundManager::GetInstance();
	clickSE_ = snd.LoadSound(SoundManager::SOUND_TYPE::BTN_CLICK);
	onBtnSE_ = snd.LoadSound(SoundManager::SOUND_TYPE::BTN_ON);

	//�{�^����Ƀ}�E�X������Ă��邩�ǂ���
	onBtnBefor_ = false;
	onBtn_ = false;

}

void RectButton::Update(void)
{
	if (!IsGoalPos())
	{
		//�ړ�����
		MovePosProcess();
	}

	//SE�Đ�
	PlayOnBtnSE();
	PlayDecSE();

	//�{�^�����N���b�N���ꂽ���m�F
	if (PushButton())
	{
		frameColorDepth_ = FRAME_COLOR_DEPTH_NORMAL;
		return;
	}

	//�}�E�X���{�^����ɂ��邩�m�F
	if (MouseOnButton())
	{
		frameColorDepth_ = FRAME_COLOR_DEPTH_CLICK;
	}

}

void RectButton::Draw(void)
{
	auto& ds = DrawShader::GetInstance();

	//�w�ʉ摜
	if (!MouseOnButton())
	{
		ds.DrawExtendGraphToShader(
			pos_, size_, backImg_);
	}
	else
	{
		//�o�ߎ���
		float totalTime_ = SceneManager::GetInstance().GetTotalTime();

		COLOR_F buf = COLOR_F{
			totalTime_,
			frameColorDepth_
		};
		//ds.DrawGraphToShader(
		//	pos_, backImg_, PsType::ON_BUTTON, buf);
		ds.DrawExtendGraphToShader(
			pos_, size_, backImg_, PsType::ON_BUTTON, buf);
	}

	//��O�摜
	if (frontImg_ <= 0)return;
	ds.DrawExtendGraphToShader(pos_, size_, frontImg_);

}

void RectButton::Release(void)
{

}

void RectButton::Create(Vector2 pos, int back, int front)
{
	//������
	Init();

	pos_ = afterMovePos_ = beforMovePos_ = pos;
	//size_ = size;
	backImg_ = back;
	frontImg_ = front;

	GetGraphSize(back, &size_.x, &size_.y);
}

void RectButton::Create(Vector2 pos, Vector2 size, int back, int front)
{
	//������
	Init();

	pos_ = afterMovePos_ = beforMovePos_ = pos;
	size_ = size;
	backImg_ = back;
	frontImg_ = front;
}

bool RectButton::MouseOnButton(void)
{

	//�}�E�X�ʒu
	auto& ins = InputManager::GetInstance();
	Vector2 mPos = ins.GetMousePos();

	if (IsMouseInRect(mPos, pos_, size_))onBtn_ =  true;
	else onBtn_ = false;

	return onBtn_;
}

bool RectButton::IsMouseInRect(const Vector2& inside, const Vector2& outside, const Vector2& size)
{
	if (
		inside.x > outside.x &&
		inside.x < outside.x + size.x &&
		inside.y > outside.y &&
		inside.y < outside.y + size.y
		)
	{
		return true;
	}

	return false;
}

void RectButton::SetMovedPos(Vector2 moveDis)
{
	afterMovePos_ += moveDis;
	return;
}

bool RectButton::IsGoalPos(void)
{
	if (pos_ == afterMovePos_)
	{
		return true;
	}
	return false;
}

void RectButton::MovePosProcess(void)
{
	movingTime_ += SceneManager::GetInstance().GetDeltaTime();
	float t = movingTime_ / BUTTON_MOVE_TIME;

	//���W�̐��`���
	pos_ = Utility::Lerp(beforMovePos_, afterMovePos_, t);

	if (t >= 1.0f || IsGoalPos())
	{
		movingTime_ = 0.0f;
		beforMovePos_ = pos_;
	}
}
