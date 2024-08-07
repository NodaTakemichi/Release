#include "../Common/VertexToPixelTypeHeader.hlsli"
#define VERTEX_TO_PIXEL_TYPE (VERTEX_TO_PIXEL_TYPE_NORMAL_MAP)
#define BUMPMAP
#include "PixelShader3DHeader.hlsli"

cbuffer CBuffer : register(b4)
{
    float4 cameraPos;
    matrix vmat;
}

Texture2D roughnessMapTexture : register(t2);           // ラフネステクスチャ
Texture2D metallicTexture : register(t3);               // メタリックテクスチャ
Texture2D sphereMapTexture : register(t4);              // スフィアマップテクスチャ

PS_OUTPUT main(PS_INPUT input) : SV_TARGET
{
    
    float4 result = float4(1.0f, 1.0f, 1.0f, 1.0f);

    //テクスチャがある場合はそれを考慮した処理を行う
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

    //法線マップから色を取得し、0～1を-1～1に変換
    const float3 tanNormal = normalize(normalMapTexture.Sample(normalMapSampler, input.uv).xyz * 2 - 1);

    //カメラ→自身のベクトル
    const float3 ray = normalize(input.viewPos);
    //ライトベクトル
    const float3 light = ray;

    //接ベクトル空間→ビュー空間に法線を変換
    const float3 normal = ConvertCoordinateSpace(tanNormal, input.viewTan, input.viewBin, input.viewNorm);
    
    //ライティング処理での合計色
    float3 totalDiffuse = float3(0, 0, 0);
    float3 totalSpecular = float3(0, 0, 0);

    //ライトごとにライティング処理
    for (int i = 0; i < DX_D3D11_COMMON_CONST_LIGHT_NUM; i++)
    {
        ProcessLight(input.viewPos, ray, normal, common.light[i],
        common.material.power, totalDiffuse, totalSpecular);
    }

    //環境光を適用
    totalDiffuse += common.material.ambientEmissive.rgb;

    //明度にマテリアルの色を適用
    totalDiffuse *= common.material.diffuse.rgb;
    totalSpecular *= common.material.specular.rgb;

    //元の色にマテリアルを適用
    result.rgb *= totalDiffuse;
    result.rgb += totalSpecular;
    result.a *= common.material.diffuse.a * base.factorColor.a;

    //加算色を適用
    result.rgb += base.drawAddColor.rgb;

    
    //視線の反射ベクトル
    float3 refRay = reflect(ray, normal);
    //カメラ方向を考慮して視線ベクトルを回転
    refRay = mul(refRay, vmat);
    //反射ベクトルからスフィアマップ用UVを作成(範囲の正規化)
    refRay.xy = refRay.xy * float2(0.5, -0.5) + 0.5f;
    
    float4 spCol = sphereMapTexture.Sample(diffuseMapSampler, refRay.xy);       //スフィアマップ
    float rough = roughnessMapTexture.Sample(diffuseMapSampler, input.uv).r;    //ラフネス：R
    float metalic = metallicTexture.Sample(diffuseMapSampler, input.uv);        //メタリック

    float b = saturate(dot(normal, -light));
    float3 rgb = saturate(result.rgb);
    //カラー
    float3 color = lerp(totalSpecular, rgb * b, rough);
    //反射カラー
    float3 refCol = lerp(lerp(1, spCol.rgb, metalic) * rgb, 0, rough);
    result.rgb = color + refCol;
    
    //フォグの適用
    result.rgb *= input.fogFactor;
    return saturate(result);
}
