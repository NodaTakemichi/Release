#pragma once
#include"StageBase.h"
class ModelMaterial;
class ModelRenderer;
class Player;
class SoundController;

class Stage: public StageBase
{
public:
	// �g�F�̍��W
	static constexpr VECTOR FIREPLACE_POS = { 260.0f,-140.0f,-688.0f };
	// �g�F�̃��C�g���W
	static constexpr VECTOR FIREPLACE_LIGHT_POS = { FIREPLACE_POS.x - 40.0f,FIREPLACE_POS.y,FIREPLACE_POS.z };
	// �g�F�̍��W(�v���C���[���ł̏Փ˔���p�̂��� static)
	static constexpr VECTOR FIREPLACE_HIT_POS = { FIREPLACE_POS.x - 50.0f,FIREPLACE_POS.y + 50.0f,FIREPLACE_POS.z };

	Stage(const Player& player);
	virtual ~Stage(void);

	void LoadData(void)override;
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;

	//�X�e�[�W�R���W�����̎擾
	const Transform& GetCollStageTransform(void) const;


private:
	//�v���C���[���
	const Player& player_;

	//�Փ˔���p���f��
	Transform collTransform_;

	//���^���b�N�I�u�W�F�N�g���f��
	Transform metaTransform_;

	//�g�F�̉��G�t�F�N�g�n���h��
	int fireHandle_;
	//�g�F�̉��T�E���h
	std::shared_ptr< SoundController>fireSound_;
	//���C�g�n���h��
	int fireLightHandle_;


	//�ʏ�`��}�e���A��
	std::unique_ptr<ModelMaterial>normalModelMaterial_;
	std::unique_ptr<ModelRenderer>normalModelRenderer_;
	//�����`��}�e���A��
	std::unique_ptr<ModelMaterial>lumiModelMaterial_;
	std::unique_ptr<ModelRenderer>lumiModelRenderer_;

	//�X�t�B�A�}�b�v�}�e���A��
	std::unique_ptr<ModelMaterial>sphereMaterial_;
	std::unique_ptr<ModelRenderer>sphereRenderer_;


	//�����p�o�ߎ���
	float lumiProgressTime_;
	//�����`�撆�����f
	bool isLumiDrawing_;
	//�아�������W
	VECTOR amuletActionPos_;

};

