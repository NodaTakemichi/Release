#include <DxLib.h>
#include "../../Utility/Utility.h"
#include "Transform.h"

Transform::Transform(void)
{
	modelId_ = -1;

	scl_ = Utility::VECTOR_ONE;
	rot_ = Utility::VECTOR_ZERO;
	pos_ = Utility::VECTOR_ZERO;
	localPos_ = Utility::VECTOR_ZERO;

	matScl_ = MGetIdent();
	matRot_ = MGetIdent();
	matPos_ = MGetIdent();
	quaRot_ = Quaternion();
	quaRotLocal_ = Quaternion();

	collider_ = nullptr;
}

Transform::Transform(int model)
{
	modelId_ = model;

	scl_ = Utility::VECTOR_ONE;
	rot_ = Utility::VECTOR_ZERO;
	pos_ = Utility::VECTOR_ZERO;
	localPos_ = Utility::VECTOR_ZERO;

	matScl_ = MGetIdent();
	matRot_ = MGetIdent();
	matPos_ = MGetIdent();
	quaRot_ = Quaternion();
	quaRotLocal_ = Quaternion();

	collider_ = nullptr;
}

Transform::~Transform(void)
{
}

void Transform::Init(VECTOR scl, VECTOR pos, Quaternion quaRot, Quaternion quaRotLocal, Collider::TYPE collType)
{
	scl_ = scl;
	pos_ = pos;
	quaRot_ = quaRot;
	quaRotLocal_ = quaRotLocal;
	if (Collider::TYPE::NONE != collType)
	{
		MakeCollider(collType);
	}

	//3D情報の更新
	Update();
}

void Transform::Update(void)
{

	// 大きさ
	matScl_ = MGetScale(scl_);

	// 回転
	rot_ = quaRot_.ToEuler();
	matRot_ = quaRot_.ToMatrix();

	// 位置
	matPos_ = MGetTranslate(pos_);

	// 行列の合成
	MATRIX mat = MGetIdent();
	mat = MMult(mat, matScl_);
	Quaternion q = quaRot_.Mult(quaRotLocal_);
	mat = MMult(mat, q.ToMatrix());
	mat = MMult(mat, matPos_);

	// 行列をモデルに判定
	if (modelId_ != -1)
	{
		MV1SetMatrix(modelId_, mat);
	}

	// 衝突判定の更新
	if (collider_ != nullptr)
	{
		MV1RefreshCollInfo(modelId_);
	}

}

void Transform::Release(void)
{
}

void Transform::SetModel(int model)
{
	modelId_ = model;
}

void Transform::MakeCollider(Collider::TYPE type)
{
	if (modelId_ == -1)
	{
		return;
	}

	collider_ = std::make_shared<Collider>(type, modelId_);
	int ret = MV1SetupCollInfo(modelId_, -1, 1, 1, 1);

}

VECTOR Transform::GetForward(void) const
{
	return GetDir(Utility::DIR_F);
}

VECTOR Transform::GetBack(void) const
{
	return GetDir(Utility::DIR_B);
}

VECTOR Transform::GetRight(void) const
{
	return GetDir(Utility::DIR_R);
}

VECTOR Transform::GetLeft(void) const
{
	return GetDir(Utility::DIR_L);
}

VECTOR Transform::GetUp(void) const
{
	return GetDir(Utility::DIR_U);
}

VECTOR Transform::GetDown(void) const
{
	return GetDir(Utility::DIR_D);
}

VECTOR Transform::GetDir(const VECTOR& vec) const
{
	return quaRot_.PosAxis(vec);
}

VECTOR Transform::GetPos(void) const
{
	return pos_;
}
