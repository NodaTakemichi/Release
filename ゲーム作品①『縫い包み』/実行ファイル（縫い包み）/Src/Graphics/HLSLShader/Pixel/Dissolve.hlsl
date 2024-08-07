#include"PixelShader2DHeader.hlsli"

//浸食速度
static float dissolveSpeed = 3.0f;
//浸食カラー
static float3 redColor = float3(1.0f, 0.2f, 0.2f);
static float3 blockColor = float3(0.2f, 0.0f, 0.0f);
//消える閾値
static float discardValue = 0.9f;
//線形補完調整
static float lerpAdjustmentValue = 10.0f;

//定数バッファ：スロット番号4番目（b4）
cbuffer cbParam : register(b4)
{
    float g_time;
}

//ノイズ画像
Texture2D noiseTex : register(t1);

PS_OUTPUT main(PS_INPUT input) : SV_TARGET
{ 

    //描画画像
    float4 srcCol = tex.Sample(texSampler, input.uv);
    
    //ノイズ画像
    float4 noiseCol =
		noiseTex.Sample(texSampler, input.uv);


    //ノイズ画像の色の平均(０～１)
    float alpha = (noiseCol.r + noiseCol.g + noiseCol.b) / 3.0f;
    //１～２
    alpha += 1.0f;
    alpha -= g_time / dissolveSpeed;
    
    //白い部分から消えていく
    if (alpha < discardValue)
    {
        discard;
    }
    //通常描画
    if (alpha > 1.0f)
    {
        return srcCol;
    }
    
        
    //割合
    float par = alpha - discardValue;
    srcCol.rgb = lerp(redColor, blockColor, par * lerpAdjustmentValue);
    
    return srcCol;
}