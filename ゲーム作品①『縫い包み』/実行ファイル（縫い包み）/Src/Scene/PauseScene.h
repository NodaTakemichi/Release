#pragma once
#include "SceneBase.h"


class Button;
class PauseScene :
    public SceneBase
{
public:
	PauseScene(SceneManager& manager);
	~PauseScene(void);

	virtual void LoadData(void)override;	
	virtual void Init(void) override;
	virtual void Update(InputManager& input) override;
	virtual void Draw(void) override;


private:


	void DefaultUpdate(InputManager& input);	//�������Ȃ�
	void BeginUpdate(InputManager& input);	//�|�[�Y�\���܂ł̍X�V
	void NormalUpdate(InputManager& input);	//�ʏ�X�V
	void EndUpdate(InputManager& input);	//�|�[�Y�����܂ł̍X�V

	void BeginDraw(void);	//�|�[�Y�\���܂ł̕`��
	void NormalDraw(void);	//�ʏ�`��
	void EndDraw(void);		//�|�[�Y�����܂ł̕`��

	float frameTime_ = 0.0f;

	//�|�[�Y�w�i�摜
	int pauseBackImg_;

	//�I���{�^��
	std::vector<Button>buttons_;

	//�e�{�^������
	void RuleBtnProcess(void);
	void TitleBtnProcess(void);
	void ExitBtnProcess(void);
	void CloseBtnProcess(void);

};

