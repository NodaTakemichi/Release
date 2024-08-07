//�s�N�Z���V�F�[�_�[�̓���
struct PS_INPUT
{
	//���W�i�v���W�F�N�V������ԁj
	float4 Position			:SV_POSITION;
	//�f�B�t�[�Y�J���[
	float4 Diffuse			:COLORO;
	//�e�N�X�`�����W
	float2 TexCoords0		:TEXCOORD0;

};

//�萔�o�b�t�@�F�X���b�g�ԍ�3�Ԗځib3�j
cbuffer cbParam : register(b3)
{
	float g_time;
}

//�`�悷��e�N�X�`��
Texture2D g_SrcTexture:register(t0);

//�T���v���[�F�K�؂ȐF�����߂�
SamplerState g_SrcSampler:register(s0);

float4 main(PS_INPUT PSInput) : SV_TARGET
{
	//UV���W
	float2 uv = PSInput.TexCoords0;


	//����
    float xSin = sin((uv.x + (g_time / 2)) * 7.0f);
	// ���Ԍo�߂ɉ����ĉ摜��y���W��ω�������
	float amplitube = 4.5f;
	float offsetY = sin(g_time * amplitube) * 0.1f * xSin; // 5.0f�͐U���𒲐�����l

	// �c�񂾃e�N�X�`�����W�Ńe�N�X�`�����T���v�����O���ďo��
	float2 distortedTexCoord = uv + float2(0, offsetY);

	float4 srcCol = g_SrcTexture.Sample(g_SrcSampler, distortedTexCoord);

	return srcCol;
}