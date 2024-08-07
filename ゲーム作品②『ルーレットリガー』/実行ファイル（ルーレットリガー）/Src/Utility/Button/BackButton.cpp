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
    //画像の登録
    backImg_ = LoadGraph("Data/Image/UI/BackBtn.png");

    frameColorDepth_ = FRAME_COLOR_DEPTH_NORMAL;
}

void BackButton::Update(void)
{
    //マウスがボタン上にあるか確認
    if (MouseOnButton())
    {
        frameColorDepth_ = FRAME_COLOR_DEPTH_CLICK;
    }

    //ボタンがクリックされたか確認
    if (PushButton())
    {
        frameColorDepth_ = FRAME_COLOR_DEPTH_NORMAL;
    }
}

void BackButton::Draw(void)
{
    auto& ds = DrawShader::GetInstance();

    //背面画像
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
    //初期化
    Init();

    center_ = centerPos;


    //表示位置を半径分ずらす
    int x, y;
    GetGraphSize(backImg_, &x, &y);
    rad_ = x / 2;
    pos_ = { center_.x - rad_,center_.y - rad_ };

}

bool BackButton::MouseOnButton(void)
{
    //マウス位置
    Vector2 mPos = InputManager::GetInstance().GetMousePos();

    if (IsMouseInCircle(center_, mPos, rad_))return true;

    return false;
}

bool BackButton::IsMouseInCircle(const Vector2& cPos, const Vector2& mPos, const int& rad)
{
    //半径
    int dis = static_cast<int>(Utility::Distance(cPos, mPos));
    if (dis < rad)
    {
        return true;
    }

    return false;
}
