#pragma once
#include <DxLib.h>
#include <memory>
#include "../Common/Quaternion.h"
class Transform;
class InputManager;

class Camera
{
public:
	//カメラモード
	enum class MODE
	{
		NONE,
		FIXED_POINT,
		FREE,
		FPS,
		GAME_OVER_AUTO,
		ROTATION,

	};

	Camera(void);
	~Camera(void);

	void Init(void);
	void Update(void);
	void Draw(void);


	// カメラ位置
	VECTOR GetPos(void) const;
	// カメラの操作角度
	VECTOR GetAngles(void) const;
	// カメラの注視点
	VECTOR GetTargetPos(void) const;
	// カメラ角度
	Quaternion GetQuaRot(void) const;
	// X回転を抜いたカメラ角度
	Quaternion GetQuaRotOutX(void) const;
	// カメラの前方方向
	VECTOR GetForward(void) const;

	//カメラの位置、角度の描画更新
	void SetBeforeDraw(void);
	//カメラモードの変更
	void ChangeMode(MODE mode);

	// 追従対象の設定
	void SetFollow(const Transform* follow);
	// 注視点の設定
	void SetTargetPos(const VECTOR& targetPos);

	//ゲームオーバー時のカメラ設定
	void SetGameOverRot(const VECTOR& targetPos);

private:
	// カメラが追従対象とするTransform
	const Transform* followTransform_;

	
	VECTOR pos_;	// カメラの位置
	VECTOR angles_;	// カメラの角度
	Quaternion rotOutX_;// X回転がない角度
	Quaternion rot_;	// カメラ角度
	VECTOR targetPos_;	// 注視点
	VECTOR cameraUp_;	// カメラの上方向

	//カメラモード
	MODE mode_;

	//回転時間
	float rotateTime_;
	//ゴール角度
	Quaternion goalQuaRot_;
	Quaternion goalDir_;

	//カメラ軸回転
	float camearAxisRot_;

	// カメラを初期位置に戻す
	void SetDefault(void);
	// カメラ操作
	void SyncFollow(void);

	//モード別更新ステップ
	void SetBeforeDrawFixedPoint(void);
	void SetBeforeDrawFree(void);
	void SetBeforeDrawFollow(void);
	void SetBeforeDrawGameOverAuto(void);
	void SetBeforeDrawRotationTitle(void);

};

