#pragma once
#include<vector>
#include<string>
class Command;

class Roulette
{
public:

	Roulette();
	~Roulette();

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	//���[���b�g���~�߂�w���o���֐�
	void StopRoulette(const bool& autom);		//true�F�����Afalse�F�蓮

	//���[���b�g�ɃR�}���h�Z���Z�b�g����
	void SetCommand(std::vector<Command*> cmd);

	//���[���b�g�̉�]�󋵂����Z�b�g����
	void ResetRouSpin(void);

	//���[���b�g�̒�~��Ԃ̎擾�֐�
	bool GetRouStop(void) { return isStop_; }

	//���肵���R�}���h�̎擾
	Command* GetCmd(void);

private:

	
	int rouletteImg_;	//���[���b�g�摜
	int rouFrame_;		//�g
	int center_;		//����
	//���摜
	int arrowImg_;

	//��]�p�x
	double angle_;
	//��]��
	double rotPower_;

	//���[���b�g�̉�]��
	bool isRouSpin_;
	//���[���b�g����~��Ԃ����f
	bool isStop_;

	//�t�H���g�n���h��
	int fontHandle_;

	//�R�}���h�Z
	std::vector<Command*>cmdNames_;

	//�ҋ@����
	float totalTime_;

	//SE�֘A
	int rotateSE_;
	int stopingSE_;
	int stopBtnSE_;

	//���[���b�g�̉�]����
	void RotateProcess(void);


};

