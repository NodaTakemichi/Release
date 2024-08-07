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

//符号なし整数型の最大値
static const uint UINT_MAX = 0xffffffffu;

uint XorShift(uint n)
{
	n ^= (n << 13);
	n ^= (n >> 17);
	n ^= (n << 15);
	return n;
}

float noise(float2 p)
{
	return float(XorShift(asuint(p.x + p.y))) / float(UINT_MAX);
}

float CreateLine(
	float2 currentTempPos, float2 currentStraightPos,
	float2 size, float2 uv)
{
	// 本来の雷ポジションから補正後のポジションへのベクトル
	float2 lab = currentTempPos - currentStraightPos;
	// 補正後ポジションから処理中のuv座標へのベクトル
	float2 la = uv - currentTempPos;
	// 本来のポジションから処理中uv座標へのベクトル
	float2 lb = uv - currentStraightPos;
	// +1だと点になる
	float2 d = (length(la) + length(lb) - length(lab)) * min(length(la), length(lb));
	// pow(x,y) xのy乗を返す
	return size - pow(d * 4e8, 0.07);
}

float Lightning(float2 uv, float2 sPos, float2 ePos)
{
	float s = 0.0f;
	int num = 10;
	// 同時に表示する雷の本数
	int lightningNum = 4;
	for (int j = 1; j < lightningNum; j++)
	{
		// 雷の先端ポジション
		float2 temp = sPos;
		// 分割した雷の1つ分の距離
		float2 d = (ePos - sPos) / num;
		for (int i = 1; i < num; i++)
		{
			float2 currentPos = sPos + d * i;
			if (i == j * 2)
			{
				// 雷の動きを激しくさせるランダム要素
				d += (noise(float2(j, j) + currentPos) - 0.5) *
					noise(currentPos) * 0.025;
			}
			currentPos += (noise(currentPos) - 0.5) * d.y;
			float l = CreateLine(
				temp, currentPos, j * 0.25 + 2.0, uv);
			s = max(l, s);

			temp = currentPos;
		}
	}

	return s;
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{

	//UV座標とテクスチャを参照して、最適な色を取得する
	float2 uv = PSInput.TexCoords0;
	float2 revers = float2(abs(g_revers - uv.x), uv.y);
	float4 srcCol = g_SrcTexture.Sample(g_SrcSampler, revers);


	//雷
	float s = 0.0f;

	//始点、終点
	float2 endPos = float2(0.1, 1);
	float2  startPos =
		float2((cos(g_time * 0.001f) + 1.0f) * 0.005f + 0.1f,
				(sin(g_time * 0.001) + 1.0f) * 0.005f - 0.2f);
	//求めた雷模様を加算
	s += Lightning(uv, startPos, endPos);

	//始点、終点
    endPos = float2(0.9, 1);
    startPos =
		float2((cos(g_time * 0.001f) + 1.0f) * 0.005f + 0.9f,
				(sin(g_time * 0.001) + 1.0f) * 0.005f - 0.2f);
	//求めた雷模様をk加算
	s += Lightning(uv, startPos, endPos);


	// 雷の色をセット
	float4 lightning = float4(s, s, s - 1.0f, s);


	//画像色の進行度
    float pro = 1.0f - sin(g_time * g_compTime);
	//黒色
	float3 black = 0.0f;
	//画像との差分
	float3 disCol = lerp(black.rgb, srcCol.rgb, pro);

	//結果
	float3 result = srcCol.rgb + (disCol * pro);

	return float4 (disCol, srcCol.a) + lightning;
}