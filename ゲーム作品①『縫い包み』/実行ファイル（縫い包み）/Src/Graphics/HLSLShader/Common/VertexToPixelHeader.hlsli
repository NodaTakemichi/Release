//インクルードガード
#if !defined(VERTEX_TO_PIXEL_HEADER)
	#define VERTEX_TO_PIXEL_HEADER

	#if !defined(VERTEX_TO_PIXEL_TYPE)
		#include "VertexToPixelTypeHeader.hlsli"
		#define VERTEX_TO_PIXEL_TYPE VERTEX_TO_PIXEL_TYPE_DEFAULT
	#endif

	#if (VERTEX_TO_PIXEL_TYPE == VERTEX_TO_PIXEL_TYPE_DEFAULT)
		//法線マップなしの場合
		struct VertexToPixel
		{
		    float4 svPos : SV_POSITION;		//位置(プロジェクション)
		    float3 worldPos : POSITION0;	//位置(ワールド)
		    float3 viewPos : POSITION1;		//位置(ビュー)
		    float3 viewNorm : NORMAL0;		//法線
		    float4 diffuse : COLOR0;		//拡散反射色
		    float4 specular : COLOR1;		//鏡面反射色
		    float2 uv : TEXCOORD;			//UV値
		};
	#elif (VERTEX_TO_PIXEL_TYPE == VERTEX_TO_PIXEL_TYPE_NORMAL_MAP)
		//法線マップありの場合
		struct VertexToPixel
		{
			float4 svPos : SV_POSITION;		//位置(プロジェクション)
		    float3 worldPos : POSITION0;	//位置(ワールド)
		    float3 viewPos : POSITION1;		//位置(ビュー)
		    float3 viewTan : TANGENT0;		//接線
		    float3 viewBin : BINORMAL;		//従法線
		    float3 viewNorm : NORMAL0;		//法線
		    float4 diffuse : COLOR0;		//拡散反射色
		    float4 specular : COLOR1;		//鏡面反射色
		    float2 uv : TEXCOORD0;			//UV値
		    float fogFactor : COLOR2;			//フォグの強さ
		};
	#endif
#endif

