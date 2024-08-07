#include <EffekseerForDXLib.h>
#include"../Manager/InputManager.h"
#include"../Manager/SceneManager.h"
#include"../Common/Quaternion.h"
#include"../Utility/Utility.h"
#include"../Object/Common/Transform.h"
#include "Camera.h"

// クリップ距離：Near
constexpr float CAMERA_NEAR = 30.0f;
// クリップ距離：Far
constexpr float CAMERA_FAR = 2000.0f;
// カメラ回転量
constexpr float CAMERA_ROTATE_ANGLE_DEG = -0.15f;
// カメラ回転量
constexpr float CAMERA_ROTATE_SPEED = 150.0f;
// カメラ移動量
constexpr float CAMERA_MOVE_POW = 8.0f;
// カメラスピード
constexpr float CAMERA_SPEED = 0.15f;
// カメラの初期座標
constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, -1500.0f, -1000.0f };
// 追従位置からカメラ位置までの相対座標
constexpr VECTOR LOCAL_F2C_POS = { 0.0f, 170.0f, 0.0f };
// 追従位置から注視点までの相対座標
constexpr VECTOR LOCAL_F2T_POS = { 0.0f, 170.0f, 500.0f };
// カメラのX回転上限度角
constexpr float LIMIT_X_UP_RAD = 60.0f * (DX_PI_F / 180.0f);
constexpr float LIMIT_X_DW_RAD = 25.0f * (DX_PI_F / 180.0f);

Camera::Camera(void)
{
	followTransform_ = nullptr;

	pos_ = VECTOR();
	angles_ = VECTOR();
	rotOutX_ = Quaternion();
	rot_ = Quaternion();
	targetPos_ = VECTOR();
	cameraUp_ = VECTOR();

	camearAxisRot_ = 0.0f;
	rotateTime_=0.0f;
	goalQuaRot_= Quaternion();
	goalDir_= Quaternion();

	mode_=MODE::NONE;


}

Camera::~Camera(void)
{
}

void Camera::Init(void)
{
	// 定点カメラを初期状態にする
	ChangeMode(MODE::FIXED_POINT);
}

void Camera::Update(void)
{
	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		SetBeforeDrawFixedPoint();
		break;
	case Camera::MODE::FREE:
		SetBeforeDrawFree();
		break;
	case Camera::MODE::FPS:
		SetBeforeDrawFollow();
		break;
	case Camera::MODE::GAME_OVER_AUTO:
		SetBeforeDrawGameOverAuto();
		break;
	case Camera::MODE::ROTATION:
		SetBeforeDrawRotationTitle();
		break;
	}
}

void Camera::SetBeforeDrawFixedPoint(void)
{
	//何もしない
}

void Camera::SetBeforeDrawFree(void)
{
	//入力情報
	auto& input = InputManager::GetInstance();

	// WASDでカメラの位置を変える
	float movePow = CAMERA_MOVE_POW;
	auto dir = Utility::VECTOR_ZERO;
	if (input.IsNew(KEY_INPUT_W)) { dir = rot_.GetForward(); }
	if (input.IsNew(KEY_INPUT_S)) { dir = rot_.GetBack(); }
	if (input.IsNew(KEY_INPUT_A)) { dir = rot_.GetLeft(); }
	if (input.IsNew(KEY_INPUT_D)) { dir = rot_.GetRight(); }
	if (input.IsNew(KEY_INPUT_Q)) { pos_.y += movePow; }
	if (input.IsNew(KEY_INPUT_E)) { pos_.y -= movePow; }
	pos_ = VAdd(pos_, VScale(dir, movePow));


	// マウスでカメラの角度を変える
	auto mouseMove = input.GetMouseMovePow();
	float speed = CAMERA_SPEED;
	//左右
	if (mouseMove.x > 0)
	{
		// 右回転
		angles_.y += Utility::Deg2RadF(speed);
	}
	if (mouseMove.x < 0)
	{
		// 左回転
		angles_.y += Utility::Deg2RadF(-speed);
	}

	//カメラの上下方向の操作
	if (mouseMove.y > 0)
	{
		angles_.x += Utility::Deg2RadF(speed);
		if (angles_.x > LIMIT_X_UP_RAD)
		{
			angles_.x = LIMIT_X_UP_RAD;
		}
	}
	if (mouseMove.y < 0)
	{
		angles_.x += Utility::Deg2RadF(-speed);
		if (angles_.x < -LIMIT_X_DW_RAD)
		{
			angles_.x = -LIMIT_X_DW_RAD;
		}
	}


	// 正面から設定されたY軸分、回転させる
	rotOutX_ = Quaternion::AngleAxis(angles_.y, Utility::AXIS_Y);
	// 正面から設定されたX軸分、回転させる
	rot_ = rotOutX_.Mult(Quaternion::AngleAxis(angles_.x, Utility::AXIS_X));
	// カメラの上方向
	cameraUp_ = Utility::DIR_U;

}

void Camera::SetBeforeDrawFollow(void)
{
	//入力情報
	auto& input = InputManager::GetInstance();
	auto movePow = input.GetMouseMovePow();

	if (movePow.x != 0)
	{
		// 横回転
		angles_.y += Utility::Deg2RadF(CAMERA_SPEED * movePow.x);
	}

	//カメラの上下方向の操作
	// 上回転
	if (movePow.y > 0)
	{
		angles_.x += Utility::Deg2RadF(CAMERA_SPEED * movePow.y);
		if (angles_.x > LIMIT_X_UP_RAD)
		{
			angles_.x = LIMIT_X_UP_RAD;
		}
	}
	// 下回転
	if (movePow.y < 0)
	{
		angles_.x += Utility::Deg2RadF(-CAMERA_SPEED *abs(movePow.y));
		if (angles_.x < -LIMIT_X_DW_RAD)
		{
			angles_.x = -LIMIT_X_DW_RAD;
		}
	}

	//ターゲットとの同期
	SyncFollow();
}

