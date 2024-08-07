#include "../../../Application.h"
#include "PixelMaterial.h"
#include "PixelRenderer.h"

PixelRenderer::PixelRenderer(PixelMaterial& pixelMaterial):
	pixelMaterial_(pixelMaterial)
{
	pos_ = Vector2();
	size_ = Vector2();

}

PixelRenderer::~PixelRenderer(void)
{
}

void PixelRenderer::Draw(void)
{
	//シェーダーデータ：PS
	auto psData = pixelMaterial_.GetShaderDataPS();

	// シェーダー設定
	SetUsePixelShader(psData.handle);

	//定数バッファ
	auto cb = psData.constBufHandle;

	// テクスチャの設定
	int index = 0;
	for (const auto& tex : pixelMaterial_.GetTextures())
	{
		SetUseTextureToShader(index, tex);
		index++;
	}

	// 定数バッファ
	FLOAT4* constBufsPtr =
		(FLOAT4*)GetBufferShaderConstantBuffer(cb);
	// 乗算色
	for (const auto& constBuf : psData.constBufs)
	{
		constBufsPtr->x = constBuf.x;
		constBufsPtr->y = constBuf.y;
		constBufsPtr->z = constBuf.z;
		constBufsPtr->w = constBuf.w;
		constBufsPtr++;
	}

	// 定数バッファを更新して書き込んだ内容を反映する
	UpdateShaderConstantBuffer(cb);
	// 定数バッファをピクセルシェーダー用定数バッファレジスタにセット
	SetShaderConstantBuffer(
		cb, DX_SHADERTYPE_PIXEL, CONSTANT_BUF_SLOT_BEGIN_PS);
	
	SetDrawMode(DX_DRAWMODE_NEAREST);
	// 描画
	DrawPolygonIndexed2DToShader(vertexs_, NUM_VERTEX, indexes_, NUM_POLYGON);
	SetDrawMode(DX_DRAWMODE_BILINEAR);

}

void PixelRenderer::Draw(int x, int y)
{
}

void PixelRenderer::MakeSquereVertex(Vector2 pos, Vector2 size)
{
	int cnt = 0;
	float sX = static_cast<float>(pos.x);
	float sY = static_cast<float>(pos.y);
	float eX = static_cast<float>(pos.x+ size.x);
	float eY = static_cast<float>(pos.y+ size.y);

	// ４頂点の初期化
	for (int i = 0; i < 4; i++)
	{
		vertexs_[i].rhw = 1.0f;
		vertexs_[i].dif = GetColorU8(255, 255, 255, 255);
		vertexs_[i].spc = GetColorU8(255, 255, 255, 255);
		vertexs_[i].su = 0.0f;
		vertexs_[i].sv = 0.0f;
	}

	// 左上
	vertexs_[cnt].pos = VGet(sX, sY, 0.0f);
	vertexs_[cnt].u = 0.0f;
	vertexs_[cnt].v = 0.0f;
	cnt++;

	// 右上
	vertexs_[cnt].pos = VGet(eX, sY, 0.0f);
	vertexs_[cnt].u = 1.0f;
	vertexs_[cnt].v = 0.0f;
	cnt++;

	// 右下
	vertexs_[cnt].pos = VGet(eX, eY, 0.0f);
	vertexs_[cnt].u = 1.0f;
	vertexs_[cnt].v = 1.0f;
	cnt++;

	// 左下
	vertexs_[cnt].pos = VGet(sX, eY, 0.0f);
	vertexs_[cnt].u = 0.0f;
	vertexs_[cnt].v = 1.0f;

	// 頂点インデックス
	cnt = 0;
	indexes_[cnt++] = 0;
	indexes_[cnt++] = 1;
	indexes_[cnt++] = 3;

	indexes_[cnt++] = 1;
	indexes_[cnt++] = 2;
	indexes_[cnt++] = 3;
}

