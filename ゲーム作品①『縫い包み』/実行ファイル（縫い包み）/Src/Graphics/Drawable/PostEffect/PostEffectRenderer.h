#pragma once
#include<DxLib.h>
#include"../../../Common/Vector2.h"
class PostEffectMaterial;


class PostEffectRenderer
{
public:
	//コンストラクタ
	PostEffectRenderer(PostEffectMaterial& postEffectMaterial);
	~PostEffectRenderer(void);

	//描画
	void Draw(void);

	//描画矩形の生成
	void MakeSquereVertex(void);
	

private:
	// ピクセルシェーダ用オリジナル定数バッファの使用開始スロット
	static constexpr int CONSTANT_BUF_SLOT_BEGIN_PS = 4;
	// 頂点数
	static constexpr int NUM_VERTEX = 4;
	// 頂点インデックス数
	static constexpr int NUM_VERTEX_IDX = 6;
	// ポリゴン数
	static constexpr int NUM_POLYGON = 2;


	//頂点
	VERTEX2DSHADER vertexs_[NUM_VERTEX];

	//頂点インデックス
	WORD indexes_[NUM_VERTEX_IDX];

	//ピクセルマテリアル
	PostEffectMaterial& postEffectMaterial_;

	//メインスクリーン
	int mainScreen_;

};

