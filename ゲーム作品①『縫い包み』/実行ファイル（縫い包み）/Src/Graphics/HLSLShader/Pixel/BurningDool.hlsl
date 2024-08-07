#include "../Common/VertexToPixelTypeHeader.hlsli"
#define VERTEX_TO_PIXEL_TYPE (VERTEX_TO_PIXEL_TYPE_NORMAL_MAP)
#define BUMPMAP
#include "PixelShader3DHeader.hlsli"

SamplerState subSampler : register(s2); //  ハイトマップ
Texture2D subTexture : register(t2); // ハイトマップ

//ディゾルブ開始値（アルファ値に加算）
static float dissolveStartValue = 0.8;
//小数点切り捨て値
static float runcateDecimalValue = 80.0f;
//変化色
static float3 blockColor = float3(0.05f, 0.05f, 0.05f);      //黒
static float3 redColor = float3(1.0f, 0.05f, 0.05f);         //赤
static float3 orangeColor = float3(1.0f, 0.8f, 0.0f);        //橙
static float3 whiteColor = float3(1.0f, 1.0f, 1.0f);         //白
//延焼部分の色変化時間    
static float burnColorChangeTime6 = 0.7f;   //黒
static float burnColorChangeTime5 = 0.71f;  //白
static float burnColorChangeTime4 = 0.73f;  //オレンジ→白
static float burnColorChangeTime3 = 0.76f;  //赤→オレンジ
static float burnColorChangeTime2 = 0.8f;   //黒→赤
static float burnColorChangeTime1 = 0.85f;  //燃え始め(黒)

//定数バッファ：スロット番号4番目（b4）
cbuffer cbParam : register(b4)
{
    float g_totalTime;
    float g_finishTime;
    float2 paddhing;
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

    //ハイトマップ
    float4 heightCol = subTexture.Sample(subSampler, input.uv);
    
    //ハイトマップ画像の色の平均(０～１)
    float alpha = (heightCol.r + heightCol.g + heightCol.b) / 3.0f;
    alpha += dissolveStartValue;
    //浸食割合
    alpha -= g_totalTime / g_finishTime;
    //焼却線を滑らかにする（小数点をまとめる）
    alpha = floor(alpha * runcateDecimalValue);
    alpha /= runcateDecimalValue;

    //延焼部分
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

    //0～1に揃えて返す
    return saturate(result);
}
