#pragma once
#pragma <string>
#include "ButtonUI.h"
#include "../../Manager/DataManager/UnitDataManager.h"

class UnitButton :
    public ButtonUI
{
public:

    UnitButton();
    ~UnitButton();

    void Init(void)override;
    void Update(void)override;
    void Draw(void)override;
    void Release(void)override;

    //ボタン生成
    void Create(Vector2 pos, int back, UnitData unit);

    //ボタンの上にマウスが乗っているかどうか
    bool MouseOnButton(void);

    //ユニットデータの変更
    void ChengeUnitData(const UnitData& data);


private:
    //ボタンサイズ
    Vector2 size_;

    //属性画像

    //ユニット画像、ユニット名
    int unitImg_;
    std::string name_;

    //フォントハンドル
    int fontHandle_;

    //枠の色
    COLOR_F fColor_;

    //四角形上にマウスがあるかの確認
    bool IsMouseInRect(
        const Vector2& inside, const Vector2& outside, const Vector2& size);


};

