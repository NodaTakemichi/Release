#include "../Common/VertexToPixelTypeHeader.hlsli"
#define VERTEX_TO_PIXEL_TYPE (VERTEX_TO_PIXEL_TYPE_NORMAL_MAP)
#define BUMPMAP
#include "PixelShader3DHeader.hlsli"

cbuffer CBuffer : register(b4)
{
    float3 action_pos;
    float progress_time;
    float lumi_dis_time;    //発行距離時間
    float lumi_dis_area;    //発行距離範囲
}

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

    ////元の色にマテリアルを適用
    result.rgb *= totalDiffuse;
    result.rgb += totalSpecular;
    result.a *= common.material.diffuse.a * base.factorColor.a;

    //加算色を適用
    result.rgb += base.drawAddColor.rgb;

    //フォグを適用
    result.rgb *= input.fogFactor;
    
    //ワールド座標とプレイヤー座標の差分
    float dis = distance(input.worldPos, action_pos);
    //発光位置
    float lightingDis = progress_time * lumi_dis_time;
    //ポリゴンが光源から離れている距離
    float lightDis = abs(distance(lightingDis, dis));
    //発光範囲距離
    float lumiArea = lumi_dis_area;

    if (lightDis < lumiArea)
    {
        //発行色
        float3 lightCol = float3(1.0f,1.0f,1.0f);
        float3 zeroCol = float3(0.0f, 0.0f, 0.0f);

        //発光度
        float lumiPow = (lumiArea - lightDis) / lumiArea;
        //ライト色の加算
        result.rgb = lerp(zeroCol, lightCol, lumiPow);
    }
    

    
    //0～1に揃えて返す
    return saturate(result);
}
