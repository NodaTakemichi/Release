#pragma once
#include <DxLib.h>
#include <memory>
#include "../../Common/Quaternion.h"
#include "Collider.h"

class Transform
{

public:

	// コンストラクタ
	Transform(void);
	Transform(int model);

	// デストラクタ
	~Transform(void);

	// モデルのハンドルID
	int modelId_;

	// 大きさ
	VECTOR scl_;
	// 回転
	VECTOR rot_;
	// 位置
	VECTOR pos_;
	VECTOR localPos_;

	MATRIX matScl_;
	MATRIX matRot_;
	MATRIX matPos_;

	// 回転
	Quaternion quaRot_;

	// ローカル回転
	Quaternion quaRotLocal_;

	// コライダ
	std::shared_ptr<Collider> collider_;

	/// <summary>
	/// 3Dモデル情報の初期化
	/// </summary>
	/// <param name="scl">大きさ</param>
	/// <param name="pos">座標</param>
	/// <param name="quaRot">回転</param>
	/// <param name="quaRotLocal">ローカル回転</param>
	/// <param name="collType">コライダータイプ</param>
	void Init(VECTOR scl, VECTOR pos, Quaternion quaRot, Quaternion quaRotLocal, Collider::TYPE collType);

	/// <summary>
	/// モデル制御の基本情報更新
	/// </summary>
	void Update(void);
	
	void Release(void);

	void SetModel(int modelHId);
	void MakeCollider(Collider::TYPE type);

	// 前方方向を取得
	VECTOR GetForward(void) const;

	// 後方方向を取得
	VECTOR GetBack(void) const;

	// 右方向を取得
	VECTOR GetRight(void) const;

	// 左方向を取得
	VECTOR GetLeft(void) const;

	// 上方向を取得
	VECTOR GetUp(void) const;

	// 下方向を取得
	VECTOR GetDown(void) const;

	// 対象方向を取得
	VECTOR GetDir(const VECTOR& vec) const;

	// 座標を取得
	VECTOR GetPos(void) const;

};
