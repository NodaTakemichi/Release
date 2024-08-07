//インクルードガード
#if !defined(VERTEX_SHADER_3D_HEADER)
	#define VERTEX_SHADER_3D_HEADER

	#include "../Common/CommonShader3DHeader.hlsli"

	//頂点シェーダーの入力型のデフォルト定義
	#if !defined(VS_INPUT)
		#include "VertexInputHeader.hlsli"
		#define VS_INPUT VertexInput
	#endif

	//頂点シェーダーの出力型のデフォルト定義
	#if !defined(VS_OUTPUT)
		#include "../Common/VertexToPixelHeader.hlsli"
		#define VS_OUTPUT VertexToPixel
	#endif

	#include "VertexStructuresHeader.hlsli"
	#include "VertexFunctionHeader.hlsli"

	// 基本パラメータ
	cbuffer cbD3D11_CONST_BUFFER_VS_BASE : register(b1)
	{
	    VsBase base;
	};

	// その他の行列
	cbuffer cbD3D11_CONST_BUFFER_VS_OTHERMATRIX : register(b2)
	{
	    VsOtherMatrix otherMatrix;
	};

	// スキニングメッシュ用の　ローカル　→　ワールド行列
	cbuffer cbD3D11_CONST_BUFFER_VS_LOCALWORLDMATRIX : register(b3)
	{
	    VsLocalWorldMatrix localWorldMatrix;
	};

#endif