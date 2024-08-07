#include <EffekseerForDXLib.h>
#include"../Manager/InputManager.h"
#include"../Manager/SceneManager.h"
#include"../Common/Quaternion.h"
#include"../Utility/Utility.h"
#include"../Object/Common/Transform.h"
#include "Camera.h"

// �N���b�v�����FNear
constexpr float CAMERA_NEAR = 30.0f;
// �N���b�v�����FFar
constexpr float CAMERA_FAR = 2000.0f;
// �J������]��
constexpr float CAMERA_ROTATE_ANGLE_DEG = -0.15f;
// �J������]��
constexpr float CAMERA_ROTATE_SPEED = 150.0f;
// �J�����ړ���
constexpr float CAMERA_MOVE_POW = 8.0f;
// �J�����X�s�[�h
constexpr float CAMERA_SPEED = 0.15f;
// �J�����̏������W
constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, -1500.0f, -1000.0f };
// �Ǐ]�ʒu����J�����ʒu�܂ł̑��΍��W
constexpr VECTOR LOCAL_F2C_POS = { 0.0f, 170.0f, 0.0f };
// �Ǐ]�ʒu���璍���_�܂ł̑��΍��W
constexpr VECTOR LOCAL_F2T_POS = { 0.0f, 170.0f, 500.0f };
// �J������X��]����x�p
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
	// ��_�J������������Ԃɂ���
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
	//�������Ȃ�
}

void Camera::SetBeforeDrawFree(void)
{
	//���͏��
	auto& input = InputManager::GetInstance();

	// WASD�ŃJ�����̈ʒu��ς���
	float movePow = CAMERA_MOVE_POW;
	auto dir = Utility::VECTOR_ZERO;
	if (input.IsNew(KEY_INPUT_W)) { dir = rot_.GetForward(); }
	if (input.IsNew(KEY_INPUT_S)) { dir = rot_.GetBack(); }
	if (input.IsNew(KEY_INPUT_A)) { dir = rot_.GetLeft(); }
	if (input.IsNew(KEY_INPUT_D)) { dir = rot_.GetRight(); }
	if (input.IsNew(KEY_INPUT_Q)) { pos_.y += movePow; }
	if (input.IsNew(KEY_INPUT_E)) { pos_.y -= movePow; }
	pos_ = VAdd(pos_, VScale(dir, movePow));


	// �}�E�X�ŃJ�����̊p�x��ς���
	auto mouseMove = input.GetMouseMovePow();
	float speed = CAMERA_SPEED;
	//���E
	if (mouseMove.x > 0)
	{
		// �E��]
		angles_.y += Utility::Deg2RadF(speed);
	}
	if (mouseMove.x < 0)
	{
		// ����]
		angles_.y += Utility::Deg2RadF(-speed);
	}

	//�J�����̏㉺�����̑���
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


	// ���ʂ���ݒ肳�ꂽY�����A��]������
	rotOutX_ = Quaternion::AngleAxis(angles_.y, Utility::AXIS_Y);
	// ���ʂ���ݒ肳�ꂽX�����A��]������
	rot_ = rotOutX_.Mult(Quaternion::AngleAxis(angles_.x, Utility::AXIS_X));
	// �J�����̏����
	cameraUp_ = Utility::DIR_U;

}

void Camera::SetBeforeDrawFollow(void)
{
	//���͏��
	auto& input = InputManager::GetInstance();
	auto movePow = input.GetMouseMovePow();

	if (movePow.x != 0)
	{
		// ����]
		angles_.y += Utility::Deg2RadF(CAMERA_SPEED * movePow.x);
	}

	//�J�����̏㉺�����̑���
	// ���]
	if (movePow.y > 0)
	{
		angles_.x += Utility::Deg2RadF(CAMERA_SPEED * movePow.y);
		if (angles_.x > LIMIT_X_UP_RAD)
		{
			angles_.x = LIMIT_X_UP_RAD;
		}
	}
	// ����]
	if (movePow.y < 0)
	{
		angles_.x += Utility::Deg2RadF(-CAMERA_SPEED *abs(movePow.y));
		if (angles_.x < -LIMIT_X_DW_RAD)
		{
			angles_.x = -LIMIT_X_DW_RAD;
		}
	}

	//�^�[�Q�b�g�Ƃ̓���
	SyncFollow();
}

