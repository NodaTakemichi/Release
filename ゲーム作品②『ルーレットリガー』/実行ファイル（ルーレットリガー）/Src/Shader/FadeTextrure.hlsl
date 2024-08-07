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
Texture2D g_FadeTexture : register(t1);

//サンプラー：適切な色を決める
SamplerState g_SrcSampler : register(s0);

//定数バッファ：スロット番号3番目（b3）
cbuffer cbParam : register(b3)
{
    float g_pra;
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
	//UV座標を受け取る
    float2 uv = PSInput.TexCoords0;

	//UV座標とテクスチャを参照して、最適な色を取得する
    float4 srcCol =
		g_SrcTexture.Sample(g_SrcSampler, uv);
    float4 fadeCol =
		g_FadeTexture.Sample(g_SrcSampler, uv);

	//アルファ値
    float alpha = 1.0f - (fadeCol.r + fadeCol.g + fadeCol.b) / 3.0f;
    alpha -= 1.0;                   //{0.0f}～{-1.0f}の状態にする
    alpha += (g_pra * 2.0f);        //{0.0f}～{2.0f}の時間割合を加算する

    return float4(srcCol.rgb, srcCol.a * alpha);
}