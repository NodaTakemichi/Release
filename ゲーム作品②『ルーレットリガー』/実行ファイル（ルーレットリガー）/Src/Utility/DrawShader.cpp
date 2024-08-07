#include <string>
#include "../Common/Vector2.h"
#include"../_debug/_DebugConOut.h"
#include "DrawShader.h"

DrawShader* DrawShader::instance_ = nullptr;

void DrawShader::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new DrawShader();
	}
	instance_->Init();
}

DrawShader& DrawShader::GetInstance(void)
{
	return *instance_;
}

void DrawShader::Init(void)
{
	//ピクセルシェーダーの登録
	InitPS();


	//定数バッファ
	psConstBuf_= CreateShaderConstantBuffer(sizeof(float) * 8);
}

void DrawShader::Release(void)
{
	//シェーダーの開放
	for (auto& ps : psMap_)
	{
		DeleteShader(ps.second);
	}
	psMap_.clear();
	//シェーダー定数バッファの開放
	DeleteShaderConstantBuffer(psConstBuf_);
}

void DrawShader::DrawGraph(const Vector2& pos, const int& handle)
{
	//シェーダーの設定
	int ps = SearchPS(PS_TYPE::TEXTURE);
	SetUsePixelShader(ps);

	//シェーダーにテクスチャを転送
	SetUseTextureToShader(0, handle);

	//サイズ
	int x, y;
	GetGraphSize(handle, &x, &y);

	//描画座標
	MakeSquereVertex(pos, { x,y });

	//描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 0);
	DrawPolygonIndexed2DToShader(vertex_, NUM_VERTEX, index_, NUM_POLYGON);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void DrawShader::DrawGraphToShader(
	const Vector2& pos, const int& handle, const PS_TYPE& type)
{
	//シェーダーの設定
	int ps = SearchPS(type);
	SetUsePixelShader(ps);
	//シェーダーにテクスチャを転送
	SetUseTextureToShader(0, handle);

	//サイズ
	int x, y;
	GetGraphSize(handle, &x, &y);

	//描画座標
	MakeSquereVertex(pos, { x,y });

	//描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 0);
	DrawPolygonIndexed2DToShader(vertex_, NUM_VERTEX, index_, NUM_POLYGON);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void DrawShader::DrawGraphToShader(
	const Vector2& pos, const int& handle, const PS_TYPE& type,
	const COLOR_F& buf, const COLOR_F& subBuf)
{
	//シェーダーの設定
	int ps = SearchPS(type);
	SetUsePixelShader(ps);

	//シェーダーにテクスチャを転送
	SetUseTextureToShader(0, handle);

	//シェーダー用の定数バッファ
	auto& cBuf = psConstBuf_;

	//ピクセルシェーダー用の定数バッファのアドレスを取得
	COLOR_F* cbBuf =
		(COLOR_F*)GetBufferShaderConstantBuffer(cBuf);
	*cbBuf = buf;
	cbBuf++;
	*cbBuf = subBuf;

	//サイズ
	int x, y;
	GetGraphSize(handle, &x, &y);

	//描画座標
	MakeSquereVertex(pos, {x,y});

	//ピクセルシェーダー用の定数バッファを更新して書き込んだ内容を反映する
	UpdateShaderConstantBuffer(cBuf);

	//ピクセルシェーダー用の定数バッファを定数バッファレジスタにセット
	SetShaderConstantBuffer(cBuf, DX_SHADERTYPE_PIXEL, CONSTANT_BUF_SLOT_BEGIN_PS);

	//描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 0);
	DrawPolygonIndexed2DToShader(vertex_, NUM_VERTEX, index_, NUM_POLYGON);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void DrawShader::DrawExtendGraphToShader(const Vector2& pos, const Vector2& size, const int& handle)
{

	//シェーダーの設定
	int ps = SearchPS(PS_TYPE::TEXTURE);
	SetUsePixelShader(ps);

	//シェーダーにテクスチャを転送
	SetUseTextureToShader(0, handle);

	//描画座標
	MakeSquereVertex(pos, size);

	//バイリニア補間モード
	SetDrawMode(DX_DRAWMODE_BILINEAR);

	//描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 0);
	DrawPolygonIndexed2DToShader(vertex_, NUM_VERTEX, index_, NUM_POLYGON);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//ネアレストネイバー法
	SetDrawMode(DX_DRAWMODE_NEAREST);
}

void DrawShader::DrawExtendGraphToShader(
	const Vector2& pos, const Vector2& size, 
	const int& handle, const PS_TYPE& type,
	const COLOR_F& buf, const COLOR_F& subBuf)
{
	//シェーダーの設定
	int ps = SearchPS(type);
	SetUsePixelShader(ps);

	//シェーダーにテクスチャを転送
	if (handle != -1)SetUseTextureToShader(0, handle);

	//シェーダー用の定数バッファ
	auto& cBuf = psConstBuf_;

	//ピクセルシェーダー用の定数バッファのアドレスを取得
	COLOR_F* cbBuf =
		(COLOR_F*)GetBufferShaderConstantBuffer(cBuf);
	*cbBuf = buf;
	cbBuf++;
	*cbBuf = subBuf;

	//描画座標
	MakeSquereVertex(pos,size);

	//ピクセルシェーダー用の定数バッファを更新して書き込んだ内容を反映する
	UpdateShaderConstantBuffer(cBuf);

	//ピクセルシェーダー用の定数バッファを定数バッファレジスタにセット
	SetShaderConstantBuffer(cBuf, DX_SHADERTYPE_PIXEL, CONSTANT_BUF_SLOT_BEGIN_PS);

	//バイリニア補間モード
	SetDrawMode(DX_DRAWMODE_BILINEAR);

	//描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 0);
	DrawPolygonIndexed2DToShader(vertex_, NUM_VERTEX, index_, NUM_POLYGON);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//ネアレストネイバー法
	SetDrawMode(DX_DRAWMODE_NEAREST);
}

void DrawShader::DrawGraphAndSubToShader(
	const Vector2& pos,
	const int& mHandle, const int& sHandle, const PS_TYPE& type,
	const COLOR_F& buf, const COLOR_F& subBuf)
{
	//シェーダーの設定
	int ps = SearchPS(type);
	SetUsePixelShader(ps);

	//シェーダーにテクスチャを転送
	SetUseTextureToShader(0, mHandle);
	SetUseTextureToShader(1, sHandle);

	//シェーダー用の定数バッファ
	auto& cBuf = psConstBuf_;

	//ピクセルシェーダー用の定数バッファのアドレスを取得
	COLOR_F* cbBuf =
		(COLOR_F*)GetBufferShaderConstantBuffer(cBuf);
	*cbBuf = buf;
	cbBuf++;
	*cbBuf = subBuf;

	//サイズ
	int x, y;
	GetGraphSize(mHandle, &x, &y);
	//描画座標
	MakeSquereVertex(pos, {x,y});


	//ピクセルシェーダー用の定数バッファを更新して書き込んだ内容を反映する
	UpdateShaderConstantBuffer(cBuf);

	//ピクセルシェーダー用の定数バッファを定数バッファレジスタにセット
	SetShaderConstantBuffer(cBuf, DX_SHADERTYPE_PIXEL, CONSTANT_BUF_SLOT_BEGIN_PS);

	//描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 0);
	DrawPolygonIndexed2DToShader(vertex_, NUM_VERTEX, index_, NUM_POLYGON);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void DrawShader::DrawGraphAndSubToShader(
	const Vector2& pos, const Vector2& size, 
	const int& mHandle, const int& sHandle, const PS_TYPE& type,
	const COLOR_F& buf, const COLOR_F& subBuf)
{
	//シェーダーの設定
	int ps = SearchPS(type);
	SetUsePixelShader(ps);

	//シェーダーにテクスチャを転送
	SetUseTextureToShader(0, mHandle);
	SetUseTextureToShader(1, sHandle);

	//シェーダー用の定数バッファ
	auto& cBuf = psConstBuf_;

	//ピクセルシェーダー用の定数バッファのアドレスを取得
	COLOR_F* cbBuf =
		(COLOR_F*)GetBufferShaderConstantBuffer(cBuf);
	*cbBuf = buf;
	cbBuf++;
	*cbBuf = subBuf;

	//描画座標
	MakeSquereVertex(pos, size);

	//ピクセルシェーダー用の定数バッファを更新して書き込んだ内容を反映する
	UpdateShaderConstantBuffer(cBuf);

	//ピクセルシェーダー用の定数バッファを定数バッファレジスタにセット
	SetShaderConstantBuffer(cBuf, DX_SHADERTYPE_PIXEL, CONSTANT_BUF_SLOT_BEGIN_PS);

	//バイリニア補間モード
	SetDrawMode(DX_DRAWMODE_BILINEAR);

	//描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 0);
	DrawPolygonIndexed2DToShader(vertex_, NUM_VERTEX, index_, NUM_POLYGON);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//ネアレストネイバー法
	SetDrawMode(DX_DRAWMODE_NEAREST);

}

void DrawShader::MakeSquereVertex(Vector2 pos, Vector2 size)
{

	int cnt = 0;
	float sX = static_cast<float>(pos.x);
	float sY = static_cast<float>(pos.y);
	float eX = static_cast<float>(pos.x + size.x);
	float eY = static_cast<float>(pos.y + size.y);

	// ４頂点の初期化
	for (int i = 0; i < NUM_VERTEX; i++)
	{
		vertex_[i].rhw = 1.0f;
		vertex_[i].dif = GetColorU8(255, 255, 255, 255);
		vertex_[i].spc = GetColorU8(255, 255, 255, 255);
		vertex_[i].su = 0.0f;
		vertex_[i].sv = 0.0f;
	}

	// 左上
	vertex_[cnt].pos = VGet(sX, sY, 0.0f);
	vertex_[cnt].u = 0.0f;
	vertex_[cnt].v = 0.0f;
	cnt++;

	// 右上
	vertex_[cnt].pos = VGet(eX, sY, 0.0f);
	vertex_[cnt].u = 1.0f;
	vertex_[cnt].v = 0.0f;
	cnt++;

	// 右下
	vertex_[cnt].pos = VGet(eX, eY, 0.0f);
	vertex_[cnt].u = 1.0f;
	vertex_[cnt].v = 1.0f;
	cnt++;

	// 左下
	vertex_[cnt].pos = VGet(sX, eY, 0.0f);
	vertex_[cnt].u = 0.0f;
	vertex_[cnt].v = 1.0f;

	// 頂点インデックス
	cnt = 0;
	index_[cnt++] = 0;
	index_[cnt++] = 1;
	index_[cnt++] = 3;

	index_[cnt++] = 1;
	index_[cnt++] = 2;
	index_[cnt++] = 3;
}

void DrawShader::InitPS(void)
{
	std::string path = "Data/Shader/";
	std::string fileName;

	auto loadPS = [&](PS_TYPE type,std::string fName) {
		fileName = path + fName;
		psMap_.emplace(std::make_pair(type, LoadPixelShader(fileName.c_str())));
	};

	//各シェーダーの登録
	loadPS(PS_TYPE::TEXTURE,		"Texture.cso");
	loadPS(PS_TYPE::REVERSE_TEXTURE,"ReverseTexture.cso");
	loadPS(PS_TYPE::MONOTONE,		"Monotone.cso");
	loadPS(PS_TYPE::NORMAL_MAP,		"NormalMap.cso");
	loadPS(PS_TYPE::MASK,			"Mask.cso");
	loadPS(PS_TYPE::APPEARANCE,		"Appearance.cso");
	loadPS(PS_TYPE::DISTORTION,		"Distortion.cso");
	loadPS(PS_TYPE::SIMPLE_SEPIA,	"SimpleSepia.cso");
	loadPS(PS_TYPE::FADE,			"Fade.cso");			
	loadPS(PS_TYPE::FADE_TEXTURE,	"FadeTextrure.cso");

	loadPS(PS_TYPE::ON_BUTTON,		"OnButton.cso");
	loadPS(PS_TYPE::ON_CIRCLE_BUTTON,"OnCircleButton.cso");
	loadPS(PS_TYPE::CARD_FRAME,		"CardFrame.cso");
	loadPS(PS_TYPE::IUMI_FRAME,		"IumiFrame.cso");
	loadPS(PS_TYPE::HP_SHADER,		"HpShader.cso");
	loadPS(PS_TYPE::DEATH_SHADER,	"DeathShader.cso");
	loadPS(PS_TYPE::DEATH_UNIT,		"DeathUnit.cso");

	loadPS(PS_TYPE::STATUS_UP,		"StatusUp.cso");
	loadPS(PS_TYPE::STATUS_DOWN,	"StatusDown.cso");
	loadPS(PS_TYPE::PARALYSIS,		"Paralysis.cso");
	loadPS(PS_TYPE::POISON,			"Poison.cso");
	loadPS(PS_TYPE::BAYER_DITHE,	"BayerDithe.cso");


}

const int& DrawShader::SearchPS(const PS_TYPE& type)const
{
	//登録が行われているか判断
	const auto& psPair = psMap_.find(type);
	if (psPair == psMap_.end())
	{
		//登録されていない
		TRACE("ピクセルシェーダーが登録されていません");
		return 0;
	}

	return psMap_.at(type);
}

DrawShader::DrawShader(void)
{
	psConstBuf_ = 0;
}

DrawShader::~DrawShader(void)
{
	delete instance_;
}
