#include"../Pixel/PixelShader2DHeader.hlsli"

#define AMPLITUDE 0.1
#define SPEED 0.1


Texture2D noiseTex : register(t1); //�m�C�Y�e�N�X�`��

//�萔�o�b�t�@�F�X���b�g�ԍ�3�Ԗځib3�j
cbuffer cbParam : register(b4)
{
    float progress_time;
    float time_adiusted_value;   //�^�C�������l
    float amplitude_value; //�U���l
        
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
	//UV���W����
    float2 uv = input.uv;
    //�o�̓J���[
    float4 outCol = float4(0.0, 0.0, 0.0, 1.0);
    //�^�C��
    float time = progress_time % time_adiusted_value;

    //�m�C�Y�摜
    float4 noiseColor = noise(float2(SPEED * time, SPEED * time ));
    //�U���l
    float4 amplitude = float4(AMPLITUDE, AMPLITUDE, AMPLITUDE, amplitude_value);
    float4 shift = vec4pow(noiseColor, amplitude_value) * amplitude;
    
    outCol += rgbShift(uv, shift);

    return outCol;
}