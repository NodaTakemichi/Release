#include "../Common/VertexToPixelTypeHeader.hlsli"
#define VERTEX_TO_PIXEL_TYPE (VERTEX_TO_PIXEL_TYPE_NORMAL_MAP)
#define BUMPMAP
#include "PixelShader3DHeader.hlsli"

//定数バッファ
cbuffer cbParam : register(b4)
{
    float g_scrollTime;
    float g_dissolveTime;
}

SamplerState heightMapSampler : register(s2); // ハイトマップテクスチャ
Texture2D heightMapTexture : register(t2); // ハイトマップブテクスチャ

//ディゾルブ
static float dissolveTimeSlowValue = 3.0f;    //経過時間遅らせ値
static float disappearValue = 1.05f;          //消える閾値
static float aboutDisappearValue = 1.1f;      //消えかける閾値
static float4 aboutDisappearColor = float4(0.5f, 0.5f, 0.5f, 1.0f);     //消えかけ色

PS_OUTPUT main(PS_INPUT input) : SV_TARGET
{
    float4 result = 0.0f;
    
    //ディゾルブ
    if (g_dissolveTime > 0.0f)
    {
        float2 uv = input.uv;
        float4 dissolveCol = heightMapTexture.Sample(heightMapSampler,uv );
        float alpha = (dissolveCol.x + dissolveCol.y + dissolveCol.z) / 3.0f;   //平均値
        alpha += 1.0f; //1～2
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
    
    
    //uvスクロール
    float2 scrollUv = input.uv;
    float scrollSpeed = 0.02f;
    scrollUv += sin(g_scrollTime * scrollSpeed);
    scrollUv.x = frac(scrollUv.x - (g_scrollTime * scrollSpeed));
    
    //テクスチャがある場合はそれを考慮した処理を行う
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

    //元の色にマテリアルを適用
    result.rgb *= totalDiffuse;
    result.rgb += totalSpecular;
    result.a *= common.material.diffuse.a * base.factorColor.a;

    //加算色を適用
    result.rgb += base.drawAddColor.rgb;
    
    //rgbを反転
    result.rgb = 1.0f - saturate(result.rgb);

    //フォグの適用
    result.rgb *= input.fogFactor;

    
    return saturate(result);
}