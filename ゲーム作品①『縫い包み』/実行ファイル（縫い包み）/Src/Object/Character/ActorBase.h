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


	//�Փ˔���ɗp������X�e�[�W�R���C�_����
	void AddStageCollider(const Collider& collider);
	void ClearStageCollider(void);

	//�Փ˗p�J�v�Z���̎擾
	const Capsule& GetCapsule(void)const;

	//�L�����N�^�[���a�̎擾
	float GetRadious(void);

	//�Q�[���I�[�o�[�̐ݒ�
	virtual void SetGameOverInfo(const VECTOR& opponentPos) = 0;

protected:
	float speed_;       //�ړ��X�s�[�h
	VECTOR moveDir_;    //�ړ�����
	VECTOR movePow_;    //�ړ���
	VECTOR movedPos_;   //�ړ���̍��W
	VECTOR beforePos_;   //�ړ��O�̍��W
	Quaternion actorRotY_; //��]

	Quaternion goalQuaRot_; //�S�[���p�x
	float rotateTime_ = 0.0f;	//��]����

	//��~��Ԃ��ǂ���
	bool isStoping_;

	//�n�ʂƂ̏Փ˃`�F�b�N�p
	VECTOR gravHitPosDown_;
	VECTOR gravHitPosUp_;

	//���g�̃J�v�Z���R���C�_
	std::unique_ptr<Capsule>capsule_;
	//�Փ˔���p�̃R���C�_
	std::vector<std::shared_ptr<Collider>>stageColliders_;

	//Updata�֐��Ŏ��s�����֐����w�������|�C���^
	std::function<void(void)> updateFunc_;

	//�Փ˔���
	void Collision(void);
	void CollisionCapsule(void);
	void CollisionGravity(void);

	//Line�ƃX�e�[�W�R���C�_�[�̏Փ˔��� //true:�Փ˂��Ă���Afalse:�Փ˂��Ă��Ȃ�
	bool CollCheckLine_AllStage(const VECTOR& front, const VECTOR& back);
	bool CollCheckLine_Stage(const VECTOR& front, const VECTOR& back,const Collider::TYPE& type);

	//�ړ��ʂ̌v�Z
	void CalcGravityPow(void);
	//�ړ���p�x�����߂�
	Quaternion CalcQuaRad(const VECTOR& pos1, const VECTOR& pos2);

	//�X�V����
	virtual void UpdateGameOver(void) = 0;      //�Q�[���I�[�o�[��Ԃ̍X�V

private:

};

