#pragma once
#include <string>
#include "../../Manager/DataManager/CmdManager.h"
#include "ButtonUI.h"
class CmdButton :
    public ButtonUI
{
public:

    CmdButton();
    ~CmdButton();

    void Init(void)override;
    void Update(void)override;
    void Draw(void)override;
    void Release(void)override;

    //ボタン生成
    void Create(const Vector2& pos, const Vector2& size, const int& cmdNum);

    //ボタンの上にマウスが乗っているかどうか
    bool MouseOnButton(void);

    //コマンド名の再セット
    void SetCmd(const int& cmdNum);

    //コマンド詳細描画
    void DrawCmdDetail(void);

private:
    //ボタンサイズ
    Vector2 size_;

    //コマンド番号
    int cmdNum_;

    //コマンド名
    std::string cmdName_;

    //コマンド詳細背面
    int cmtDetailBack_;

    //フォント
    int fontHandle_;
    //コマンドボタン画像
    int btnImg_;


    //四角形上にマウスがあるかの確認
    bool IsMouseInRect(
        const Vector2& inside, const Vector2& outside, const Vector2& size);

    //バフの表示名変更
    std::string ChageDenoteBuffName(const std::string& buff);


};