void Camera::SetBeforeDrawGameOverAuto(void)
{
	//球面補完
	rotateTime_ += SceneManager::GetInstance().GetDeltaTime();
	float par = rotateTime_ / 0.1f;
	rot_ = Quaternion::Slerp(rot_, goalQuaRot_, par);

	//オイラー角へ変換
	angles_ = rot_.ToEuler();

	// 同期先の位置
	VECTOR pos = followTransform_->pos_;
	VECTOR localPos;
	// 注視点(通常重力でいうところのY値を追従対象と同じにする)
	localPos = rot_.PosAxis(LOCAL_F2T_POS);
	targetPos_ = VAdd(pos, localPos);

	// カメラの上方向
	cameraUp_ = Utility::DIR_U;

}

void Camera::SetBeforeDrawRotationTitle(void)
{
	float rotateAngleDeg = CAMERA_ROTATE_ANGLE_DEG;
	camearAxisRot_ += Utility::Deg2RadF(rotateAngleDeg);
	
	//回転速度
	float rotateSpeed = CAMERA_ROTATE_SPEED;

	pos_.x = targetPos_.x + rotateSpeed * cosf(camearAxisRot_);
	pos_.z = targetPos_.z + rotateSpeed * sinf(camearAxisRot_);
	
	//角度
	angles_ = VNorm(VSub(targetPos_, pos_));

}

VECTOR Camera::GetPos(void) const
{
	return pos_;
}

VECTOR Camera::GetAngles(void) const
{
	return angles_;
}

VECTOR Camera::GetTargetPos(void) const
{
	return targetPos_;
}

Quaternion Camera::GetQuaRot(void) const
{
	return rot_;
}

Quaternion Camera::GetQuaRotOutX(void) const
{
	return rotOutX_;
}

VECTOR Camera::GetForward(void) const
{
	return VNorm(VSub(targetPos_, pos_));
}

void Camera::Draw(void)
{
#ifdef DEBUG
	DrawFormatString(10, 500, 0xffffff,
		L"カメラ座標：(% .2f, % .2f, % .2f)", pos_.x, pos_.y, pos_.z);
	DrawFormatString(10, 520, 0xffffff, L"カメラ角度：(%.2f, %.2f, %.2f)",
		angles_.x * 180.0f / DX_PI_F,
		angles_.y * 180.0f / DX_PI_F,
		angles_.z * 180.0f / DX_PI_F);

#endif // DEBUG
}

void Camera::SetBeforeDraw(void)
{
	// クリップ距離の設定(SetDrawScreenでリセットされる)
	SetCameraNearFar(CAMERA_NEAR, CAMERA_FAR);

	if (mode_ == MODE::ROTATION) {
		// カメラの設定(位置、注視点、上方向)
		SetCameraPositionAndTargetAndUpVec(pos_, targetPos_, cameraUp_);
	}
	else
	{
		// カメラの設定(位置と角度による制御)
		SetCameraPositionAndAngle(
			pos_,
			angles_.x,
			angles_.y,
			angles_.z);
	}

	//DXLib とEffekseerのカメラ同期を行う
	Effekseer_Sync3DSetting();

}

void Camera::ChangeMode(MODE mode)
{
	// カメラモードの変更
	mode_ = mode;
	// 変更時の初期化処理
	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		SetDefault();
		break;
	case Camera::MODE::FREE:
		SetDefault();
		break;
	case Camera::MODE::FPS:
		SetDefault();
		break;
	case Camera::MODE::GAME_OVER_AUTO:
		break;
	case Camera::MODE::ROTATION:
		SetDefault();
		break;
	}
}

void Camera::SetFollow(const Transform* follow)
{
	followTransform_ = follow;
}

void Camera::SetTargetPos(const VECTOR& targetPos)
{
	targetPos_ = targetPos;
}

void Camera::SetGameOverRot(const VECTOR& targetPos)
{
	rotateTime_ = 0.0f;

	//ゴール角度
	VECTOR dir = VNorm(VSub(targetPos, pos_));
	goalQuaRot_ = Quaternion::LookRotation(dir);
	goalDir_ = dir;
}

void Camera::SetDefault(void)
{
	//追従対象のリセット
	followTransform_ = nullptr;

	// カメラの初期設定
	pos_ = DEFAULT_CAMERA_POS;

	// 注視点
	targetPos_ = Utility::VECTOR_ZERO;

	// カメラの上方向
	cameraUp_ = Utility::DIR_U;

	//カメラ角度
	angles_.x = Utility::Deg2RadF(0.0f);
	angles_.y = Utility::Deg2RadF(0.0f);
	angles_.z = 0.0f;

	rot_ = Quaternion();
}

void Camera::SyncFollow(void)
{
	// 同期先の位置
	VECTOR pos = followTransform_->pos_;

	// 正面から設定されたY軸分、回転させる
	rotOutX_ = Quaternion::AngleAxis(angles_.y, Utility::AXIS_Y);

	// 正面から設定されたX軸分、回転させる
	rot_ = rotOutX_.Mult(Quaternion::AngleAxis(angles_.x, Utility::AXIS_X));
	
	VECTOR localPos;
	// 注視点(通常重力でいうところのY値を追従対象と同じにする)
	localPos = rot_.PosAxis(LOCAL_F2T_POS);
	targetPos_ = VAdd(pos, localPos);

	// カメラ位置
	localPos = LOCAL_F2C_POS;
	pos_ = VAdd(pos, localPos);

	// カメラの上方向
	cameraUp_ = Utility::DIR_U;
}
