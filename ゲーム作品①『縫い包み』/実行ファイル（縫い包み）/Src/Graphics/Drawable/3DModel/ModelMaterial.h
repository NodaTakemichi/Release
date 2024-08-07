#pragma once
#include<DxLib.h>
#include<vector>
#include<map>
#include<string>
#include"../../../Manager/ResourceManager.h"
#include"../MaterialBase.h"

class ModelMaterial:
	MaterialBase
{
public:	
	enum class VS_TYPE
	{
		MODEL_NORMAL_1FRAME,		//ノーマル
	};

	enum class PS_TYPE
	{
		MODEL_NORMAL,		//ノーマル
		BURNING_DOOL,		//焼却ぬいぐるみ	
		DISTORTION_WALL,	//歪み壁
		SPHERE_MAP,			//スフィアマップ
		LUMINESCENCE		//発光モデル
	};

	ModelMaterial(void);
	~ModelMaterial(void);

	/// <summary>
	/// シェーダーデータのロード
	/// </summary>
	/// <param name="vsType">頂点シェーダーのタイプ</param>
	/// <param name="psType">ピクセルシェーダーのタイプ</param>
	void DataLoad(const VS_TYPE& vsType, const PS_TYPE& psType);
	//初期化
	void Init(void);


	//テクスチャの更新
	void SetTextures(int slot ,int texDiffuse);

	//定数バッファの追加
	void AddConstBufVS(const FLOAT4& constBuf);
	void AddConstBufPS(const FLOAT4& constBuf);

	//定数バッファの更新
	void SetConstBufVS(int idx ,const FLOAT4& constBuf);
	void SetConstBufPS(int idx ,const FLOAT4& constBuf);

	//シェーダーデータの取得
	const ShaderData& GetShaderDataVS(void)const { return vsData_; }
	const ShaderData& GetShaderDataPS(void)const { return psData_; }

	//テクスチャ配列の取得
	const std::map<int,int>& GetTextures(void)const { return texture_; }

	//テクスチャアドレスを取得
	TEXADDRESS GetTextureAddress(void)const { return texAddress_; }
	//テクスチャアドレスを設定
	void SetTextureAddress(TEXADDRESS texA) { texAddress_ = texA; }

private:

	//シェーダーデータ
	ShaderData vsData_;
	ShaderData psData_;


	//画像
	std::map<int,int> texture_;


};

