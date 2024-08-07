#include <cassert>
#include "../../../Application.h"
#include "../../../Utility/StringUtility.h"
#include "../../../Manager/ResourceManager.h"
#include "ModelMaterial.h"

using ResourceName = ResourceManager::SRC;

//3Dモデルシェーダー:VS
constexpr int NORML_MODEL_VS_CONST_BUF_SIZE = 2;		
//歪みシェーダー:PS
constexpr int DISTORTION_PS_CONST_BUF_NUM = 1;		
//発行シェーダー:PS
constexpr int LUMI_MODEL_PS_CONST_BUF_NUM = 2;		
//スフィアマップシェーダー:PS
constexpr int SPHERE_PS_CONST_BUF_NUM = 5;	
//焼却シェーダー:PS
constexpr int BURN_DOOL_PS_CONST_BUF_NUM = 1;		

ModelMaterial::ModelMaterial(void)
{
}

ModelMaterial::~ModelMaterial(void)
{
}

void ModelMaterial::DataLoad(const VS_TYPE& vsType, const PS_TYPE& psType)
{
	auto& res = ResourceManager::GetInstance();

	//シェーダーデータ
	ShaderData shaderData = {};

	//頂点シェーダー別のデータ登録
	switch (vsType)
	{
	case VS_TYPE::MODEL_NORMAL_1FRAME:
		shaderData = SetShaderData(
			res.Load(ResourceName::NORMAL_MODEL_VS).handleId_,
			NORML_MODEL_VS_CONST_BUF_SIZE);
		break;
	}
	vsData_ = shaderData;

	//ピクセルシェーダー別のデータ登録
	switch (psType)
	{
	case PS_TYPE::MODEL_NORMAL:
		shaderData = SetShaderData(
			res.Load(ResourceName::NORMAL_MODEL_PS).handleId_,
			0);
		break;
	case PS_TYPE::BURNING_DOOL:
		shaderData = SetShaderData(
			res.Load(ResourceName::BURNING_DOOL_PS).handleId_,
			BURN_DOOL_PS_CONST_BUF_NUM);
		break;
	case PS_TYPE::DISTORTION_WALL:
		shaderData = SetShaderData(
			res.Load(ResourceName::DISTORTION_WALL_PS).handleId_,
			DISTORTION_PS_CONST_BUF_NUM);
		break;
	case PS_TYPE::SPHERE_MAP:
		shaderData = SetShaderData(
			res.Load(ResourceName::SPHERE_MAP_PS).handleId_,
			SPHERE_PS_CONST_BUF_NUM);
		break;
	case PS_TYPE::LUMINESCENCE:
		shaderData = SetShaderData(
			res.Load(ResourceName::LUMINESCENCE_PS).handleId_,
			LUMI_MODEL_PS_CONST_BUF_NUM);
		break;
	}
	psData_ = shaderData;
}

void ModelMaterial::Init(void)
{
	//定数バッファハンドルを作成
	vsData_.constBufHandle = CreateShaderConstantBuffer(sizeof(FLOAT4) * vsData_.constBufFloat4Size);
	psData_.constBufHandle = CreateShaderConstantBuffer(sizeof(FLOAT4) * psData_.constBufFloat4Size);
	
	//テクスチャアドレス
	texAddress_ = TEXADDRESS::CLAMP;
}

void ModelMaterial::SetTextures(int slot, int texDiffuse)
{
	if (texture_.count(slot) == 0)
	{
		texture_.emplace(slot, texDiffuse);
	}
	else
	{
		texture_[slot] = texDiffuse;
	}
}

void ModelMaterial::AddConstBufVS(const FLOAT4& constBuf)
{
	if (vsData_.constBufFloat4Size > vsData_.constBufs.size())
	{
		vsData_.constBufs.emplace_back(constBuf);
	}
}

void ModelMaterial::AddConstBufPS(const FLOAT4& constBuf)
{
	if (psData_.constBufFloat4Size > psData_.constBufs.size())
	{
		psData_.constBufs.emplace_back(constBuf);
	}
}

void ModelMaterial::SetConstBufVS(int idx, const FLOAT4& constBuf)
{
	if (idx >= vsData_.constBufs.size())return;

	vsData_.constBufs[idx] = constBuf;
}

void ModelMaterial::SetConstBufPS(int idx, const FLOAT4& constBuf)
{
	if (idx >= psData_.constBufs.size())return;

	psData_.constBufs[idx] = constBuf;
}

