#pragma once
#include <DxLib.h>
#include <memory>
#include "../../Common/Quaternion.h"
#include "Collider.h"

class Transform
{

public:

	// �R���X�g���N�^
	Transform(void);
	Transform(int model);

	// �f�X�g���N�^
	~Transform(void);

	// ���f���̃n���h��ID
	int modelId_;

	// �傫��
	VECTOR scl_;
	// ��]
	VECTOR rot_;
	// �ʒu
	VECTOR pos_;
	VECTOR localPos_;

	MATRIX matScl_;
	MATRIX matRot_;
	MATRIX matPos_;

	// ��]
	Quaternion quaRot_;

	// ���[�J����]
	Quaternion quaRotLocal_;

	// �R���C�_
	std::shared_ptr<Collider> collider_;

	/// <summary>
	/// 3D���f�����̏�����
	/// </summary>
	/// <param name="scl">�傫��</param>
	/// <param name="pos">���W</param>
	/// <param name="quaRot">��]</param>
	/// <param name="quaRotLocal">���[�J����]</param>
	/// <param name="collType">�R���C�_�[�^�C�v</param>
	void Init(VECTOR scl, VECTOR pos, Quaternion quaRot, Quaternion quaRotLocal, Collider::TYPE collType);

	/// <summary>
	/// ���f������̊�{���X�V
	/// </summary>
	void Update(void);
	
	void Release(void);

	void SetModel(int modelHId);
	void MakeCollider(Collider::TYPE type);

	// �O���������擾
	VECTOR GetForward(void) const;

	// ����������擾
	VECTOR GetBack(void) const;

	// �E�������擾
	VECTOR GetRight(void) const;

	// ���������擾
	VECTOR GetLeft(void) const;

	// ��������擾
	VECTOR GetUp(void) const;

	// ���������擾
	VECTOR GetDown(void) const;

	// �Ώە������擾
	VECTOR GetDir(const VECTOR& vec) const;

	// ���W���擾
	VECTOR GetPos(void) const;

};
