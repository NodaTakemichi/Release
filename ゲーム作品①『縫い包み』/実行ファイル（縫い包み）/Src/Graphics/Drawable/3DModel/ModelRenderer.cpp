#include "ModelMaterial.h"
#include "ModelRenderer.h"

ModelRenderer::ModelRenderer(int& modelId, ModelMaterial& modelMaterial):
	modelId_(modelId),modelMaterial_(modelMaterial)
{
}

ModelRenderer::~ModelRenderer(void)
{
}

void ModelRenderer::Draw(void)
{
	//�I���W�i���V�F�[�_�[�̐ݒ�iON�j
	MV1SetUseOrigShader(true);

	//�V�F�[�_�[�ݒ�i���_�j
	SetReserveVS();

	//�V�F�[�_�[�ݒ�i�s�N�Z���j
	SetReservePS();

	// �e�N�X�`���A�h���X�^�C�v�̎擾
	auto texA = modelMaterial_.GetTextureAddress();
	int texAType = static_cast<int>(texA);

	//�e�N�X�`���A�h���X�^�C�v�̕ύX
	SetTextureAddressModeUV(texAType, texAType);
	
	// �`��
	MV1DrawModel(modelId_);

	//�I���W�i���V�F�[�_�[�̐ݒ�iOFF�j
	MV1SetUseOrigShader(false);
}

void ModelRenderer::SetReserveVS(void)
{
	//�V�F�[�_�[�f�[�^�FVS
	auto vsData = modelMaterial_.GetShaderDataVS();

	//�萔�o�b�t�@�n���h��
	auto cb = vsData.constBufHandle;

	FLOAT4* constBufsPtr = (FLOAT4*)GetBufferShaderConstantBuffer(cb);
	const auto& constBufs = vsData.constBufs;
	size_t size = constBufs.size();
	for (int i = 0; i < size; i++)
	{
		if (i != 0)
		{
			constBufsPtr++;
		}
		constBufsPtr->x = constBufs[i].x;
		constBufsPtr->y = constBufs[i].y;
		constBufsPtr->z = constBufs[i].z;
		constBufsPtr->w = constBufs[i].w;
	}

	// �萔�o�b�t�@���X�V���ď������񂾓��e�𔽉f����
	UpdateShaderConstantBuffer(cb);
	// �萔�o�b�t�@�𒸓_�V�F�[�_�[�p�萔�o�b�t�@���W�X�^�ɃZ�b�g
	SetShaderConstantBuffer(
		cb, DX_SHADERTYPE_VERTEX, CONSTANT_BUF_SLOT_BEGIN_VS);

	// �V�F�[�_�[�ݒ�
	SetUseVertexShader(vsData.handle);

}

void ModelRenderer::SetReservePS(void)
{
	//�V�F�[�_�[�f�[�^�FPS
	auto psData = modelMaterial_.GetShaderDataPS();

	//�s�N�Z���V�F�[�_�[�Ƀe�N�X�`����]��
	const auto& textures = modelMaterial_.GetTextures();
	size_t size = textures.size();
	if (size == 0)
	{
		//�O��g�p���̃e�N�X�`���������p���Ȃ��悤��
		SetUseTextureToShader(0, -1);
		SetUseTextureToShader(1, -1);
		//SetUseTextureToShader(2, -1);
		//SetUseTextureToShader(3, -1);
		//SetUseTextureToShader(4, -1);
	}
	else
	{
		for (const auto& pair : textures)
		{
			SetUseTextureToShader(pair.first, pair.second);
		}
	}


	//�萔�o�b�t�@�n���h��
	auto cb = psData.constBufHandle;

	FLOAT4* constBufsPtr = (FLOAT4*)GetBufferShaderConstantBuffer(cb);
	const auto& constBufs = psData.constBufs;
	size = constBufs.size();
	for (int i = 0;i<size;i++)
	{
		if (i != 0)
		{
			constBufsPtr++;
		}
		constBufsPtr->x = constBufs[i].x;
		constBufsPtr->y = constBufs[i].y;
		constBufsPtr->z = constBufs[i].z;
		constBufsPtr->w = constBufs[i].w;
	}

	// �萔�o�b�t�@���X�V���ď������񂾓��e�𔽉f����
	UpdateShaderConstantBuffer(cb);
	// �萔�o�b�t�@���s�N�Z���V�F�[�_�[�p�萔�o�b�t�@���W�X�^�ɃZ�b�g
	SetShaderConstantBuffer(
		cb, DX_SHADERTYPE_PIXEL, CONSTANT_BUF_SLOT_BEGIN_PS);

	// �V�F�[�_�[�ݒ�
	SetUsePixelShader(psData.handle);
}
