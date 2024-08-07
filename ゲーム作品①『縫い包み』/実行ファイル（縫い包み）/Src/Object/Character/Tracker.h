#pragma once
#include <list>
#include <unordered_map>
#include "ActorBase.h"
class Capsule;
class Collider;
class AnimationController;
class SoundController;
class Player;

class Tracker :
    public ActorBase
{
public:

    struct MovePoint
    {
        int goledNum;   //ゴールした回数
        VECTOR pos;     //座標
        bool warpPoint; //ワープポイントかどうか
    };

    // アニメーション種別
    enum class ANIM_TYPE
    {
        IDLE,
        RUN,
        FAST_RUN,
        LOOKING,
        STUN,
        CHACH,
        LOST,
    };


    Tracker(const Player& player);
    virtual ~Tracker(void);
    void LoadData(void)override;
    void Init(void)override;
    void Update(void)override;
    void Draw(void)override;

    //ゲームオーバーの設定
    void SetGameOverInfo(const VECTOR& opponentPos)override;

    //気絶状態を取得
    const bool GetFainting(void)const { return isFainting_; }

private:
    //プレイヤー情報
    const Player& player_;

    //アニメーションコントローラー
    std::unique_ptr<AnimationController> animController_;

    //足音サウンド
    std::shared_ptr<SoundController>footstepsSound_;

    //移動ポイント
    std::vector<MovePoint>movePoints_;
    //移動可能ポイント番号
    std::vector<int>moveablePointNums_;
    //移動ポイント番号
    int currentMovePointNum_;

    //ゴール座標
    VECTOR goalPos_ = {};
    //気絶時間
    float faintingTime_ = 0.0f;
    //気絶しているか判断
    bool isFainting_;

    //移動方向の取得
    VECTOR GetMoveDir(const VECTOR& goalPos);

    //移動可能ポイント初期化
    void InitMovePoint(void);
    //移動可能ポイントの検索
    void SearchMoveablePoint(void);
    //移動可能ポイントからゴールを決定
    void DecisionGoalPoint(void);

    //ワープ可能か判断
    bool IsWarping(void);
    //ワープ処理
    void ProcessWarp(void);

    //ゴール判定
    bool IsGoal(void);
    //プレイヤーの発見判定
    bool IsDiscoveryPlayer(void);
    //プレイヤーを見失う判定
    bool IsLoseSightOfPlayer(void);
    //プレイヤーの発見エリア描画（デバック用）
    void DrawDiscoveryArea(void);
        
    void ProcessMove(void);     //移動処理
    void ProcessTrack(void);    //追跡処理
    void ProcessRotatePortal(void);    //回転処理(巡回中)
    void ProcessRotateTrack(void);     //回転処理(追跡中)

    //更新処理
    void UpdateNormal(void);                 //通常状態の更新
    void UpdatePatrol(void);                 //巡回状態の更新
    void UpdateTracking(void);               //追跡状態の更新
    void UpdateFainting(void);               //気絶状態の更新
    void UpdateLostPlayer(void);             //ロスト状態の更新
    void UpdateGameOver(void)override;       //ゲームオーバー状態の更新


};

