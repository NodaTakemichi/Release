#include<cassert>
#include "../../../Utility/StringUtility.h"
#include "../../../Application.h"
#include "../../../Manager/ResourceManager.h"
#include "PixelMaterial.h"

using ResourceName = ResourceManager::SRC;
//�f�B�]���u�V�F�[�_�[
constexpr int DISSOLVE_PS_CONST_BUF_NUM = 1;			//PS�̒萔�o�b�t�@��
//�ċp�Q�[�W�V�F�[�_�[
constexpr int BURN_GAUGE_PS_CONST_BUF_NUM = 1;			//PS�̒萔�o�b�t�@��

PixelMaterial::PixelMaterial(void)
{
}

PixelMaterial::~PixelMaterial(void)
{
}

void PixelMaterial::DataLoad(const PS_TYPE& psType)
{
	auto& res = ResourceManager::GetInstance();
	//�V�F�[�_�[�f�[�^
	ShaderData shaderData = {};
	//�s�N�Z���V�F�[�_�[�ʂ̃f�[�^�o�^
	switch (psType)
	{
	case PS_TYPE::BURN_GAUGE:
		shaderData = SetShaderData(
			res.Load(ResourceName::BURN_GAUGE_PS).handleId_,
			BURN_GAUGE_PS_CONST_BUF_NUM);
		break;
	case PS_TYPE::DISTORTION:
		shaderData = SetShaderData(
			res.Load(ResourceName::DISSOLVE_PS).handleId_,
			DISSOLVE_PS_CONST_BUF_NUM);
		break;
	}

	psData_ = shaderData;
	
}

void PixelMaterial::Init(void)
{	
	// �s�N�Z���V�F�[�_�[�p�̒萔�o�b�t�@���쐬
	psData_.constBufHandle = CreateShaderConstantBuffer(sizeof(FLOAT4) * psData_.constBufFloat4Size);

	//�e�N�X�`���A�h���X
	texAddress_ = TEXADDRESS::CLAMP;
}
