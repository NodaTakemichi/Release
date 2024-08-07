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
	float4 g_color;
	float g_time;
}


//サンプラー：適切な色を決める
SamplerState g_SrcSampler : register(s0);

float4 main(PS_INPUT PSInput) : SV_TARGET
{
	//UV座標を受け取る
	float2 uv = PSInput.TexCoords0;

	//回転
	//回転ベクトル
	float speed = 1.0f;
	float rotCos = cos(g_time * speed);
	float rotSin = sin(g_time * speed);

	//2次元の回転行列
	float2x2 mat = { rotCos, -rotSin, rotSin, rotCos };

	//UV座標を受け取る(中心地を真ん中にする)
	uv = PSInput.TexCoords0 - 0.5f;
	//uvスクロール
	uv = mul(uv, mat) + 0.5f;



	float2 center = float2(0.5f, 0.5f);
	//中心線に近かったら照らす
	float lr = distance(uv.x, center.x) * 2.0f;
	lr = min(lr, 1.0f);

	//白色
	float3 white = float3(1.0f, 1.0f, 1.0f);
	//線形補間
	float3 result = lerp(white, g_color, lr);

	return float4(result, 1.0f);

}