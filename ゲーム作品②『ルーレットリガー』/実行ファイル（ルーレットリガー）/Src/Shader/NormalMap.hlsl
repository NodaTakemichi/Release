//ピクセルシェーダーの入力
struct PS_INPUT
{
	//座標（プロジェクション空間）
	float4 Position			:SV_POSITION;
	//ディフーズカラー
	float4 Diffuse			:COLORO;
	//テクスチャ座標
	float2 TexCoords0		:TEXCOORD0;
};

//定数バッファ：スロット番号3番目（b3）
cbuffer cbParam : register(b3)
{
	float g_time;
}

//描画するテクスチャ
Texture2D g_SrcTexture:register(t0);
//法線マップ
Texture2D g_SrcNormalMap:register(t1);

//サンプラー
SamplerState g_SrcSampler:register(s0);


float4 main(PS_INPUT PSInput) : SV_TARGET
{
	float2 uv = PSInput.TexCoords0;

	//ベクトルの算出
	float4 n = g_SrcNormalMap.Sample(g_SrcSampler,uv);
	//範囲　正規化
	n.xy *= 2;
	n.xy -= 1;
	n.z = -1;
	n.xyz = normalize(n.xyz);//正規化

	//光：回転速度
	float ls = g_time * 0.6f;

	//陰影計算						 
	float3 light = normalize(float3(cos(ls), sin(ls), 1));
	float b = saturate(dot(n.xyz, -light));//クランプより速いクランプ
	light = reflect(light, n);//ディフーズ
	float3 eye = float3(0, 0, 1);
	float spec = pow(saturate(dot(-eye, light)), 10);//スペキュラ
	float4 col = g_SrcTexture.Sample(g_SrcSampler,uv);
	return float4(col.xyz * b + spec, col.a);

}