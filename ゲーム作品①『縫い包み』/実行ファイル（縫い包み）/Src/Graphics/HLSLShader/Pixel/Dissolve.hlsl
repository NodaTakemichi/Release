#include"PixelShader2DHeader.hlsli"

//�Z�H���x
static float dissolveSpeed = 3.0f;
//�Z�H�J���[
static float3 redColor = float3(1.0f, 0.2f, 0.2f);
static float3 blockColor = float3(0.2f, 0.0f, 0.0f);
//������臒l
static float discardValue = 0.9f;
//���`�⊮����
static float lerpAdjustmentValue = 10.0f;

//�萔�o�b�t�@�F�X���b�g�ԍ�4�Ԗځib4�j
cbuffer cbParam : register(b4)
{
    float g_time;
}

//�m�C�Y�摜
Texture2D noiseTex : register(t1);

PS_OUTPUT main(PS_INPUT input) : SV_TARGET
{ 

    //�`��摜
    float4 srcCol = tex.Sample(texSampler, input.uv);
    
    //�m�C�Y�摜
    float4 noiseCol =
		noiseTex.Sample(texSampler, input.uv);


    //�m�C�Y�摜�̐F�̕���(�O�`�P)
    float alpha = (noiseCol.r + noiseCol.g + noiseCol.b) / 3.0f;
    //�P�`�Q
    alpha += 1.0f;
    alpha -= g_time / dissolveSpeed;
    
    //����������������Ă���
    if (alpha < discardValue)
    {
        discard;
    }
    //�ʏ�`��
    if (alpha > 1.0f)
    {
        return srcCol;
    }
    
        
    //����
    float par = alpha - discardValue;
    srcCol.rgb = lerp(redColor, blockColor, par * lerpAdjustmentValue);
    
    return srcCol;
}