//�s�N�Z���V�F�[�_�[�̓���
struct PS_INPUT
{
	//���W�i�v���W�F�N�V������ԁj
    float4 Position : SV_POSITION;
	//�f�B�t�[�Y�J���[
    float4 Diffuse : COLORO;
	//�e�N�X�`�����W
    float2 TexCoords0 : TEXCOORD0;
};

//�`�悷��e�N�X�`��
Texture2D g_SrcTexture : register(t0);

//�T���v���[�F�K�؂ȐF�����߂�
SamplerState g_SrcSampler : register(s0);

//�萔�o�b�t�@
cbuffer cbParam : register(b3)
{
    float g_time;
    float g_slow;
}


float4 main(PS_INPUT PSInput) : SV_TARGET
{
    //�^�C�����O�̏ꍇ�A�\�����Ȃ�
    if (g_time<0)
    {
        discard;
    }

	//UV���W���󂯎��
    float2 uv = PSInput.TexCoords0;
    float4 srcCol =
		g_SrcTexture.Sample(g_SrcSampler, uv);

    float pro = g_time / g_slow;
    pro = min(pro, 1.0f);

    //���`���
    float3 result = lerp(1.0f, srcCol. rgb, pro);


    return float4(result, srcCol.a);
}