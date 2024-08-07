#include <DxLib.h>
#include "../../Manager/InputManager.h"
#include "../../Utility/DrawShader.h"
#include "../Utility.h"
#include "BackButton.h"
using PsType = DrawShader::PS_TYPE;

BackButton::BackButton()
{
    center_ = Vector2{};
    rad_=0;
}

BackButton::~BackButton()
{
}

void BackButton::Init(void)
{
    //�摜�̓o�^
    backImg_ = LoadGraph("Data/Image/UI/BackBtn.png");

    frameColorDepth_ = FRAME_COLOR_DEPTH_NORMAL;
}

void BackButton::Update(void)
{
    //�}�E�X���{�^����ɂ��邩�m�F
    if (MouseOnButton())
    {
        frameColorDepth_ = FRAME_COLOR_DEPTH_CLICK;
    }

    //�{�^�����N���b�N���ꂽ���m�F
    if (PushButton())
    {
        frameColorDepth_ = FRAME_COLOR_DEPTH_NORMAL;
    }
}

void BackButton::Draw(void)
{
    auto& ds = DrawShader::GetInstance();

    //�w�ʉ摜
    if (!MouseOnButton())
    {
        DrawGraph(pos_.x, pos_.y, backImg_, true);
    }
    else
    {
        COLOR_F buf = COLOR_F{
            frameColorDepth_
        };
        ds.DrawGraphToShader(
            pos_, backImg_, PsType::ON_BUTTON, buf);
    }
}

void BackButton::Release(void)
{
    DeleteGraph(backImg_);
}

void BackButton::Create(Vector2 centerPos)
{
    //������
    Init();

    center_ = centerPos;


    //�\���ʒu�𔼌a�����炷
    int x, y;
    GetGraphSize(backImg_, &x, &y);
    rad_ = x / 2;
    pos_ = { center_.x - rad_,center_.y - rad_ };

}

bool BackButton::MouseOnButton(void)
{
    //�}�E�X�ʒu
    Vector2 mPos = InputManager::GetInstance().GetMousePos();

    if (IsMouseInCircle(center_, mPos, rad_))return true;

    return false;
}

bool BackButton::IsMouseInCircle(const Vector2& cPos, const Vector2& mPos, const int& rad)
{
    //���a
    int dis = static_cast<int>(Utility::Distance(cPos, mPos));
    if (dis < rad)
    {
        return true;
    }

    return false;
}
