#include "../Common/VertexToPixelTypeHeader.hlsli"
#define VERTEX_TO_PIXEL_TYPE (VERTEX_TO_PIXEL_TYPE_NORMAL_MAP)
#define BUMPMAP
#include "PixelShader3DHeader.hlsli"

cbuffer CBuffer : register(b4)
{
    float4 cameraPos;
    matrix vmat;
}

Texture2D roughnessMapTexture : register(t2);           // ���t�l�X�e�N�X�`��
Texture2D metallicTexture : register(t3);               // ���^���b�N�e�N�X�`��
Texture2D sphereMapTexture : register(t4);              // �X�t�B�A�}�b�v�e�N�X�`��

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

    //�@���}�b�v����F���擾���A0�`1��-1�`1�ɕϊ�
    const float3 tanNormal = normalize(normalMapTexture.Sample(normalMapSampler, input.uv).xyz * 2 - 1);

    //�J���������g�̃x�N�g��
    const float3 ray = normalize(input.viewPos);
    //���C�g�x�N�g��
    const float3 light = ray;

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

    
    //�����̔��˃x�N�g��
    float3 refRay = reflect(ray, normal);
    //�J�����������l�����Ď����x�N�g������]
    refRay = mul(refRay, vmat);
    //���˃x�N�g������X�t�B�A�}�b�v�pUV���쐬(�͈͂̐��K��)
    refRay.xy = refRay.xy * float2(0.5, -0.5) + 0.5f;
    
    float4 spCol = sphereMapTexture.Sample(diffuseMapSampler, refRay.xy);       //�X�t�B�A�}�b�v
    float rough = roughnessMapTexture.Sample(diffuseMapSampler, input.uv).r;    //���t�l�X�FR
    float metalic = metallicTexture.Sample(diffuseMapSampler, input.uv);        //���^���b�N

    float b = saturate(dot(normal, -light));
    float3 rgb = saturate(result.rgb);
    //�J���[
    float3 color = lerp(totalSpecular, rgb * b, rough);
    //���˃J���[
    float3 refCol = lerp(lerp(1, spCol.rgb, metalic) * rgb, 0, rough);
    result.rgb = color + refCol;
    
    //�t�H�O�̓K�p
    result.rgb *= input.fogFactor;
    return saturate(result);
}
