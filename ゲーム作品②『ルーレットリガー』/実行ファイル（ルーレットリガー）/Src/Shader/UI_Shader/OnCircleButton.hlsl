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

//定数バッファ：スロット番号3番目（b3）
cbuffer cbParam : register(b3)
{
    float g_darkness;
}

//描画するテクスチャ
Texture2D g_SrcTexture : register(t0);

//サンプラー：適切な色を決める
SamplerState g_SrcSampler : register(s0);

float4 main(PS_INPUT PSInput) : SV_TARGET
{
	//UV座標を受け取る
    float2 uv = PSInput.TexCoords0;

	//UV座標とテクスチャを参照して、最適な色を取得する
    float4 srcCol =
		g_SrcTexture.Sample(g_SrcSampler, uv);
    
    //描画外
    if (srcCol.a <= 0)
    {
        discard;
    }
    
    float2 center = abs(uv - 0.5f) * 2.0f;
    
    float c = center.x + center.y;
    c /= (4.0f + g_darkness);
    
    return float4(srcCol.rgb + c, srcCol.a);
}