void Camera::SetBeforeDrawGameOverAuto(void)
{
	//���ʕ⊮
	rotateTime_ += SceneManager::GetInstance().GetDeltaTime();
	float par = rotateTime_ / 0.1f;
	rot_ = Quaternion::Slerp(rot_, goalQuaRot_, par);

	//�I�C���[�p�֕ϊ�
	angles_ = rot_.ToEuler();

	// ������̈ʒu
	VECTOR pos = followTransform_->pos_;
	VECTOR localPos;
	// �����_(�ʏ�d�͂ł����Ƃ����Y�l��Ǐ]�ΏۂƓ����ɂ���)
	localPos = rot_.PosAxis(LOCAL_F2T_POS);
	targetPos_ = VAdd(pos, localPos);

	// �J�����̏����
	cameraUp_ = Utility::DIR_U;

}

void Camera::SetBeforeDrawRotationTitle(void)
{
	float rotateAngleDeg = CAMERA_ROTATE_ANGLE_DEG;
	camearAxisRot_ += Utility::Deg2RadF(rotateAngleDeg);
	
	//��]���x
	float rotateSpeed = CAMERA_ROTATE_SPEED;

	pos_.x = targetPos_.x + rotateSpeed * cosf(camearAxisRot_);
	pos_.z = targetPos_.z + rotateSpeed * sinf(camearAxisRot_);
	
	//�p�x
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
		L"�J�������W�F(% .2f, % .2f, % .2f)", pos_.x, pos_.y, pos_.z);
	DrawFormatString(10, 520, 0xffffff, L"�J�����p�x�F(%.2f, %.2f, %.2f)",
		angles_.x * 180.0f / DX_PI_F,
		angles_.y * 180.0f / DX_PI_F,
		angles_.z * 180.0f / DX_PI_F);

#endif // DEBUG
}

void Camera::SetBeforeDraw(void)
{
	// �N���b�v�����̐ݒ�(SetDrawScreen�Ń��Z�b�g�����)
	SetCameraNearFar(CAMERA_NEAR, CAMERA_FAR);

	if (mode_ == MODE::ROTATION) {
		// �J�����̐ݒ�(�ʒu�A�����_�A�����)
		SetCameraPositionAndTargetAndUpVec(pos_, targetPos_, cameraUp_);
	}
	else
	{
		// �J�����̐ݒ�(�ʒu�Ɗp�x�ɂ�鐧��)
		SetCameraPositionAndAngle(
			pos_,
			angles_.x,
			angles_.y,
			angles_.z);
	}

	//DXLib ��Effekseer�̃J�����������s��
	Effekseer_Sync3DSetting();

}

void Camera::ChangeMode(MODE mode)
{
	// �J�������[�h�̕ύX
	mode_ = mode;
	// �ύX���̏���������
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

	//�S�[���p�x
	VECTOR dir = VNorm(VSub(targetPos, pos_));
	goalQuaRot_ = Quaternion::LookRotation(dir);
	goalDir_ = dir;
}

void Camera::SetDefault(void)
{
	//�Ǐ]�Ώۂ̃��Z�b�g
	followTransform_ = nullptr;

	// �J�����̏����ݒ�
	pos_ = DEFAULT_CAMERA_POS;

	// �����_
	targetPos_ = Utility::VECTOR_ZERO;

	// �J�����̏����
	cameraUp_ = Utility::DIR_U;

	//�J�����p�x
	angles_.x = Utility::Deg2RadF(0.0f);
	angles_.y = Utility::Deg2RadF(0.0f);
	angles_.z = 0.0f;

	rot_ = Quaternion();
}

void Camera::SyncFollow(void)
{
	// ������̈ʒu
	VECTOR pos = followTransform_->pos_;

	// ���ʂ���ݒ肳�ꂽY�����A��]������
	rotOutX_ = Quaternion::AngleAxis(angles_.y, Utility::AXIS_Y);

	// ���ʂ���ݒ肳�ꂽX�����A��]������
	rot_ = rotOutX_.Mult(Quaternion::AngleAxis(angles_.x, Utility::AXIS_X));
	
	VECTOR localPos;
	// �����_(�ʏ�d�͂ł����Ƃ����Y�l��Ǐ]�ΏۂƓ����ɂ���)
	localPos = rot_.PosAxis(LOCAL_F2T_POS);
	targetPos_ = VAdd(pos, localPos);

	// �J�����ʒu
	localPos = LOCAL_F2C_POS;
	pos_ = VAdd(pos, localPos);

	// �J�����̏����
	cameraUp_ = Utility::DIR_U;
}
