#include"../Pixel/PixelShader2DHeader.hlsli"

#define AMPLITUDE 0.1
#define SPEED 0.1


Texture2D noiseTex : register(t1); //ノイズテクスチャ

//定数バッファ：スロット番号3番目（b3）
cbuffer cbParam : register(b4)
{
    float progress_time;
    float time_adiusted_value;   //タイム調整値
    float amplitude_value; //振幅値
        
}

float4 rgbShift(in float2 p, in float4 shift)
{
    shift *= 2.0 * shift.w - 1.0;
    float2 rs = float2(shift.x, -shift.y);
    float2 gs = float2(shift.y, -shift.z);
    float2 bs = float2(shift.z, -shift.x);
    
    float r = tex.Sample(texSampler, p + rs).x;
    float g = tex.Sample(texSampler, p + gs).y;
    float b = tex.Sample(texSampler, p + bs).z;
    
    return float4(r, g, b, 1.0);
}

float4 noise(in float2 p)
{
    return noiseTex.Sample(texSampler, p);
}

float4 vec4pow(in float4 v, in float p)
{
    return float4(pow(v.x, p), pow(v.y, p), pow(v.z, p), v.w);
}


PS_OUTPUT main(PS_INPUT input) : SV_TARGET
{
	//UV座標調整
    float2 uv = input.uv;
    //出力カラー
    float4 outCol = float4(0.0, 0.0, 0.0, 1.0);
    //タイム
    float time = progress_time % time_adiusted_value;

    //ノイズ画像
    float4 noiseColor = noise(float2(SPEED * time, SPEED * time ));
    //振幅値
    float4 amplitude = float4(AMPLITUDE, AMPLITUDE, AMPLITUDE, amplitude_value);
    float4 shift = vec4pow(noiseColor, amplitude_value) * amplitude;
    
    outCol += rgbShift(uv, shift);

    return outCol;
}