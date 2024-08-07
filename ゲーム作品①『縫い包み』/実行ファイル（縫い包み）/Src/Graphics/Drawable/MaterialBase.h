#pragma once
#include<DxLib.h>
#include<vector>

struct ShaderData
{	
	int handle;					//シェーダーハンドル
	int constBufFloat4Size;		//FLOAT4型の定数バッファの個数
	int constBufHandle ;			//定数バッファハンドル
	std::vector<FLOAT4>constBufs;	//定数バッファ配列
};


class MaterialBase
{
public:
	// テクスチャアドレスタイプ
	enum class TEXADDRESS
	{
		NONE = 0,
		WRAP,
		MIRROR,
		CLAMP,
		BORDER,
		MAX
	};

	MaterialBase(void);
	virtual ~MaterialBase(void);

	virtual void Init(void) = 0;

	//テクスチャアドレスを取得
	TEXADDRESS GetTextureAddress(void)const { return texAddress_; }
	//テクスチャアドレスを設定
	void SetTextureAddress(TEXADDRESS texA) { texAddress_ = texA; }

protected:

	/// <summary>
	/// シェーダーデータの設定
	/// </summary>
	/// <param name="shaderHandle">シェーダーハンドル</param>
	/// <param name="constBufSize">定数バッファのサイズ</param>
	/// <returns></returns>
	ShaderData SetShaderData(const int& shaderHandle, const int& constBufSize);

	//テクスチャアドレス
	TEXADDRESS texAddress_;

private:

};

