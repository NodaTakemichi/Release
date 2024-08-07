#pragma once
#include<string>
#include<vector>
#include"../../../Common/Vector2.h"
#include "../../../Utility/Utility.h"
#include "../Status/Buff.h"
#include"../UnitBase.h"

class UnitUI
{
public:

	UnitUI(Vector2 pos, std::string& name,int& hp,int& nowHp, int& maxHp, bool& active);
	virtual ~UnitUI();


	virtual void Init(void);
	virtual void Draw(void);
	void Release(void);


	//���b�N�I��
	virtual void DrawRockOn(void) = 0;
	//�o�t���Z�b�g����
	void SetBuff(std::vector<Buff*> buffs);


	//�_���[�W�\�L
	void SetDmgValue(const bool& drawing, const int& dmg);
	//�񕜗ʕ\�L
	void SetHealValue(const bool& drawing, const int& heal);

private:

protected:

	//���j�b�g���W
	Vector2 unitPos_;
	//���j�b�g�l�[��
	std::string& name_;
	//�ő�HP,������HP
	int& hp_;
	int& maxHp_;
	int& nowHp_;
	bool& active_;

	//�l�[���t���[���摜
	int nameFrameImg_;
	//�^�[�Q�b�g�摜
	int targetImg_;

	//�_���[�W�t���[���摜
	int dmgFrameImg_;
	//�_���[�W�`�攻��
	bool dmgNumDrawing_;	//�_���[�W�� true:�\�� , false:��\��
	//�񕜒l�t���[���摜
	int healFrameImg_;
	//�񕜒l�`�攻��
	bool healNumDrawing_;	//�_���[�W�� true:�\�� , false:��\��
	//�_���[�W�l
	int dmg_;

	//�_���[�W�t�H���g
	int dmgFontHandle_;
	//�񕜃t�H���g
	int healFontHandle_;
	//���j�b�g�t�H���g
	int unitFontHandle_;

	//���v����
	float totalTime_;

	//�摜�A�C�R��
	int buffIconHandles_[static_cast<int>(Buff::BUFF_TYPE::MAX)];
	//���L�o�t�̎擾
	std::vector<Buff*> buffs_;

	//HP�̃V�F�[�_�[�`��
	void DrawHpShader(const Vector2& pos ,const COLOR_F& color);
	//HP�g�̕`��
	void DrawHpFrame(const Vector2& pos);
	//���O�̕`��
	void DrawName(const std::string& name,const Vector2& uPos,const COLOR_F& color);
	//�o�t�A�C�R���̕`��
	void DrawBuffIcon();
	



};

