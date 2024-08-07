#include "../Common/VertexToPixelTypeHeader.hlsli"
#define VERTEX_TO_PIXEL_TYPE (VERTEX_TO_PIXEL_TYPE_NORMAL_MAP)
#define BUMPMAP
#include "PixelShader3DHeader.hlsli"

SamplerState subSampler : register(s2); //  �n�C�g�}�b�v
Texture2D subTexture : register(t2); // �n�C�g�}�b�v

//�f�B�]���u�J�n�l�i�A���t�@�l�ɉ��Z�j
static float dissolveStartValue = 0.8;
//�����_�؂�̂Ēl
static float runcateDecimalValue = 80.0f;
//�ω��F
static float3 blockColor = float3(0.05f, 0.05f, 0.05f);      //��
static float3 redColor = float3(1.0f, 0.05f, 0.05f);         //��
static float3 orangeColor = float3(1.0f, 0.8f, 0.0f);        //��
static float3 whiteColor = float3(1.0f, 1.0f, 1.0f);         //��
//���ĕ����̐F�ω�����    
static float burnColorChangeTime6 = 0.7f;   //��
static float burnColorChangeTime5 = 0.71f;  //��
static float burnColorChangeTime4 = 0.73f;  //�I�����W����
static float burnColorChangeTime3 = 0.76f;  //�ԁ��I�����W
static float burnColorChangeTime2 = 0.8f;   //������
static float burnColorChangeTime1 = 0.85f;  //�R���n��(��)

//�萔�o�b�t�@�F�X���b�g�ԍ�4�Ԗځib4�j
cbuffer cbParam : register(b4)
{
    float g_totalTime;
    float g_finishTime;
    float2 paddhing;
}

PS_OUTPUT main(PS_INPUT input) : SV_TARGET
{
    float4 result = float4(1.0f, 1.0f, 1.0f, 1.0f);

    //�e�N�X�`��������ꍇ�͂�����l�������������s��
    uint2 diffuseMapSize;
    diffuseMapTexture.GetDimensions(diffuseMapSize.x, diffuseMapSize.y);
    if (diffuseMapSize.x * diffuseMapSize.y > 0)
    {
        float4 texColor = diffuseMapTexture.Sample(diffuseMapSampler, input.uv);
        if (texColor.a == 0.0f)
        {
            discard;
        }
        
        result *= texColor;
    }

    //�n�C�g�}�b�v
    float4 heightCol = subTexture.Sample(subSampler, input.uv);
    
    //�n�C�g�}�b�v�摜�̐F�̕���(�O�`�P)
    float alpha = (heightCol.r + heightCol.g + heightCol.b) / 3.0f;
    alpha += dissolveStartValue;
    //�Z�H����
    alpha -= g_totalTime / g_finishTime;
    //�ċp�������炩�ɂ���i�����_���܂Ƃ߂�j
    alpha = floor(alpha * runcateDecimalValue);
    alpha /= runcateDecimalValue;

    //���ĕ���
    if (alpha < burnColorChangeTime6)
    {
        result.rgb *= blockColor;
        //discard;
    }
    else if (alpha <= burnColorChangeTime5)
    {
        result.rgb = whiteColor;
    }
    else if (alpha <= burnColorChangeTime4)
    {
        float par = (burnColorChangeTime4 - alpha) / (burnColorChangeTime4 - burnColorChangeTime5);
        result.rgb = lerp(whiteColor, orangeColor, 1-par);
    }
    else if (alpha <= burnColorChangeTime3)
    {
        float par = (burnColorChangeTime3 - alpha) / (burnColorChangeTime3 - burnColorChangeTime4);
        result.rgb = lerp(orangeColor, redColor, 1-par);
    }
    else if (alpha <= burnColorChangeTime2)
    {
        float par = (burnColorChangeTime2 - alpha) / (burnColorChangeTime2 - burnColorChangeTime3);
        result.rgb = lerp(redColor, blockColor, 1-par);
    }
    else if (alpha <= burnColorChangeTime1)
    {
        result.rgb *= blockColor;
    }

    
    //�@���}�b�v����F���擾���A0�`1��-1�`1�ɕϊ�
    const float3 tanNormal = normalize(normalMapTexture.Sample(normalMapSampler, input.uv).xyz * 2 - 1);

    //�J���������g�̃x�N�g��
    const float3 ray = normalize(input.viewPos);

    //�ڃx�N�g����ԁ��r���[��Ԃɖ@����ϊ�
    const float3 normal = ConvertCoordinateSpace(tanNormal, input.viewTan, input.viewBin, input.viewNorm);
    
    //���C�e�B���O�����ł̍��v�F
    float3 totalDiffuse = float3(0, 0, 0);
    float3 totalSpecular = float3(0, 0, 0);

    //���C�g���ƂɃ��C�e�B���O����
    for (int i = 0; i < DX_D3D11_COMMON_CONST_LIGHT_NUM; i++)
    {
        ProcessLight(input.viewPos, ray, normal, common.light[i],
        common.material.power, totalDiffuse, totalSpecular);
    }

    //������K�p
    totalDiffuse += common.material.ambientEmissive.rgb;

    //���x�Ƀ}�e���A���̐F��K�p
    totalDiffuse *= common.material.diffuse.rgb;
    totalSpecular *= common.material.specular.rgb;

    //���̐F�Ƀ}�e���A����K�p
    result.rgb *= totalDiffuse;
    result.rgb += totalSpecular;
    result.a *= common.material.diffuse.a * base.factorColor.a;

    //���Z�F��K�p
    result.rgb += base.drawAddColor.rgb;

    //0�`1�ɑ����ĕԂ�
    return saturate(result);
}
