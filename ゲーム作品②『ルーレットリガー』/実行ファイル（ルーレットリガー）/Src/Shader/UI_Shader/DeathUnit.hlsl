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



//描画するテクスチャ
Texture2D g_Texture : register(t0);

//サンプラー：適切な色を決める
SamplerState g_SrcSampler:register(s0);

float4 main(PS_INPUT PSInput) : SV_TARGET
{
	//UV座標を受け取る
	float2 uv = PSInput.TexCoords0;


	//UV座標とテクスチャを参照して、最適な色を取得する
	float4 srcCol =
		g_Texture.Sample(g_SrcSampler, uv);
		

    float3 reverse = 1.0f - srcCol.rbg;
	
    return float4(reverse, srcCol.a);
}