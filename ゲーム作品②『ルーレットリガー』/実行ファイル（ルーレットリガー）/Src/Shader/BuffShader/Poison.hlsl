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

//模様玉
static float growthRate = 0.4f; //成長率
static float upSpeed = 0.2f; //上昇速度

//玉：一個目
static float2 genPos1 = float2(0.8f, 0.5f); //発生座標
static float genTime1 = 0.0f; //発生時間
//玉：二個目
static float2 genPos2 = float2(0.3f, 0.8f); //発生座標
static float genTime2 = 0.8f; //発生時間
//玉：三個目
static float2 genPos3 = float2(0.6f, 0.6f); //発生座標
static float genTime3 = 1.4f; //発生時間
//玉：四個目
static float2 genPos4 = float2(0.2f, 0.8f); //発生座標
static float genTime4 = 1.8f; //発生時間

float4 PosCircle(float2 uv, float2 pos, float genTime)
{
	//発生時間
	float gTime = g_time - genTime;

	//進行度で判断する
	float per = gTime * 0.5f;
	//1以上にならない
	if (per >= 1.0f || 0.0f >= per)
	{
		return float4(0.0f, 0.0f, 0.0f, 0.0f);
	}

	// 上昇
	pos.y -= gTime > 0.0f ? upSpeed * gTime : 0.0f;

	//座標差
	float2 disPos = uv - pos;
	//円の半径
	float circlePer = sin(per * 3.0f) / 3.0f;

	//反転
	float radius = growthRate * circlePer;
	//円の範囲内
	float alpha = saturate(1.0f - length(disPos) / radius);
	//円の色
	float4 circle = float4(0.64f, 0.218f, 1.0f, alpha * 1.9f);


	//光線と座標位置のベクトル計算
	float2 light = float2(0.0f, -1.0f);
	//内積
	float d = dot(disPos, light);
	//半径から距離が、遠ければ薄くする
	float len = saturate(radius * (0.7f + d));
	circle.rg -= length(disPos) >= len ? float2(0.25f, 0.1f) : 0.0f;

	return circle;

}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
	//UV座標とテクスチャを参照して、最適な色を取得する
	float2 uv = PSInput.TexCoords0;
	float2 revers = float2(abs(g_revers - uv.x), uv.y);
	float4 srcCol =
		g_SrcTexture.Sample(g_SrcSampler, revers);

	//画像色の進行度
    float pro = sin(g_time * g_compTime);
	//紫色（毒)
	float3 poi = float3(0.54f, 0.168f, 1.0f);
	//色の線形補間
	float3 disCol = lerp(srcCol.rgb, poi.rgb, min(pro * 2.5f, 2.0f));

	//玉模様
	float4 circle = PosCircle(uv, genPos1, genTime1);

	//入れ物
	float4 c = float4(0.0f, 0.0f, 0.0f, 0.0f);

	//2
	c = PosCircle(uv, genPos2, genTime2);
	circle = c.a > circle.a ? c : circle;
	//3
	c = PosCircle(uv, genPos3, genTime3);
	circle = c.a > circle.a ? c : circle;
	//4
	c = PosCircle(uv, genPos4, genTime4);
	circle = c.a > circle.a ? c : circle;


	//結果
	disCol.rgb = circle.a > 0.0f ? circle.rgb : disCol.rgb;


	return float4 (disCol, max(srcCol.a, circle.a));
}