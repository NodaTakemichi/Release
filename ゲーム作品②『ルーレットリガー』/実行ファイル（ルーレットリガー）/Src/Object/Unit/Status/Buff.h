#pragma once
#include<vector>
//#include<array>
class Buff
{
public:
	//�o�t�̃^�C�v
	enum class BUFF_TYPE
	{
		P_DOWN,
		P_UP,
		AVOIDANCE,
		D_DOWN,
		D_UP,
		S_DOWN,
		S_UP,
		POISON,
		CONFUSION,
		PALALYSIS,
		MAX
	};


	Buff();
	~Buff();

	void Init(void);
	void Release(void);

	//�o�t�̐����֐�
	void CreateBuff(const BUFF_TYPE& buff);

	//��������
	const bool& IsAlive(void)const { return isAlive_; }

	//�o�t�̎擾�֐�
	const BUFF_TYPE& GetBuff(void) const{ return buff_; }

	//�o�t�̏��L�`�F�b�N
	bool CheckOwnBuff(const BUFF_TYPE& type);

	//�o�t�̎����^�[���v�Z
	bool DecBuffTurn(void);

	void SetAlive(bool alive) { isAlive_ = alive; }	//������Ԃ��Z�b�g����


private:
	//���L����
	bool isAlive_;
	//�����^�[����
	int turn_;

	//���L�o�t
	BUFF_TYPE buff_;


	//�o�t�̕t�^
	void CheckBuffTurn(const BUFF_TYPE& type);

};

