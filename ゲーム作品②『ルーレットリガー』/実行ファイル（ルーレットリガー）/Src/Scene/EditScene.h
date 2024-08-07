#pragma once
#include<vector>
#include<array>
#include<map>
#include<unordered_map>
#include "SceneBase.h"
#include "../Manager/DataManager/UnitDataManager.h"
#include "../Common/Vector2.h"

class CircleButton;
class RectButton;
class UnitButton;
class CmdButton;

class EditScene :
	public SceneBase
{
public:


	// �R���X�g���N�^
	EditScene(void);

	// �f�X�g���N�^
	~EditScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;


private:
	//�}�C�f�b�L�{�^��
	std::unordered_map<int, UnitButton*> myDeckBtn_;

	//�������j�b�g�J�[�h�{�^��
	std::unordered_map<int, UnitButton*> unitCards_;
	//�������j�b�g�ԍ�
	std::vector<int> unitCardNums_;

	//�s�b�N�A�b�v���j�b�g�f�[�^
	UnitData pickUpUnitData_;

	//�R�}���h�{�^��
	std::vector<CmdButton*> cmdBtns_;


	//�o�b�N�{�^��
	CircleButton* backBtn_;
	//�{�^���摜
	int backImg_;

	//�y�[�W�X�V�{�^���@�i�O�F�O�y�[�W�ցA�P�F���y�[�W�ցj
	std::array<RectButton*, 2> pageBtn_;
	//���F�O�p�摜
	int nextPB_;
	//�O�F�O�p�摜
	int backPB_;


	//���j�b�g�J�[�h�̔w�ʉ摜
	int unitBackImg_;

	//HP�A�U���́A�X�s�[�h�A�C�R��
	int hpIcon_;
	int attackIcon_;
	int speedIcon_;

	//�s�b�N�A�b�v���j�b�g
	int pickUpUnit_;
	//�X�e�[�^�X�t�H���g�n���h��
	int sFontHandle_;
	int sFontColor_;

	//�y�[�W��
	int page_;

	//�f�b�L���j�b�g�A�������j�b�g�̏�����
	void InitMyUnit(void);

	//�X�e�[�^�X��ʂ̃R�}���h����
	void CreateCmd(void);

	//�J�[�h�X�e�[�^�X�̕`��
	void DrawUnitStatus(void);

	//�f�b�L�ҏW
	void DeckEditProcess(void);

	//�s�b�N�A�b�v���j�b�g�̑I��
	void SelectPickUpUnit(void);

	//�f�b�L�̊m��
	void DeckDecision(void);

	//�y�[�W�{�^���X�V����
	void PageBtnProcess(void);

	//�f�b�L�v�[���̕\���y�[�W�̕ύX
	void ChangeDeckPoolPage(void);

};