//ピクセルシェーダーの入力
struct PS_INPUT
{
	//座標（プロジェクション空間）
	float4 Position			:SV_POSITION;
	//ディフーズカラー
	float4 Diffuse			:COLORO;
	//テクスチャ座標
	float2 TexCoords0		:TEXCOORD0;
};

//定数バッファ：スロット番号3番目（b3）
cbuffer cbParam : register(b3)
{
	float4 g_color;
	float g_nowRatio;
	float g_hpRatio;
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
	//UV座標を受け取る
	float2 uv = PSInput.TexCoords0;


	float4 color= float4(0.0f, 0.0f, 0.0f, 1.0f);
	float x = 1 - abs(uv.x - 0.5);
	x *= 1.5f;


	float gBack = g_hpRatio;
	float gFront = g_nowRatio;
	float lightRate = 1.5f;
	if (g_nowRatio > g_hpRatio)
	{
		gBack = g_nowRatio;
		gFront = g_hpRatio;
		lightRate = 0.7f;
	}
	
	if (uv.y >= 1.0f - gBack)			//HPゲージ(奥)
	{
		color = float4(x, x, x, 1.0f);
		color.rgb *= lightRate;

		if (uv.y >= 1.0f - gFront)		//HPゲージ（手前）
		{
			color = float4(x, x, x, 1.0f);
		}
	}

	return color *= g_color;
}