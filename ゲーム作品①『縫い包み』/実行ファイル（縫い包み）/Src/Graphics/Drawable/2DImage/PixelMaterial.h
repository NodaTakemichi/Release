#pragma once
#include<DxLib.h>
#include<vector>
#include<string>
#include"../MaterialBase.h"
class PixelMaterial
	:public MaterialBase
{
public:
	enum class PS_TYPE
	{
		BURN_GAUGE,	//焼却ゲージ
		DISTORTION,	//歪み壁
	};

	PixelMaterial(void);
	~PixelMaterial(void);

	/// <summary>
	/// シェーダーデータのロード
	/// </summary>
	/// <param name="psType">ピクセルシェーダーのタイプ</param>
	void DataLoad(const PS_TYPE& psType);
	//初期化
	void Init(void);

	//シェーダーデータの取得
	const ShaderData& GetShaderDataPS(void)const { return psData_; }

	//定数バッファの更新
	void SetConstBuf(const std::vector<FLOAT4>& constBuf) { psData_ .constBufs=constBuf; }

	//テクスチャセット
	void SetTextures(const std::vector<int>& texs) { texture_ = texs; };
	//テクスチャ配列の取得
	const std::vector<int>& GetTextures(void)const { return texture_; }

private:
	//シェーダーデータ
	ShaderData psData_;

	//画像
	std::vector<int> texture_;

};

