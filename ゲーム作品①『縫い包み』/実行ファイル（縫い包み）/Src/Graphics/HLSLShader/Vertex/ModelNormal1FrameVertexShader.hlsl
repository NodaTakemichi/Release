#include "../Common/VertexToPixelTypeHeader.hlsli"
#define VERTEX_INPUT_TYPE (VERTEX_INPUT_TYPE_NMAP_1FRAME)
#define VERTEX_TO_PIXEL_TYPE (VERTEX_TO_PIXEL_TYPE_NORMAL_MAP)
#include "VertexShader3DHeader.hlsli"

cbuffer CBuffer : register(b7)
{
    float3 g_camera_pos;
    float g_fog_start;
    float g_fog_end;
    float3 padding;
}

//エントリーポイント
VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT ret;
    ret.svPos = float4(input.pos, 1.0f);

    //ローカル座標系→ワールド座標系
    ret.worldPos = mul(ret.svPos, base.localWorldMatrix);
    ret.svPos.xyz = ret.worldPos;

    //ワールド座標系→ビュー座標系
    ret.viewPos = mul(ret.svPos, base.viewMatrix);
    ret.svPos.xyz = ret.viewPos;

    //ビュー座標系→プロジェクション座標系
    ret.svPos = mul(ret.svPos, base.projectionMatrix);

    //接線(ビュー)を計算
    ret.viewTan = normalize(mul(float4(input.tan, 0.0f), base.localWorldMatrix));
    ret.viewTan = normalize(mul(float4(ret.viewTan, 0.0f), base.viewMatrix));

    //従法線(ビュー)を計算
    ret.viewBin = normalize(mul(float4(input.bin, 0.0f), base.localWorldMatrix));
    ret.viewBin = normalize(mul(float4(ret.viewBin, 0.0f), base.viewMatrix));

    //法線(ビュー)を計算
    ret.viewNorm = normalize(mul(float4(input.norm, 0.0f), base.localWorldMatrix));
    ret.viewNorm = normalize(mul(float4(ret.viewNorm, 0.0f), base.viewMatrix));

    ret.uv = input.uv0.xy; //UVを代入
    ret.diffuse = input.diffuse; //拡散反射色を代入
    ret.specular = input.specular; //鏡面反射色を代入

    //フォグの強さ
    ret.fogFactor = CalculateFogFactor(ret.worldPos.xyz, g_camera_pos, g_fog_start, g_fog_end);

    return ret;
}