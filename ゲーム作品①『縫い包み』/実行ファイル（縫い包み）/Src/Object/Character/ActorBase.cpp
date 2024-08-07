#include "../Common/Capsule.h"
#include "../Common/Collider.h"
#include "../../Utility/Utility.h"
#include "ActorBase.h"

//�d�͂̋���
constexpr float GRAVITY_POW = 9.8f;
//�n�ʂƂ̓����蔻��ray�̋���
constexpr float CHECK_HIT_RAY_DIS = 10.0f;
//�㏸�l
constexpr float INCREASED_VALUE = 15.0f;
//�Փ˔�����s����
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
	// ���ݍ��W���N�_�Ɉړ�����W�����߂�
	movedPos_ = VAdd(transform_.pos_, movePow_);

	// �Փ�(�J�v�Z��)
	CollisionCapsule();

	// �Փ�(�d��)
	CollisionGravity();

	// �ړ�
	transform_.pos_ = movedPos_;
}

void ActorBase::CollisionCapsule(void)
{	
	// �J�v�Z�����ړ�������
	Transform trans = Transform(transform_);
	trans.pos_ = movedPos_;
	trans.Update();
	//�ړ��������J�v�Z���ƐڐG������s��
	Capsule cap = Capsule(*capsule_, trans);

	// �J�v�Z���Ƃ̏Փ˔���
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
					//�����߂���(�ړ���)
					float backPow = speed_;
					//�����߂�����
					VECTOR backDir = hit.Normal;

					movedPos_ = VAdd(movedPos_, VScale(backDir, backPow));
					// �J�v�Z�����ړ�������
					trans.pos_ = movedPos_;
					trans.Update();
					continue;
				}
				break;
			}
		}
		// ���o�����n�ʃ|���S�����̌�n��
		MV1CollResultPolyDimTerminate(hits);
	}
}

void ActorBase::CollisionGravity(void)
{
	// �d�͕���
	VECTOR dirGravity = Utility::DIR_D;

	// �d�͕����̔���
	VECTOR dirUpGravity = Utility::DIR_U;

	//�n�ʂƂ̓����蔻��ray	
	float checkPow = CHECK_HIT_RAY_DIS;
	gravHitPosUp_ = VAdd(movedPos_, VScale(dirUpGravity, checkPow));
	gravHitPosDown_ = VAdd(movedPos_, VScale(dirGravity, checkPow));
	for (const auto c : stageColliders_)
	{
		auto hit = MV1CollCheck_Line(c->modelId_, -1, gravHitPosUp_, gravHitPosDown_);

		if (hit.HitFlag > 0)
		{
			//�㏸�l
			const float up = INCREASED_VALUE;
			// �Փ˒n�_����A������Ɉړ�
			movedPos_ = VAdd(hit.HitPosition, VScale(dirUpGravity, up));
		}
	}
}

bool ActorBase::CollCheckLine_AllStage(const VECTOR& front, const VECTOR& back)
{
	for (const auto c : stageColliders_)
	{
		//�X�e�[�W�Ƃ̏Փ˔���
		auto hit = MV1CollCheck_Line(c->modelId_, -1, front, back);
		//�Փ˂��Ă���ꍇ
		if (hit.HitFlag > 0)return true;
	}
	return false;
}

bool ActorBase::CollCheckLine_Stage(const VECTOR& front, const VECTOR& back, const Collider::TYPE& type)
{
	for (const auto c : stageColliders_)
	{
		if (c->type_ != type)continue;

		//�X�e�[�W�Ƃ̏Փ˔���
		auto hit = MV1CollCheck_Line(c->modelId_, -1, front, back);
		//�Փ˂��Ă���ꍇ
		if (hit.HitFlag > 0)return true;
	}
	return false;
}

void ActorBase::CalcGravityPow(void)
{
	// �d�͕���
	VECTOR dirGravity = Utility::DIR_D;

	// ������
	auto fallingPow = VScale(dirGravity, GRAVITY_POW);

	//�ړ��ʂ̉��Z
	movePow_ = VAdd(fallingPow, movePow_);
}

Quaternion ActorBase::CalcQuaRad(const VECTOR& pos1, const VECTOR& pos2)
{
	float angleDiff = atan2f(pos1.x - pos2.x, pos1.z - pos2.z);
	Quaternion ret = Quaternion::AngleAxis(double(angleDiff), Utility::AXIS_Y);
	return ret;
}
