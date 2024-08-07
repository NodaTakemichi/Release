#pragma once
#include "../ObjBase.h"
class ItemBase :
    public ObjBase
{
public:
    enum class ITEM_TYPE {
        NONE,
        RABBIT,
        PIG,
        MONKEY,
        SHEEP,
        AMULET
    };

    ItemBase(void);
    virtual ~ItemBase(void);
    virtual void LoadData(void) = 0;
    virtual void Init(void) = 0;
    virtual void Update(void) = 0;
    virtual void Draw(void) = 0;

    /// <summary>
    /// アイテムオブジェクトの情報設定
    /// </summary>
    /// <param name="type">アイテムタイプ</param>
    /// <param name="scl">オブジェクトの大きさ</param>
    /// <param name="pos">オブジェクトの位置</param>
    /// <param name="rot">オブジェクトの角度</param>
    /// <returns></returns>
    const void SetModelInfo(
        const ITEM_TYPE& type,
        const VECTOR& scl,
        const VECTOR& pos,
        const VECTOR& rot);

    const bool GetHasItem(void)const { return hasItem_; }
    const void SetHasItem(const bool& hasItem) { hasItem_ = hasItem; }
    const bool IsAlive(void)const { return isAlive_; }
    const void SetIsAlive(const bool& alive) { isAlive_ = alive; }
    const ITEM_TYPE GetItemType(void)const { return type_; }

    //焼失演出の開始
    void StartBurnDirection(void);

protected:
    bool hasItem_;      //取得されているかどうか
    bool isAlive_;      //燃やされたかどうか

    //アイテム種類
    ITEM_TYPE type_;
    //焼却演出判断
    bool isBurnDirection_;


private:
    //モデルの選択
    int SelectModelHandle(void);

};

