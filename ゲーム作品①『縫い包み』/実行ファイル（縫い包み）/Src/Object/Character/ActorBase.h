#pragma once
#include <vector>
#include <functional>
#include "../ObjBase.h"

class Capsule;
class Collider;


class ActorBase :
    public ObjBase
{
public:
	ActorBase(void);
	virtual ~ActorBase(void);

	virtual void LoadData(void) = 0;
	virtual void Init(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;


	//衝突判定に用いられるステージコライダ制御
	void AddStageCollider(const Collider& collider);
	void ClearStageCollider(void);

	//衝突用カプセルの取得
	const Capsule& GetCapsule(void)const;

	//キャラクター半径の取得
	float GetRadious(void);

	//ゲームオーバーの設定
	virtual void SetGameOverInfo(const VECTOR& opponentPos) = 0;

protected:
	float speed_;       //移動スピード
	VECTOR moveDir_;    //移動方向
	VECTOR movePow_;    //移動量
	VECTOR movedPos_;   //移動後の座標
	VECTOR beforePos_;   //移動前の座標
	Quaternion actorRotY_; //回転

	Quaternion goalQuaRot_; //ゴール角度
	float rotateTime_ = 0.0f;	//回転時間

	//停止状態かどうか
	bool isStoping_;

	//地面との衝突チェック用
	VECTOR gravHitPosDown_;
	VECTOR gravHitPosUp_;

	//自身のカプセルコライダ
	std::unique_ptr<Capsule>capsule_;
	//衝突判定用のコライダ
	std::vector<std::shared_ptr<Collider>>stageColliders_;

	//Updata関数で実行される関数を指し示すポインタ
	std::function<void(void)> updateFunc_;

	//衝突判定
	void Collision(void);
	void CollisionCapsule(void);
	void CollisionGravity(void);

	//Lineとステージコライダーの衝突判定 //true:衝突している、false:衝突していない
	bool CollCheckLine_AllStage(const VECTOR& front, const VECTOR& back);
	bool CollCheckLine_Stage(const VECTOR& front, const VECTOR& back,const Collider::TYPE& type);

	//移動量の計算
	void CalcGravityPow(void);
	//移動後角度を求める
	Quaternion CalcQuaRad(const VECTOR& pos1, const VECTOR& pos2);

	//更新処理
	virtual void UpdateGameOver(void) = 0;      //ゲームオーバー状態の更新

private:

};

