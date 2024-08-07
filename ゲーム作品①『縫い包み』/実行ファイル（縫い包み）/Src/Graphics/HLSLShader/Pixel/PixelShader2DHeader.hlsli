//インクルードガード
#if !defined(PIXEL_SHADER_2D_HEADER)
	#define PIXEL_SHADER_2D_HEADER
		
	//ピクセルシェーダーの入力型のデフォルト定義
	#if !defined(PS_INPUT)
		struct PixelInput
		{
		    float4 svPos : SV_POSITION;
		    float4 diffuse : COLOR0;		//拡散反射の色
		    //float4 specular : COLOR1; //鏡面反射の色(もしver3.24c以降のライブラリを使用している場合はコメントを外す)
		    float2 uv : TEXCOORD0;			//UV値
		    float2 suv : TEXCOORD1;			//サブテクスチャのUV値
		};
		#define PS_INPUT PixelInput

	#endif

	//ピクセルシェーダーの出力型のデフォルト定義
	#if !defined(PS_OUTPUT)
		#define PS_OUTPUT float4
	#endif

	Texture2D tex : register(t0);               //テクスチャ
	SamplerState texSampler : register(s0);     //サンプラー

#endif
