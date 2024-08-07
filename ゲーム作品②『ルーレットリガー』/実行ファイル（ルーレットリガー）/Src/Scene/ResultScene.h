#pragma once
#include<vector>
#include "SceneBase.h"
class Vector2;
class UnitButton;

class ResultScene :
    public SceneBase
{
public:
	// �R���X�g���N�^
	ResultScene(void);

	// �f�X�g���N�^
	~ResultScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	//�t�H���g�n���h��
	int fontHandle_;

	//�g�����摜�A���W
	int devilImg_;
	Vector2 devilPos_;
	//�R�}���h�{�b�N�X�摜
	int cmtBoxImg_;

	//�������j�b�g�{�^��
	std::vector<UnitButton*>unitBtns_;

	//���j�b�g�J�[�h�w��
	int unitBackImg_;

	//�������j�b�g�̃{�^������
	void CreateUnitBtn(void);

	//�������j�b�g��`�悷��
	void DrawGetUnits(void);

};

