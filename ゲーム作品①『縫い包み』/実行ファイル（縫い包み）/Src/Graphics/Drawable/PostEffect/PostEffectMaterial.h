#pragma once
#include<DxLib.h>
#include<vector>
#include<string>
#include"../MaterialBase.h"
class PostEffectMaterial:
	public MaterialBase
{
public:
	enum class PS_TYPE
	{
		NOISE,		//ノイズ
		VIGNETTE,	//ビネット
		GLITCH,		//グリッチ
		BLUR		//ブラー
	};

	PostEffectMaterial(void);
	~PostEffectMaterial(void);

	/// <summary>
	/// シェーダーデータのロード
	/// </summary>
	/// <param name="psType">ピクセルシェーダーのタイプ</param>
	void DataLoad(const PS_TYPE& psType);
	//初期化
	void Init(void);

	//定数バッファのセット
	void SetConstBuf(int idx, const FLOAT4& constBuf);

	//シェーダーデータの取得
	const ShaderData& GetShaderDataPS(void)const { return psData_; }

	//ポストエフェクト用スクリーンの取得
	const int& GetPEScreen(void)const { return postEffectScreen_; }

	//テクスチャセット
	void SetTextures(const std::vector<int>& texs) { texture_ = texs; };
	//テクスチャ配列の取得
	const std::vector<int>& GetTextures(void)const { return texture_; }
private:

	//シェーダーデータ
	ShaderData psData_;

	//画像
	std::vector<int> texture_;

	//ポストエフェクト用スクリーン
	int postEffectScreen_;

	//スクリーン座標の作成
	void MakePostEffecctScreen(void);

};

