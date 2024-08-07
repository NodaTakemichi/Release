#pragma once
#include <DxLib.h>
#include <memory>
#include "../Common/Quaternion.h"
class Transform;
class InputManager;

class Camera
{
public:
	//�J�������[�h
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


	// �J�����ʒu
	VECTOR GetPos(void) const;
	// �J�����̑���p�x
	VECTOR GetAngles(void) const;
	// �J�����̒����_
	VECTOR GetTargetPos(void) const;
	// �J�����p�x
	Quaternion GetQuaRot(void) const;
	// X��]�𔲂����J�����p�x
	Quaternion GetQuaRotOutX(void) const;
	// �J�����̑O������
	VECTOR GetForward(void) const;

	//�J�����̈ʒu�A�p�x�̕`��X�V
	void SetBeforeDraw(void);
	//�J�������[�h�̕ύX
	void ChangeMode(MODE mode);

	// �Ǐ]�Ώۂ̐ݒ�
	void SetFollow(const Transform* follow);
	// �����_�̐ݒ�
	void SetTargetPos(const VECTOR& targetPos);

	//�Q�[���I�[�o�[���̃J�����ݒ�
	void SetGameOverRot(const VECTOR& targetPos);

private:
	// �J�������Ǐ]�ΏۂƂ���Transform
	const Transform* followTransform_;

	
	VECTOR pos_;	// �J�����̈ʒu
	VECTOR angles_;	// �J�����̊p�x
	Quaternion rotOutX_;// X��]���Ȃ��p�x
	Quaternion rot_;	// �J�����p�x
	VECTOR targetPos_;	// �����_
	VECTOR cameraUp_;	// �J�����̏����

	//�J�������[�h
	MODE mode_;

	//��]����
	float rotateTime_;
	//�S�[���p�x
	Quaternion goalQuaRot_;
	Quaternion goalDir_;

	//�J��������]
	float camearAxisRot_;

	// �J�����������ʒu�ɖ߂�
	void SetDefault(void);
	// �J��������
	void SyncFollow(void);

	//���[�h�ʍX�V�X�e�b�v
	void SetBeforeDrawFixedPoint(void);
	void SetBeforeDrawFree(void);
	void SetBeforeDrawFollow(void);
	void SetBeforeDrawGameOverAuto(void);
	void SetBeforeDrawRotationTitle(void);

};

