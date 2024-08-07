#include"PixelShader2DHeader.hlsli"

//定数バッファ：スロット番号4番目（b4）
cbuffer cbParam : register(b4)
{
    float g_par;    //ゲージ割合
    float2 g_size;  //画像サイズ
    float padding;
}

//ベイヤー配列
static const int BayerPattern[16] =
{
     0, 8, 2, 10,
	12, 4, 14, 6,
	 3, 11, 1, 9,
	15, 7, 13, 5
};

Texture2D maskTex : register(t1); //マスク画像
Texture2D frameTex : register(t2); //フレーム画像

PS_OUTPUT main(PS_INPUT input) : SV_TARGET
{
    float4 result = float4(0.0f, 0.0f, 0.0f, 0.0f);

    //フレーム画像
    float4 frameCol = frameTex.Sample(texSampler, input.uv);
    if (frameCol.a <= 0.0f)
    {
        discard;
    }
    result = frameCol;
    

    //メイン画像
    float4 mainCol = tex.Sample(texSampler, input.uv);
    //マスク画像
    float4 maskCol = maskTex.Sample(texSampler, input.uv);
    
    //パーセントゲージ以下　＆　マスク画像上　の時
    if (input.uv.x <= g_par && maskCol.a >= 0.1f)
    {
        result = mainCol;
    }  
    
    return result;
}