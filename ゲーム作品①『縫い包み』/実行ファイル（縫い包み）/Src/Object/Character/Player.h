#pragma once
#include <memory>
#include "ActorBase.h"
class Capsule;
class ItemBase;
class GoalGate;
class SoundController;

class Player :
    public ActorBase
{
public:

    // 焼却完了時間
    static constexpr float BURN_END_TIME = 6.0f;
    // 護符の使用クールタイム
    static constexpr float AMULET_USE_COOL_TIME = 6.0f;

    Player(void);
    virtual ~Player(void);
    void LoadData(void)override;
    void Init(void)override;
    void Update(void)override;
    void Draw(void)override;

    //キャラクター方向の取得
    const VECTOR GetDirection(void)const;

    //アイテム情報の追加
    void AddItemInfo(const std::weak_ptr<ItemBase> coll);
    //アイテム情報の消去
    void ClearItemInfos(void);

    //護符の使用判定
    const bool IsUseAmulet(void)const { return isUseAmulet_; }
    const void SetUseAmulet(const bool& use) { isUseAmulet_ = use; }

    //脱出判定
    const bool IsEscape(void)const { return isEscape_; }

    //ゲームオーバーの設定
    void SetGameOverInfo(const VECTOR& opponentPos)override;

    //ゴールゲートの設定
    void SetGoalGate(std::weak_ptr<GoalGate> goalGate) { goalGate_ = goalGate; }


#pragma region Get<-PlayerUI
    //アクションテキストの取得
    const std::wstring& GetActionText(void)const { return actionText_; }
    //コメントテキストの取得
    const std::wstring& GetCommentText(void)const { return commentText_; }
    //焼却時間
    const float& GetBurnTime(void)const { return burnTime_; }
    //護符の所持枚数の取得
    const int& GetAmuletNum(void)const {return haveAmuletNum_;}
    //ぬいぐるみの所持判定の取得
    const bool& HaveTheDoll(void)const { return haveTheDoll_; }
#pragma endregion


private:

    //オブジェクト衝突用ray
    VECTOR objHitRayPosFront_;
    VECTOR objHitRayPosBack_;

    //アイテム情報
    std::vector<std::weak_ptr<ItemBase>>itemInfos_;
    //ゴールゲート情報
    std::weak_ptr<GoalGate>goalGate_;

    //護符：使用判定
    bool isUseAmulet_ = false;
    //所持している護符の数
    int haveAmuletNum_ = 0;
    //護符のクールタイム計測
    float amuletCoolTime_;

    //所持ぬいぐるみ
    std::weak_ptr<ItemBase>haveDoll_;
    //ぬいぐるみを所持しているかどうか
    bool haveTheDoll_ = false;
    //焼却時間
    float burnTime_ = 0.0f;

    //アクションコマンドの描画文字
    std::wstring actionText_;
    //プレイヤーコメントの描画文字
    std::wstring commentText_;

    //ライトハンドル
    int lightHandle_;

    //脱出判定
    bool isEscape_;

    //足音サウンド
    std::shared_ptr<SoundController>footstepsSound_;
    //取得サウンド
    std::shared_ptr<SoundController>getItemSound_;
    //護符使用サウンド
    std::shared_ptr<SoundController>useAmuletSound_;
    //護符焼失サウンド
    std::shared_ptr<SoundController>burnAmuletSound_;


    //アイテムとの視線衝突判定
    void CollisionItem(void);
    //歪み壁との視線衝突判定
    void CollisionGoalWall(void);
    //ゴールとの視線衝突判定
    void CollisionGoalDoor(void);

    //操作
    void ProcessMove(void);         //プレイヤーの動き
    void ProcessObtainItem(void);   //アイテムの入手
    void ProcessUseItem(void);      //アイテムの使用

    //ぬいぐるみの焼却判断
    bool CheckBurnTheDoll(void);
    //ぬいぐるみの全滅判定    true：全滅 / false：全滅していない
    bool CheckDeadAllDoll(void);

    //アクションコマンドの描画文字セット
    void SetActionText(std::wstring text);
    //コメントテキストの描画文字セット
    void SetCommentText(std::wstring text);

    //更新処理
    void UpdateSearch(void);        //探索中の更新
    void UpdateBurning(void);       //ぬいぐるみ焼却中の更新
    void UpdateGameOver(void)override;      //ゲームオーバー状態の更新


};

