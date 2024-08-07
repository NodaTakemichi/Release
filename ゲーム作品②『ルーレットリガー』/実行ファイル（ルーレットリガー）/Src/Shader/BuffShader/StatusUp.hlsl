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

static float triangleHight = 0.4f;	//�R�̍���
static float triangleNum = 4.0f;	//�R�̐�
static float3 topColor = float3(1.0f, 0.6f, 0.0f);		//��F
static float3 bottomColor = float3(1.0f, 0.9f, 0.0f);	//���F
static float moveSpeed = 0.8f;				//�ړ��X�s�[�h
static float verticalSpiltNum = 5.0f;		//�c�̕�����
static float luminance = 1.2f;		//�P�x
static float lowestSrcColorValue = 0.2f;	//�摜�J���[�̍Œ�l


//�萔�o�b�t�@�F�X���b�g�ԍ�3�Ԗځib3�j
cbuffer cbParam : register(b3)
{
    float g_revers;
    float g_time;
    float g_compTime;
}

//�`�悷��e�N�X�`��
Texture2D g_SrcTexture : register(t0);

//�T���v���[�F�K�؂ȐF�����߂�
SamplerState g_SrcSampler : register(s0);

float4 main(PS_INPUT PSInput) : SV_TARGET
{

	//UV���W�ƃe�N�X�`�����Q�Ƃ��āA�œK�ȐF���擾����
    float2 uv = PSInput.TexCoords0;
    float2 revers = float2(abs(g_revers - uv.x), uv.y);
    float4 srcCol =
		g_SrcTexture.Sample(g_SrcSampler, revers);

	//�R��
	float hight = triangleHight;		//�R�̍���
	uv.x = frac(uv.x * triangleNum);
	//���]�i�^�񒆂Ŕ��]�j
	hight *= uv.x < 0.5f ? uv.x : 0.5f - uv.x;
	uv.y += hight;

	//�O���f�[�V������
	//�Q�F�̍��������߂�
	float3 c = abs(bottomColor.rgb - topColor.rgb);

	//���𓮂���
	uv.y += frac(g_time * moveSpeed);	//�X�s�[�h
	float grad = frac(uv.y * verticalSpiltNum);//���ɕ���
	c.rgb *= grad;

	//�Œ�F�����Z
	c += topColor;

	//���Ԍo�߂Ŕ����Ȃ�
    float pro = 1.0f - sin(g_time * g_compTime);
    pro = max(pro, lowestSrcColorValue);
	//�ŏI���ʂ̍���
	float3 dec = srcCol.rgb - c;
	//���������Z����
    float3 result = c + (dec * pro);

    return float4(result, srcCol.a) * luminance;
}
