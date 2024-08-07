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
	float g_time;
	float g_per;
}

//描画するテクスチャ
Texture2D g_SrcTexture:register(t0);

//サンプラー：適切な色を決める
SamplerState g_SrcSampler:register(s0);

float4 main(PS_INPUT PSInput) : SV_TARGET
{
	//UV座標を受け取る
	float2 uv = PSInput.TexCoords0;

	//UV座標とテクスチャを参照して、最適な色を取得する
	float4 srcCol =
		g_SrcTexture.Sample(g_SrcSampler, uv);



	//シンプルセピア
	float3 sepia = srcCol.rgb;
	sepia.r = dot(srcCol.rgb, float3(0.393f, 0.769f, 0.189f));
	sepia.g = dot(srcCol.rgb, float3(0.349f, 0.686f, 0.168f));
	sepia.b = dot(srcCol.rgb, float3(0.272f, 0.534f, 0.131f));

	//線形補間（色）
	srcCol.rgb = lerp(srcCol.rbg, sepia, saturate(g_per));


	return srcCol;
}