#pragma once
#include "SceneBase.h"
class Button;
class ModelMaterial;
class ModelRenderer;

class ResultScene :
    public SceneBase
{
public:

	ResultScene(SceneManager& manager);
	~ResultScene(void);


	virtual void LoadData(void)override;
	virtual void Init(void) override;
	virtual void Update(InputManager& input) override;
	virtual void Draw(void) override;

private:

	//�e�{�^������
	void TitleButtnProcess(void);	//�^�C�g���J�ڃ{�^��
	void ExitButtnProcess(void);	//�I���{�^��

	//Update�֐�
	void LoadingUpdate(InputManager& input);
	void NormalUpdate(InputManager& input);
	//�`��֐�
	void LoadingDraw(void);
	void NormalDraw(void);

	//�I���{�^��
	std::vector<Button>buttons_;

	//���U���g�摜
	int resultImg_;

	//BGM
	std::shared_ptr<SoundController>bgm_;

	//���C�g�n���h��
	int lightHandle_;
	//�X�e�[�W�p�X�N���[��
	int stageScreen_;

	//�X�e�[�W���f��
	int stageModelHandle_;
	std::unique_ptr<ModelMaterial>stageModelMaterial_;
	//���f�������_���[
	std::vector<std::unique_ptr<ModelRenderer>>modelRenderers_;
	//���f���쐬
	void SettingObject(void);


};

