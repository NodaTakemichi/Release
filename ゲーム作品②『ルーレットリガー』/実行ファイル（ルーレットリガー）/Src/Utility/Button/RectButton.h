#pragma once
#include "ButtonUI.h"
class RectButton :
    public ButtonUI
{
public:

    RectButton();
    ~RectButton();

    void Init(void)override;
    void Update(void)override;
    void Draw(void)override;
    void Release(void)override;

    //ボタン生成
    void Create(Vector2 pos, int back, int front = 0);
    void Create(Vector2 pos, Vector2 size, int back, int front = 0);

    //ボタンの上にマウスが乗っているかどうか
    bool MouseOnButton(void);

    //移動後座標までの移動値をセット
    void SetMovedPos(Vector2 moveDis);



private:
    //ボタンサイズ
    Vector2 size_;


    //移動後、移動前座標
    Vector2 beforMovePos_;
    Vector2 afterMovePos_;
    //移動経過時間
    float movingTime_;

    //四角形上にマウスがあるかの確認
    bool IsMouseInRect(
        const Vector2& inside, const Vector2& outside, const Vector2& size);

    //移動座標についているかどうか判断
    bool IsGoalPos(void);
    //座標移動
    void MovePosProcess(void);

};

