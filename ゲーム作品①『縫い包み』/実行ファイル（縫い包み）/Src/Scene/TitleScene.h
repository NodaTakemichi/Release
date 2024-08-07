#pragma once
#include <string>
#include "SceneBase.h"
class ModelMaterial;
class ModelRenderer;
class PostEffectMaterial;
class PostEffectRenderer;
class Button;

//�C��
#include"../Graphics/Drawable/3DModel/ModelMaterial.h"
#include"../Graphics/Drawable/3DModel/ModelRenderer.h"
#include"../Graphics/Drawable/PostEffect/PostEffectMaterial.h"
#include"../Graphics/Drawable/PostEffect/PostEffectRenderer.h"



class TitleScene : public SceneBase
{
public:

	TitleScene(SceneManager& manager);
	~TitleScene(void);

	
	virtual void LoadData(void)override;
	virtual void Init(void) override;
	virtual void Update(InputManager& input) override;
	virtual void Draw(void) override;

private:

	//�^�C�g���摜
	int titleImg_;

	//���C�g�n���h��
	int lightHandle_;
	//�X�e�[�W�p�X�N���[��
	int stageScreen_;

	//���f���쐬
	void SettingObject(void);

	//�e�{�^������
	void StartButtnProcess(void);	//�X�^�[�g�{�^��
	void InstructButtnProcess(void);//�������{�^��
	void ExitButtnProcess(void);	//�I���{�^��

	//Update�֐�
	void LoadingUpdate(InputManager& input);
	void NormalUpdate(InputManager& input);
	//�`��֐�
	void LoadingDraw(void);
	void NormalDraw(void);

	//�I���{�^��
	std::vector<Button>buttons_;

	//�X�e�[�W���f��
	int stageModelHandle_;
	std::unique_ptr<ModelMaterial>stageModelMaterial_;
	//�ʂ�����݃��f��
	int doolModelHandle_;
	std::unique_ptr<ModelMaterial>doolModelMaterial_;
	//�ʂ�����ݍ��W
	VECTOR doolPos_;
	//���f�������_���[
	std::vector<std::unique_ptr<ModelRenderer>>modelRenderers_;


	//�|�X�g�G�t�F�N�g�i�}�e���A���j
	std::unique_ptr<PostEffectMaterial>glitchMaterial_;		//�O���b�`�V�F�[�_�[
	//�|�X�g�G�t�F�N�g�����_���[
	std::vector<std::unique_ptr<PostEffectRenderer>> postEffectRenderer_;

	//BGM
	std::shared_ptr<SoundController>bgm_;

};
