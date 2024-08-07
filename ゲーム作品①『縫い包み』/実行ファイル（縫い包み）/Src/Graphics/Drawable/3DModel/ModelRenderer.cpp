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
	//オリジナルシェーダーの設定（ON）
	MV1SetUseOrigShader(true);

	//シェーダー設定（頂点）
	SetReserveVS();

	//シェーダー設定（ピクセル）
	SetReservePS();

	// テクスチャアドレスタイプの取得
	auto texA = modelMaterial_.GetTextureAddress();
	int texAType = static_cast<int>(texA);

	//テクスチャアドレスタイプの変更
	SetTextureAddressModeUV(texAType, texAType);
	
	// 描画
	MV1DrawModel(modelId_);

	//オリジナルシェーダーの設定（OFF）
	MV1SetUseOrigShader(false);
}

void ModelRenderer::SetReserveVS(void)
{
	//シェーダーデータ：VS
	auto vsData = modelMaterial_.GetShaderDataVS();

	//定数バッファハンドル
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

	// 定数バッファを更新して書き込んだ内容を反映する
	UpdateShaderConstantBuffer(cb);
	// 定数バッファを頂点シェーダー用定数バッファレジスタにセット
	SetShaderConstantBuffer(
		cb, DX_SHADERTYPE_VERTEX, CONSTANT_BUF_SLOT_BEGIN_VS);

	// シェーダー設定
	SetUseVertexShader(vsData.handle);

}

void ModelRenderer::SetReservePS(void)
{
	//シェーダーデータ：PS
	auto psData = modelMaterial_.GetShaderDataPS();

	//ピクセルシェーダーにテクスチャを転送
	const auto& textures = modelMaterial_.GetTextures();
	size_t size = textures.size();
	if (size == 0)
	{
		//前回使用分のテクスチャを引き継がないように
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


	//定数バッファハンドル
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

	// 定数バッファを更新して書き込んだ内容を反映する
	UpdateShaderConstantBuffer(cb);
	// 定数バッファをピクセルシェーダー用定数バッファレジスタにセット
	SetShaderConstantBuffer(
		cb, DX_SHADERTYPE_PIXEL, CONSTANT_BUF_SLOT_BEGIN_PS);

	// シェーダー設定
	SetUsePixelShader(psData.handle);
}
