#pragma once
#include "ButtonUI.h"
class BackButton :
    public ButtonUI
{
    BackButton();
    ~BackButton();

    void Init(void)override;
    void Update(void)override;
    void Draw(void)override;
    void Release(void)override;

    //ボタン生成
    void Create(Vector2 centerPos);

    //ボタンの上にマウスが乗っているかどうか
    bool MouseOnButton(void);


private:
    //中心座標
    Vector2 center_;

    //ボタン半径
    int rad_;

    //円形内に座標があるかの確認
    bool IsMouseInCircle(
        const Vector2& cPos, const Vector2& mPos, const int& rad);


};

