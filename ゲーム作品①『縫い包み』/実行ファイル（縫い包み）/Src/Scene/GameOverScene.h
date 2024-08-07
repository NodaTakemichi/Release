#pragma once
#include "SceneBase.h"
class PostEffectMaterial;
class PostEffectRenderer;
class Button;




class GameOverScene :
    public SceneBase
{
public:

	GameOverScene(SceneManager& manager);
	~GameOverScene(void);

	virtual void LoadData(void)override;
	virtual void Init(void) override;
	virtual void Update(InputManager& input) override;
	virtual void Draw(void) override;

private:
	void BeginUpdate(InputManager& input);	//GameOver�\���܂ł̍X�V
	void NormalUpdate(InputManager& input);	//�ʏ�X�V
	void EndUpdate(InputManager& input);	//GameOver�����܂ł̍X�V

	void BeginDraw(void);	//GameOver�\���܂ł̕`��
	void NormalDraw(void);	//�ʏ�`��
	void EndDraw(void);		//GameOver�����܂ł̕`��

	float frameTime_ = 0.0f;

	//�w�i�摜
	int bgImg_;

	//�m�C�Y
	std::shared_ptr<SoundController>noiseSE_;


	// �|�X�g�G�t�F�N�g
	std::unique_ptr<PostEffectMaterial> postEffectMaterial_;
	std::unique_ptr<PostEffectRenderer> postEffectRenderer_;

	//�I���{�^��
	std::vector<Button>buttons_;

	//�e�{�^������
	void RestartBtnProcess(void);
	void ExitBtnProcess(void);
	void TitleBtnProcess(void);
};

