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
	float g_per;
}

//�`�悷��e�N�X�`��
Texture2D g_SrcTexture:register(t0);

//�T���v���[�F�K�؂ȐF�����߂�
SamplerState g_SrcSampler:register(s0);

float4 main(PS_INPUT PSInput) : SV_TARGET
{
	//UV���W���󂯎��
	float2 uv = PSInput.TexCoords0;

	//UV���W�ƃe�N�X�`�����Q�Ƃ��āA�œK�ȐF���擾����
	float4 srcCol =
		g_SrcTexture.Sample(g_SrcSampler, uv);



	//�V���v���Z�s�A
	float3 sepia = srcCol.rgb;
	sepia.r = dot(srcCol.rgb, float3(0.393f, 0.769f, 0.189f));
	sepia.g = dot(srcCol.rgb, float3(0.349f, 0.686f, 0.168f));
	sepia.b = dot(srcCol.rgb, float3(0.272f, 0.534f, 0.131f));

	//���`��ԁi�F�j
	srcCol.rgb = lerp(srcCol.rbg, sepia, saturate(g_per));


	return srcCol;
}