#include <DxLib.h>
#include "../../Manager/DataManager/SoundManager.h"
#include "../../Manager/InputManager.h"
#include "../../Utility/DrawShader.h"
#include "../Utility.h"
#include "CircleButton.h"

using PsType = DrawShader::PS_TYPE;

CircleButton::CircleButton()
{
    center_ = Vector2{};
    rad_=0;
}

CircleButton::~CircleButton()
{
}

void CircleButton::Init(void)
{
    frameColorDepth_ = FRAME_COLOR_DEPTH_NORMAL;

    //SE
    auto& snd = SoundManager::GetInstance();
    clickSE_= snd.LoadSound(SoundManager::SOUND_TYPE::BTN_BACK);
    onBtnSE_ = snd.LoadSound(SoundManager::SOUND_TYPE::BTN_ON);

    //ボタン上にマウスが乗っているかどうか
    onBtnBefor_ = false;
    onBtn_ = false;
}

void CircleButton::Update(void)
{
    //SE再生
    PlayOnBtnSE();
    PlayDecSE();


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

void CircleButton::Draw(void)
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
            pos_, backImg_, PsType::ON_CIRCLE_BUTTON, buf);
    }

    //手前画像
    if (frontImg_ <= 0)return;
    ds.DrawGraph(pos_, frontImg_);

}

void CircleButton::Release(void)
{

}

void CircleButton::Create(Vector2 center,int back, int front)
{
    //初期化
    Init();

    center_ = center;
    backImg_ = back;
    frontImg_ = front;

    int x, y;
    GetGraphSize(back, &x, &y);
    rad_ = x / 2;

    //表示位置にずらす
    pos_ = { center_.x - rad_,center_.y - rad_ };

}

bool CircleButton::MouseOnButton(void)
{
    //マウス位置
    Vector2 mPos = InputManager::GetInstance().GetMousePos();

    if (IsMouseInCircle(center_, mPos,  rad_))onBtn_ =  true;
    else onBtn_ = false;

    return onBtn_;
}


bool CircleButton::IsMouseInCircle(const Vector2& cPos, const Vector2& mPos, const int& rad)
{
    //半径
    int dis = static_cast<int>(Utility::Distance(cPos, mPos));
    if (dis < rad)
    {
        return true;
    }

    return false;
}
