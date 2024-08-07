#include "../Common/VertexToPixelTypeHeader.hlsli"
#define VERTEX_TO_PIXEL_TYPE (VERTEX_TO_PIXEL_TYPE_NORMAL_MAP)
#define BUMPMAP
#include "PixelShader3DHeader.hlsli"

//�萔�o�b�t�@
cbuffer cbParam : register(b4)
{
    float g_scrollTime;
    float g_dissolveTime;
}

SamplerState heightMapSampler : register(s2); // �n�C�g�}�b�v�e�N�X�`��
Texture2D heightMapTexture : register(t2); // �n�C�g�}�b�v�u�e�N�X�`��

//�f�B�]���u
static float dissolveTimeSlowValue = 3.0f;    //�o�ߎ��Ԓx�点�l
static float disappearValue = 1.05f;          //������臒l
static float aboutDisappearValue = 1.1f;      //����������臒l
static float4 aboutDisappearColor = float4(0.5f, 0.5f, 0.5f, 1.0f);     //���������F

PS_OUTPUT main(PS_INPUT input) : SV_TARGET
{
    float4 result = 0.0f;
    
    //�f�B�]���u
    if (g_dissolveTime > 0.0f)
    {
        float2 uv = input.uv;
        float4 dissolveCol = heightMapTexture.Sample(heightMapSampler,uv );
        float alpha = (dissolveCol.x + dissolveCol.y + dissolveCol.z) / 3.0f;   //���ϒl
        alpha += 1.0f; //1�`2
        alpha -= g_dissolveTime / dissolveTimeSlowValue;

        if (alpha <= disappearValue)
        {
            discard;
        }
        else if (alpha <= aboutDisappearValue)
        {
            return aboutDisappearColor;
        }
    }
    
    
    //uv�X�N���[��
    float2 scrollUv = input.uv;
    float scrollSpeed = 0.02f;
    scrollUv += sin(g_scrollTime * scrollSpeed);
    scrollUv.x = frac(scrollUv.x - (g_scrollTime * scrollSpeed));
    
    //�e�N�X�`��������ꍇ�͂�����l�������������s��
    uint2 diffuseMapSize;
    diffuseMapTexture.GetDimensions(diffuseMapSize.x, diffuseMapSize.y);
    if (diffuseMapSize.x * diffuseMapSize.y > 0)
    {
        float4 heightMapCol = diffuseMapTexture.Sample(diffuseMapSampler, scrollUv);
        if (heightMapCol.a == 0.0f)
        {
            discard;
        }
        result = heightMapCol;
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
    
    //rgb�𔽓]
    result.rgb = 1.0f - saturate(result.rgb);

    //�t�H�O�̓K�p
    result.rgb *= input.fogFactor;

    
    return saturate(result);
}