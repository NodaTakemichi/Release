#pragma once
class Collider
{

public :

	// �Փˎ��
	enum class TYPE
	{
		NONE,
		STAGE,
		GOAL_DOOR,
		DIS_WALL,
		ITEM,
		FACILITY,
		ENEMY
	}; 

	// �R���X�g���N�^
	Collider(TYPE type, int modelId);

	// �f�X�g���N�^
	~Collider(void);

	// �Փˎ��
	TYPE type_;

	// ���f���̃n���h��ID
	int modelId_;

};
