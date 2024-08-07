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
}

//描画するテクスチャ
Texture2D g_SrcTexture:register(t0);

//サンプラー：適切な色を決める
SamplerState g_SrcSampler:register(s0);

float4 main(PS_INPUT PSInput) : SV_TARGET
{
	//UV座標
	float2 uv = PSInput.TexCoords0;


	//横軸
    float xSin = sin((uv.x + (g_time / 2)) * 7.0f);
	// 時間経過に応じて画像のy座標を変化させる
	float amplitube = 4.5f;
	float offsetY = sin(g_time * amplitube) * 0.1f * xSin; // 5.0fは振幅を調整する値

	// 歪んだテクスチャ座標でテクスチャをサンプリングして出力
	float2 distortedTexCoord = uv + float2(0, offsetY);

	float4 srcCol = g_SrcTexture.Sample(g_SrcSampler, distortedTexCoord);

	return srcCol;
}