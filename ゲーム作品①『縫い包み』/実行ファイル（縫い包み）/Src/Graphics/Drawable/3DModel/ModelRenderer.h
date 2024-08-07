#pragma once
#include<DxLib.h>
class ModelMaterial;


class ModelRenderer
{
public:
	//�R���X�g���N�^
	ModelRenderer(int& modelId,ModelMaterial& postEffectMaterial);
	~ModelRenderer(void);

	//�`��
	void Draw(void);


private:
	// ���_�V�F�[�_�p�I���W�i���萔�o�b�t�@�̎g�p�J�n�X���b�g
	static constexpr int CONSTANT_BUF_SLOT_BEGIN_VS = 7;
	// �s�N�Z���V�F�[�_�p�I���W�i���萔�o�b�t�@�̎g�p�J�n�X���b�g
	static constexpr int CONSTANT_BUF_SLOT_BEGIN_PS = 4;


	//���f���n���h��
	int& modelId_;

	//���f���}�e���A��
	ModelMaterial& modelMaterial_;

	//�V�F�[�_�[�ݒ�i���_�j
	void SetReserveVS(void);

	//�V�F�[�_�[�ݒ�i�s�N�Z���j
	void SetReservePS(void);


};

