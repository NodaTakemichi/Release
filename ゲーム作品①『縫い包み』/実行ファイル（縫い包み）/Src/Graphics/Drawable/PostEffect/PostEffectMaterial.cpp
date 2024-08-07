#include <cassert>
#include "../../../Application.h"
#include "../../../Manager/ResourceManager.h"
#include "PostEffectMaterial.h"

using ResourceName = ResourceManager::SRC;

//ノイズシェーダー
constexpr int NOISE_PS_CONST_BUF_NUM = 1;		
//グリッチシェーダー
constexpr int GLITCH_PS_CONST_BUF_NUM = 1;		

PostEffectMaterial::PostEffectMaterial(void)
{
	postEffectScreen_ = 0;
}

PostEffectMaterial::~PostEffectMaterial(void)
{
}

void PostEffectMaterial::DataLoad(const PS_TYPE& psType)
{
	auto& res = ResourceManager::GetInstance();
	//シェーダーデータ
	ShaderData shaderData = {};
	//ピクセルシェーダー別のデータ登録
	switch (psType)
	{
	case PS_TYPE::BLUR:
		shaderData = SetShaderData(
			res.Load(ResourceName::BLUR_PS).handleId_,
			0);
		break;
	case PS_TYPE::GLITCH:
		shaderData = SetShaderData(
			res.Load(ResourceName::GLITCH_PS).handleId_,
			GLITCH_PS_CONST_BUF_NUM);
		break;
	case PS_TYPE::NOISE:
		shaderData = SetShaderData(
			res.Load(ResourceName::NOISE_PS).handleId_,
			NOISE_PS_CONST_BUF_NUM);
		break;
	case PS_TYPE::VIGNETTE:
		shaderData = SetShaderData(
			res.Load(ResourceName::VIGNETTE_PS).handleId_,
			0);
		break;
	}

	psData_ = shaderData;

}

void PostEffectMaterial::Init(void)
{	
	// ピクセルシェーダー用の定数バッファを作成
	psData_.constBufHandle = CreateShaderConstantBuffer(sizeof(FLOAT4) * psData_.constBufFloat4Size);

	MakePostEffecctScreen();
}

void PostEffectMaterial::SetConstBuf(int idx, const FLOAT4& constBuf)
{
	if (idx >= psData_.constBufs.size())return;

	psData_.constBufs[idx] = constBuf;
}

void PostEffectMaterial::MakePostEffecctScreen(void)
{
	// ポストエフェクト用スクリーン
	postEffectScreen_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);
}
