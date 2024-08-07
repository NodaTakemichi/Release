//インクルードガード
#if !defined(PIXEL_SHADER_3D_HEADER)
	#define PIXEL_SHADER_3D_HEADER

	#include "../Common/CommonShader3DHeader.hlsli"

	//ピクセルシェーダーの入力型のデフォルト定義
	#if !defined(PS_INPUT)
		#include "../Common/VertexToPixelHeader.hlsli"
		#define PS_INPUT VertexToPixel
	#endif

	//ピクセルシェーダーの出力型のデフォルト定義
	#if !defined(PS_OUTPUT)
		#define PS_OUTPUT float4
	#endif

	#include "PixelStructuresHeader.hlsli"
	#include "PixelFunctionHeader.hlsli"

	// 基本パラメータ
	cbuffer cbD3D11_CONST_BUFFER_PS_BASE : register(b1)
	{
	    PsBase base;
	};
	// シャドウマップパラメータ
	cbuffer cbD3D11_CONST_BUFFER_PS_SHADOWMAP : register(b2)
	{
	    PsShadowMap shadowMap;
	};

	SamplerState diffuseMapSampler            : register(s0);           // ディフューズマップテクスチャ
	Texture2D diffuseMapTexture               : register(t0);           // ディフューズマップテクスチャ

	#if defined(BUMPMAP)
		SamplerState normalMapSampler             : register(s1);    		// 法線マップテクスチャ
		Texture2D    normalMapTexture             : register(t1);	    	// 法線マップテクスチャ
	#endif // BUMPMAP

	#if defined(USE_SPETEX)
		SamplerState specularMapSampler           : register(s2);    		// スペキュラマップテクスチャ
		Texture2D    specularMapTexture           : register(t2);	    	// スペキュラマップテクスチャ
	#endif // USE_SPETEX

	#if defined(TOON)
		SamplerState toonDiffuseGradSampler       : register(s3);   		// トゥーンレンダリング用ディフューズカラーグラデーションテクスチャ
		Texture2D    toonDiffuseGradTexture       : register(t3); 	    	// トゥーンレンダリング用ディフューズカラーグラデーションテクスチャ

		SamplerState toonSpecularGradSampler      : register(s4);	    	// トゥーンレンダリング用スペキュラカラーグラデーションテクスチャ
		Texture2D    toonSpecularGradTexture      : register(t4);   		// トゥーンレンダリング用スペキュラカラーグラデーションテクスチャ

		#if defined(TOON_SPHEREOP_MUL) || defined(TOON_SPHEREOP_ADD)
			SamplerState toonSphereMapSampler         : register(s5);     		// トゥーンレンダリング用スフィアマップテクスチャ
			Texture2D    toonSphereMapTexture         : register(t5);	    	// トゥーンレンダリング用スフィアマップテクスチャ
		#endif // TOON_SPHEREOP_MUL || TOON_SPHEREOP_ADD

		SamplerState toonRGBtoVMaxRGBVolumeSampler: register(s6);	    	// トゥーンレンダリング用RGB輝度飽和処理用ボリュームテクスチャ
		Texture3D    toonRGBtoVMaxRGBVolumeTexture: register(t6);		    // トゥーンレンダリング用RGB輝度飽和処理用ボリュームテクスチャ
	#endif // TOON

	#if defined(SUBTEXTUREMODE)
		SamplerState subSampler                   : register(s7);           // サブテクスチャ
		Texture2D subTexture                      : register(t7);           // サブテクスチャ
	#endif // SUBTEXTUREMODE != 0

	#if defined(SHADOWMAP)
		SamplerState shadowMap0Sampler            : register(s8);	    	// シャドウマップ０テクスチャ
		Texture2D    shadowMap0Texture            : register(t8);		    // シャドウマップ０テクスチャ

		SamplerState shadowMap1Sampler            : register(s9);   		// シャドウマップ１テクスチャ
		Texture2D    shadowMap1Texture            : register(t9);	    	// シャドウマップ１テクスチャ

		SamplerState shadowMap2Sampler            : register(s10); 	    	// シャドウマップ２テクスチャ
		Texture2D    shadowMap2Texture            : register(t10);	    	// シャドウマップ２テクスチャ
	#endif

#endif
