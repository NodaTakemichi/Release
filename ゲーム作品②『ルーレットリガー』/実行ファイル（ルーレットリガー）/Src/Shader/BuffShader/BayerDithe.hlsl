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

static float2 texSize = float2(200.0f, 200.0f);	//画像サイズ
static float luminance = 1.2f;					//輝度
static float bayerLevelMax = 15.0f;				//ベイヤーレベル最大値

//定数バッファ：スロット番号3番目（b3）
cbuffer cbParam : register(b3)
{
	float g_revers;
	float g_time;
	float g_compTime;
}

//ベイヤー配列
static const int BayerPattern[16] = {
	 0, 8, 2,10,
	12, 4,14, 6,
	 3,11, 1, 9,
	15, 7,13, 5
};

//描画するテクスチャ
Texture2D g_SrcTexture:register(t0);

//サンプラー：適切な色を決める
SamplerState g_SrcSampler:register(s0);

float4 main(PS_INPUT PSInput) : SV_TARGET
{
	//UV座標とテクスチャを参照して、最適な色を取得する
    float2 uv = PSInput.TexCoords0;
    float2 revers = float2(abs(g_revers - uv.x), uv.y);
    float4 srcCol =
		g_SrcTexture.Sample(g_SrcSampler, revers);
    if (srcCol.a < 0.01f)
    {
		//描画しない
        discard;
    }
	

	//サイン範囲
    float pro = sin(g_time * g_compTime);
	//ベイヤーレベル
	float level = pro * bayerLevelMax;
	//UV基準
	int x = round(PSInput.TexCoords0.x * texSize.x);
	int y = round(PSInput.TexCoords0.y * texSize.y);
	int dither = (x % 4) + (y % 4 * 4);
	if (int(level) > BayerPattern[dither])
	{
		discard;
	}


    return srcCol * luminance;
}