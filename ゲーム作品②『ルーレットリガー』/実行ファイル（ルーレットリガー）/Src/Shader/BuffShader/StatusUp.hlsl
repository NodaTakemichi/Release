//ピクセルシェーダーの入力
struct PS_INPUT
{
	//座標（プロジェクション空間）
    float4 Position : SV_POSITION;
	//ディフーズカラー
    float4 Diffuse : COLORO;
	//テクスチャ座標
    float2 TexCoords0 : TEXCOORD0;

};

static float triangleHight = 0.4f;	//山の高さ
static float triangleNum = 4.0f;	//山の数
static float3 topColor = float3(1.0f, 0.6f, 0.0f);		//橙色
static float3 bottomColor = float3(1.0f, 0.9f, 0.0f);	//黄色
static float moveSpeed = 0.8f;				//移動スピード
static float verticalSpiltNum = 5.0f;		//縦の分割数
static float luminance = 1.2f;		//輝度
static float lowestSrcColorValue = 0.2f;	//画像カラーの最低値


//定数バッファ：スロット番号3番目（b3）
cbuffer cbParam : register(b3)
{
    float g_revers;
    float g_time;
    float g_compTime;
}

//描画するテクスチャ
Texture2D g_SrcTexture : register(t0);

//サンプラー：適切な色を決める
SamplerState g_SrcSampler : register(s0);

float4 main(PS_INPUT PSInput) : SV_TARGET
{

	//UV座標とテクスチャを参照して、最適な色を取得する
    float2 uv = PSInput.TexCoords0;
    float2 revers = float2(abs(g_revers - uv.x), uv.y);
    float4 srcCol =
		g_SrcTexture.Sample(g_SrcSampler, revers);

	//山線
	float hight = triangleHight;		//山の高さ
	uv.x = frac(uv.x * triangleNum);
	//反転（真ん中で反転）
	hight *= uv.x < 0.5f ? uv.x : 0.5f - uv.x;
	uv.y += hight;

	//グラデーション線
	//２色の差分を求める
	float3 c = abs(bottomColor.rgb - topColor.rgb);

	//線を動かす
	uv.y += frac(g_time * moveSpeed);	//スピード
	float grad = frac(uv.y * verticalSpiltNum);//横に分割
	c.rgb *= grad;

	//最低色を加算
	c += topColor;

	//時間経過で薄くなる
    float pro = 1.0f - sin(g_time * g_compTime);
    pro = max(pro, lowestSrcColorValue);
	//最終結果の差分
	float3 dec = srcCol.rgb - c;
	//差分を加算する
    float3 result = c + (dec * pro);

    return float4(result, srcCol.a) * luminance;
}
