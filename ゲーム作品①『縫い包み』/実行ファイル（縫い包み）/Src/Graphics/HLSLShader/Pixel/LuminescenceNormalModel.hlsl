#include "../Common/VertexToPixelTypeHeader.hlsli"
#define VERTEX_TO_PIXEL_TYPE (VERTEX_TO_PIXEL_TYPE_NORMAL_MAP)
#define BUMPMAP
#include "PixelShader3DHeader.hlsli"

cbuffer CBuffer : register(b4)
{
    float3 action_pos;
    float progress_time;
    float lumi_dis_time;    //���s��������
    float lumi_dis_area;    //���s�����͈�
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

    ////���̐F�Ƀ}�e���A����K�p
    result.rgb *= totalDiffuse;
    result.rgb += totalSpecular;
    result.a *= common.material.diffuse.a * base.factorColor.a;

    //���Z�F��K�p
    result.rgb += base.drawAddColor.rgb;

    //�t�H�O��K�p
    result.rgb *= input.fogFactor;
    
    //���[���h���W�ƃv���C���[���W�̍���
    float dis = distance(input.worldPos, action_pos);
    //�����ʒu
    float lightingDis = progress_time * lumi_dis_time;
    //�|���S�����������痣��Ă��鋗��
    float lightDis = abs(distance(lightingDis, dis));
    //�����͈͋���
    float lumiArea = lumi_dis_area;

    if (lightDis < lumiArea)
    {
        //���s�F
        float3 lightCol = float3(1.0f,1.0f,1.0f);
        float3 zeroCol = float3(0.0f, 0.0f, 0.0f);

        //�����x
        float lumiPow = (lumiArea - lightDis) / lumiArea;
        //���C�g�F�̉��Z
        result.rgb = lerp(zeroCol, lightCol, lumiPow);
    }
    

    
    //0�`1�ɑ����ĕԂ�
    return saturate(result);
}
