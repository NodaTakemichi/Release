#include"../Pixel/PixelShader2DHeader.hlsli"
#include"../Pixel/PixelFunctionHeader.hlsli"

//�萔�o�b�t�@�F�X���b�g�ԍ�4�Ԗځib4�j
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
    //�X�N���[���摜
    float4 srcCol = tex.Sample(texSampler, uv);

    // �m�C�Y�v�Z
    float noise = noiseValue(uv, g_time);
    //�m�C�Y�J���[
    float3 noiseCol = saturate(float3(noise, noise, noise));
    
    //���X�Ƀm�C�Y���O��
    ret.rgb = lerp(noiseCol, srcCol.rgb, g_progress);

    return frac(ret);
}

