#include "../Common/Capsule.h"
#include "../Common/Collider.h"
#include "../../Utility/Utility.h"
#include "ActorBase.h"

//重力の強さ
constexpr float GRAVITY_POW = 9.8f;
//地面との当たり判定rayの距離
constexpr float CHECK_HIT_RAY_DIS = 10.0f;
//上昇値
constexpr float INCREASED_VALUE = 15.0f;
//衝突判定を行う回数
constexpr int CHECK_COLLISION_COUNT = 10;

ActorBase::ActorBase(void)
{
	moveDir_= VECTOR();   
	movePow_ = VECTOR();
	movedPos_ = VECTOR();
	beforePos_ = VECTOR();
	actorRotY_ = Quaternion();
	goalQuaRot_ = Quaternion();
	gravHitPosDown_ = VECTOR();
	gravHitPosUp_ = VECTOR();

	isStoping_ = true;
	rotateTime_ = 0.0f;
	speed_ = 0.0f;

}

ActorBase::~ActorBase(void)
{
}

void ActorBase::AddStageCollider(const Collider& collider)
{
	stageColliders_.push_back(std::make_shared<Collider>(collider));
}

void ActorBase::ClearStageCollider(void)
{
	stageColliders_.clear();
}

const Capsule& ActorBase::GetCapsule(void) const
{
	return *capsule_;
}

float ActorBase::GetRadious(void) 
{
	 return capsule_->GetRadius(); 
}

void ActorBase::Collision(void)
{
	// 現在座標を起点に移動後座標を決める
	movedPos_ = VAdd(transform_.pos_, movePow_);

	// 衝突(カプセル)
	CollisionCapsule();

	// 衝突(重力)
	CollisionGravity();

	// 移動
	transform_.pos_ = movedPos_;
}

void ActorBase::CollisionCapsule(void)
{	
	// カプセルを移動させる
	Transform trans = Transform(transform_);
	trans.pos_ = movedPos_;
	trans.Update();
	//移動させたカプセルと接触判定を行う
	Capsule cap = Capsule(*capsule_, trans);

	// カプセルとの衝突判定
	for (const auto c : stageColliders_)
	{
		auto hits = MV1CollCheck_Capsule(
			c->modelId_, -1,
			cap.GetPosTop(), cap.GetPosDown(), cap.GetRadius());

		for (int i = 0; i < hits.HitNum; i++)
		{
			auto hit = hits.Dim[i];


			for (int tryCnt = 0; tryCnt < CHECK_COLLISION_COUNT; tryCnt++)
			{
				int pHit = HitCheck_Capsule_Triangle(
					cap.GetPosTop(), cap.GetPosDown(), cap.GetRadius(),
					hit.Position[0], hit.Position[1], hit.Position[2]);

				if (pHit)
				{
					//押し戻す力(移動量)
					float backPow = speed_;
					//押し戻す方向
					VECTOR backDir = hit.Normal;

					movedPos_ = VAdd(movedPos_, VScale(backDir, backPow));
					// カプセルを移動させる
					trans.pos_ = movedPos_;
					trans.Update();
					continue;
				}
				break;
			}
		}
		// 検出した地面ポリゴン情報の後始末
		MV1CollResultPolyDimTerminate(hits);
	}
}

void ActorBase::CollisionGravity(void)
{
	// 重力方向
	VECTOR dirGravity = Utility::DIR_D;

	// 重力方向の反対
	VECTOR dirUpGravity = Utility::DIR_U;

	//地面との当たり判定ray	
	float checkPow = CHECK_HIT_RAY_DIS;
	gravHitPosUp_ = VAdd(movedPos_, VScale(dirUpGravity, checkPow));
	gravHitPosDown_ = VAdd(movedPos_, VScale(dirGravity, checkPow));
	for (const auto c : stageColliders_)
	{
		auto hit = MV1CollCheck_Line(c->modelId_, -1, gravHitPosUp_, gravHitPosDown_);

		if (hit.HitFlag > 0)
		{
			//上昇値
			const float up = INCREASED_VALUE;
			// 衝突地点から、少し上に移動
			movedPos_ = VAdd(hit.HitPosition, VScale(dirUpGravity, up));
		}
	}
}

bool ActorBase::CollCheckLine_AllStage(const VECTOR& front, const VECTOR& back)
{
	for (const auto c : stageColliders_)
	{
		//ステージとの衝突判定
		auto hit = MV1CollCheck_Line(c->modelId_, -1, front, back);
		//衝突している場合
		if (hit.HitFlag > 0)return true;
	}
	return false;
}

bool ActorBase::CollCheckLine_Stage(const VECTOR& front, const VECTOR& back, const Collider::TYPE& type)
{
	for (const auto c : stageColliders_)
	{
		if (c->type_ != type)continue;

		//ステージとの衝突判定
		auto hit = MV1CollCheck_Line(c->modelId_, -1, front, back);
		//衝突している場合
		if (hit.HitFlag > 0)return true;
	}
	return false;
}

void ActorBase::CalcGravityPow(void)
{
	// 重力方向
	VECTOR dirGravity = Utility::DIR_D;

	// 落下量
	auto fallingPow = VScale(dirGravity, GRAVITY_POW);

	//移動量の加算
	movePow_ = VAdd(fallingPow, movePow_);
}

Quaternion ActorBase::CalcQuaRad(const VECTOR& pos1, const VECTOR& pos2)
{
	float angleDiff = atan2f(pos1.x - pos2.x, pos1.z - pos2.z);
	Quaternion ret = Quaternion::AngleAxis(double(angleDiff), Utility::AXIS_Y);
	return ret;
}
