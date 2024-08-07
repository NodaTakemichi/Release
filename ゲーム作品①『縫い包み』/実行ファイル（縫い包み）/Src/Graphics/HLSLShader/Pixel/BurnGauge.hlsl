#include"PixelShader2DHeader.hlsli"

//�萔�o�b�t�@�F�X���b�g�ԍ�4�Ԗځib4�j
cbuffer cbParam : register(b4)
{
    float g_par;    //�Q�[�W����
    float2 g_size;  //�摜�T�C�Y
    float padding;
}

//�x�C���[�z��
static const int BayerPattern[16] =
{
     0, 8, 2, 10,
	12, 4, 14, 6,
	 3, 11, 1, 9,
	15, 7, 13, 5
};

Texture2D maskTex : register(t1); //�}�X�N�摜
Texture2D frameTex : register(t2); //�t���[���摜

PS_OUTPUT main(PS_INPUT input) : SV_TARGET
{
    float4 result = float4(0.0f, 0.0f, 0.0f, 0.0f);

    //�t���[���摜
    float4 frameCol = frameTex.Sample(texSampler, input.uv);
    if (frameCol.a <= 0.0f)
    {
        discard;
    }
    result = frameCol;
    

    //���C���摜
    float4 mainCol = tex.Sample(texSampler, input.uv);
    //�}�X�N�摜
    float4 maskCol = maskTex.Sample(texSampler, input.uv);
    
    //�p�[�Z���g�Q�[�W�ȉ��@���@�}�X�N�摜��@�̎�
    if (input.uv.x <= g_par && maskCol.a >= 0.1f)
    {
        result = mainCol;
    }  
    
    return result;
}