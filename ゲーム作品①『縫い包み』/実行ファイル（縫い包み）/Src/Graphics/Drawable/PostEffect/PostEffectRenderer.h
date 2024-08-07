#pragma once
#include<DxLib.h>
#include"../../../Common/Vector2.h"
class PostEffectMaterial;


class PostEffectRenderer
{
public:
	//�R���X�g���N�^
	PostEffectRenderer(PostEffectMaterial& postEffectMaterial);
	~PostEffectRenderer(void);

	//�`��
	void Draw(void);

	//�`���`�̐���
	void MakeSquereVertex(void);
	

private:
	// �s�N�Z���V�F�[�_�p�I���W�i���萔�o�b�t�@�̎g�p�J�n�X���b�g
	static constexpr int CONSTANT_BUF_SLOT_BEGIN_PS = 4;
	// ���_��
	static constexpr int NUM_VERTEX = 4;
	// ���_�C���f�b�N�X��
	static constexpr int NUM_VERTEX_IDX = 6;
	// �|���S����
	static constexpr int NUM_POLYGON = 2;


	//���_
	VERTEX2DSHADER vertexs_[NUM_VERTEX];

	//���_�C���f�b�N�X
	WORD indexes_[NUM_VERTEX_IDX];

	//�s�N�Z���}�e���A��
	PostEffectMaterial& postEffectMaterial_;

	//���C���X�N���[��
	int mainScreen_;

};

