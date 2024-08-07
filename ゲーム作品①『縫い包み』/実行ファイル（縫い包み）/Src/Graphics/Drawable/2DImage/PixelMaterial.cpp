#include<cassert>
#include "../../../Utility/StringUtility.h"
#include "../../../Application.h"
#include "../../../Manager/ResourceManager.h"
#include "PixelMaterial.h"

using ResourceName = ResourceManager::SRC;
//ディゾルブシェーダー
constexpr int DISSOLVE_PS_CONST_BUF_NUM = 1;			//PSの定数バッファ数
//焼却ゲージシェーダー
constexpr int BURN_GAUGE_PS_CONST_BUF_NUM = 1;			//PSの定数バッファ数

PixelMaterial::PixelMaterial(void)
{
}

PixelMaterial::~PixelMaterial(void)
{
}

void PixelMaterial::DataLoad(const PS_TYPE& psType)
{
	auto& res = ResourceManager::GetInstance();
	//シェーダーデータ
	ShaderData shaderData = {};
	//ピクセルシェーダー別のデータ登録
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
	// ピクセルシェーダー用の定数バッファを作成
	psData_.constBufHandle = CreateShaderConstantBuffer(sizeof(FLOAT4) * psData_.constBufFloat4Size);

	//テクスチャアドレス
	texAddress_ = TEXADDRESS::CLAMP;
}
