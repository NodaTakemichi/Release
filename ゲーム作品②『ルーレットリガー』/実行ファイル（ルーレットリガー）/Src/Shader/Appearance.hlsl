//ピクセルシェーダーの入力
struct PS_INPUT
{
	//座標（プロジェクション空間）
    float4 Position : SV_POSITION;
	//ディフーズカラー
    float4 Diffuse : COLORO;
	//テクスチャ座標
    float2 TexCoords0 : TEXCOORD0;
};

//描画するテクスチャ
Texture2D g_SrcTexture : register(t0);

//サンプラー：適切な色を決める
SamplerState g_SrcSampler : register(s0);

//定数バッファ
cbuffer cbParam : register(b3)
{
    float g_time;
    float g_slow;
}


float4 main(PS_INPUT PSInput) : SV_TARGET
{
    //タイムが０の場合、表示しない
    if (g_time<0)
    {
        discard;
    }

	//UV座標を受け取る
    float2 uv = PSInput.TexCoords0;
    float4 srcCol =
		g_SrcTexture.Sample(g_SrcSampler, uv);

    float pro = g_time / g_slow;
    pro = min(pro, 1.0f);

    //線形補間
    float3 result = lerp(1.0f, srcCol. rgb, pro);


    return float4(result, srcCol.a);
}