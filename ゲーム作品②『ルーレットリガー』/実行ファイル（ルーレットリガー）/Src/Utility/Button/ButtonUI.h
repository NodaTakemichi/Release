#pragma once
#include"../../Common/Vector2.h"
class ButtonUI
{
public:
	ButtonUI();
	virtual ~ButtonUI();

	virtual void Init(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	virtual void Release(void) = 0;



	//�{�^���̏�Ƀ}�E�X������Ă��邩�ǂ���
	virtual bool MouseOnButton(void) = 0;
	//�N���b�N����(�������u��)
	bool PushButton(void);
	//����i�{�^�����痣�����Ƃ��j
	bool ButtonDecision(void);


protected:
	//�{�^���t���[���̐F�̔Z���i�ʏ�j
	static constexpr float FRAME_COLOR_DEPTH_NORMAL = 1.0f;
	//�{�^���t���[���̐F�̔Z���i�N���b�N���j
	static constexpr float FRAME_COLOR_DEPTH_CLICK = 2.0f;

	//���W
	Vector2 pos_;

	//�w�ʉ摜
	int backImg_;
	//��O�摜
	int frontImg_;
	
	
	//�{�^���t���[���̔Z��
	float frameColorDepth_;

	//�{�^���̏�Ƀ}�E�X������Ă��邩�ǂ���
	bool onBtnBefor_;
	bool onBtn_;

	//�{�^������SE
	int clickSE_;
	//�I���{�^��SE
	int onBtnSE_;

	//�I���{�^��SE�Đ�
	void PlayOnBtnSE(void);
	//����{�^��SE�Đ�
	void PlayDecSE(void);

private:

};

