#include"../Pixel/PixelShader2DHeader.hlsli"
#include"../Pixel/PixelFunctionHeader.hlsli"

//定数バッファ：スロット番号4番目（b4）
cbuffer cbParam : register(b4)
{
    float g_time;
    float g_progress;
}

float noiseValue(in float2 uv,in float value )
{
    float noiseValue = frac(sin(
    dot(uv * value, float2(12.9898f, 78.233f))) * 43758.5453f) - 0.5f;
    return noiseValue;
}


PS_OUTPUT main(PS_INPUT input) : SV_TARGET
{  
    float4 ret = float4(0.0f, 0.0f, 0.0f, 1.0f);
    
    float2 uv = input.uv;
    //スクリーン画像
    float4 srcCol = tex.Sample(texSampler, uv);

    // ノイズ計算
    float noise = noiseValue(uv, g_time);
    //ノイズカラー
    float3 noiseCol = saturate(float3(noise, noise, noise));
    
    //徐々にノイズを外す
    ret.rgb = lerp(noiseCol, srcCol.rgb, g_progress);

    return frac(ret);
}

