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
Texture2D g_Tex : register(t0);
Texture2D g_MaskTex:register(t1);

//サンプラー：適切な色を決める
SamplerState g_SrcSampler:register(s0);

float4 main(PS_INPUT PSInput) : SV_TARGET
{
	//UV座標を受け取る
	float2 uv = PSInput.TexCoords0;

	//マスク画像のアルファ値が０以下は描画しない
	float4 maskCol = g_MaskTex.Sample(g_SrcSampler, uv);
	if (maskCol.a <= 0)
	{
		discard;
    }

	//UV座標とテクスチャを参照して、最適な色を取得する
	float4 srcCol =
		g_Tex.Sample(g_SrcSampler, uv);

	return float4(srcCol.rgb,1.0f);
}