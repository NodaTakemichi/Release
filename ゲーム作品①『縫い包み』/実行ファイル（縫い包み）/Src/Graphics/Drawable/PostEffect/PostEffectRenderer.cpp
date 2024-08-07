#include "../../../Application.h"
#include "../../../Manager/SceneManager.h"
#include "PostEffectMaterial.h"
#include "PostEffectRenderer.h"

PostEffectRenderer::PostEffectRenderer(PostEffectMaterial& postEffectMaterial):
	postEffectMaterial_(postEffectMaterial)
{
	//ポリゴンの作成
	MakeSquereVertex();

	//メインスクリーン
	mainScreen_ = SceneManager::GetInstance().GetMainScreen();
}


PostEffectRenderer::~PostEffectRenderer(void)
{
}


void PostEffectRenderer::Draw(void)
{
	//スクリーンをセット
	int screen = postEffectMaterial_.GetPEScreen();
	SetDrawScreen(screen);
	ClearDrawScreen();

	//シェーダーデータ：PS
	auto psData = postEffectMaterial_.GetShaderDataPS();

	// シェーダー設定
	SetUsePixelShader(psData.handle);

	//定数バッファ
	auto cb = psData.constBufHandle;

	// テクスチャの設定
	//一枚目は必ず、メインスクリーンを渡す
	SetUseTextureToShader(0, mainScreen_);
	//2枚目以降
	int index = 1;
	for (const auto& tex : postEffectMaterial_.GetTextures())
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

	// 描画
	DrawPolygonIndexed2DToShader(vertexs_, NUM_VERTEX, indexes_, NUM_POLYGON);

	// メインに戻す
	SetDrawScreen(mainScreen_);
	DrawGraph(0, 0, screen, false);
}

void PostEffectRenderer::MakeSquereVertex(void)
{
	int cnt = 0;
	float sX = static_cast<float>(0.0f);
	float sY = static_cast<float>(0.0f);
	float eX = static_cast<float>(Application::SCREEN_SIZE_X);
	float eY = static_cast<float>(Application::SCREEN_SIZE_Y);

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
