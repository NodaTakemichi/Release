#pragma once
#include<vector>
#include<array>
#include<unordered_map>
#include "../Common/Vector2.h"
#include "../Common/Deck.h"
#include "SceneBase.h"

class RectButton;
class CircleButton;

struct ModeData {
	int modeImg;		//���[�h�I���摜
	RectButton* btn;	//���[�h�{�^��
};

struct StageData {
	int backImg;				//�w�i�摜
	std::vector<Deck>enemys;	//�G�l�~�[�Y
};

class SelectScene :
    public SceneBase
{
public:

	enum class SELECT_MODE
	{
		BATTLE	  = 0,
		DECK_EDIT = 1,
		RULE_BOOK = 2,
		CREDIT	  = 3,
		TITLE	  = 4,
		MAX		  
	};

	enum class SELECT_STAGE
	{
		VOLCANO = 0,
		OCEAN	= 1,
		WILDRNESS	= 2,
		FOREST		= 3,
		SANCTUARY	= 4,
		ABYSS		= 5,
		MAX
	};

	enum class SELECT_PAGE
	{
		MODE = 0,
		STAGE	= 1,
		ENEMY	= 2,
		MAX
	};

	// �R���X�g���N�^
	SelectScene(void);

	// �f�X�g���N�^
	~SelectScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:
	//�o�b�N�{�^��
	CircleButton* backBtn_;
	//�{�^���摜
	int backImg_;

	//����{�^��
	RectButton* deciBtn_;
	//�{�^���摜
	int deciBtnImg_;

	//�I����ʂ̐i�s
	SELECT_PAGE page_;

	//�o�g��������
	std::vector<Deck>enemys_;
	//���G�l�~�[�Y�ԍ�
	int nowEnemysNum_;
	//�X�e�[�W
	std::unordered_map<SELECT_STAGE, StageData>stages_;
	//���݃X�e�[�W�ԍ�
	int nowStageNum_;


	//�I���{�b�N�X�摜
	int selectedBoxImg_;
	//�I���{�b�N�X�̃}�X�N�摜
	int selectedBoxMaskImg_;
	//�I��p�}�X�N�X�N���[��
	int selectMaskScreen_;
	//�I���摜���W
	Vector2 selectImgPos_;

	//�X�N���[���o�ߎ���
	float scrollTime_;
	//�z�C�[�����씻�f�i�X�N���[�����͔������Ȃ��j
	bool isMoveWheel_;
	//�z�C�[���̃X�N���[������
	int wheelSign_;



	//���݃��[�h
	SELECT_MODE mode_;
	//���[�h�{�^��
	std::map<SELECT_MODE, ModeData>modeBtn_;
	//���[�h�{�^���摜
	int modeBackBtnImg_;

	//���[�h�{�b�N�X�摜
	Vector2 modePos_;
	std::string nowMode_;
	int modeBox_;
	int modeFontHandle_;

	//�R�����g�{�b�N�X
	Vector2 cmtPos_;
	std::string devilCmt_;
	int cmtBox_;
	int cmtFontHandle_;
	//�}�X�R�b�g�摜
	int devilImg_;
	Vector2 devilPos_;
	int shakeY_;

	//�����֘A
	int scrollSE_;		//�X�N���[��SE


	//���[�h�{�b�N�X�`��
	void DrawModeBox(void);
	//�R�����g�{�b�N�X�`��
	void DrawCmtBox(void);


	//�{�^������
	void BtnProcess();
	//�e�{�^������
	void BattleBtnProcess(void);
	void EditBtnProcess(void);
	void TitleBtnProcess(void);

	//���[�h�{�^������
	void CreateModeBtn(void);
	//���[�h�̕ύX
	void ChangeSelectMode(const SELECT_MODE& mode);

	//�N���b�N�����{�^���̏���
	void SelectBtnProcess(void);

	//�}�X�R�b�g�̃R�����g�Z�b�g
	void SetDevilCmt(const std::string& cmt);

	//�X�e�[�W���̓o�^
	void StageDataInit();

	//�I���X�N���[������
	void SelectSceollProcess(void);
	//�I���摜�X���C�h�ړ�
	void ScrollSelectImg(int& num, const int& size);

	//�I���X�e�[�W�̕`��i�I���{�b�N�X�j
	void DrawSelectBoxStage(void);
	//�I���G�l�~�[�̕`��i�I���{�b�N�X�j
	void DrawSelectBoxEnemy(void);

};

