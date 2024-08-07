//インクルードガード
#if !defined(VERTEX_STRUCTURES_HEADER)
	#define VERTEX_STRUCTURES_HEADER

	#define DX_D3D11_COMMON_CONST_LIGHT_NUM			(6)			// 共通パラメータのライトの最大数
	#define DX_D3D11_VS_CONST_TEXTURE_MATRIX_NUM	(3)			// テクスチャ座標変換行列の転置行列の数
	#define DX_D3D11_VS_CONST_WORLD_MAT_NUM			(54)		// トライアングルリスト一つで同時に使用するローカル→ワールド行列の最大数

	// 定数バッファ頂点シェーダー基本パラメータ
	struct VsBase
	{
	    matrix antiViewportMatrix;  // アンチビューポート行列
	    matrix projectionMatrix;    // ビュー　→　プロジェクション行列
	    float4x3 viewMatrix;        // ワールド　→　ビュー行列
	    float4x3 localWorldMatrix;  // ローカル　→　ワールド行列

	    float4 toonOutLineSize;     // トゥーンの輪郭線の大きさ
	    float diffuseSource;        // ディフューズカラー( 0.0f:マテリアル  1.0f:頂点 )
	    float specularSource;       // スペキュラカラー(   0.0f:マテリアル  1.0f:頂点 )
	    float mulSpecularColor;     // スペキュラカラー値に乗算する値( スペキュラ無効処理で使用 )
	    float padding;
	};

	// その他の行列
	struct VsOtherMatrix
	{
	    float4x4 shadowMapLightViewProjectionMatrix[3];					// シャドウマップ用のライトビュー行列とライト射影行列を乗算したもの
	    float4x2 textureMatrix[DX_D3D11_VS_CONST_TEXTURE_MATRIX_NUM];	// テクスチャ座標操作用行列
	};

	// スキニングメッシュ用の　ローカル　→　ワールド行列
	struct VsLocalWorldMatrix
	{
	    float4 lwMatrix[DX_D3D11_VS_CONST_WORLD_MAT_NUM * 3];           // ローカル　→　ワールド行列
	};

#endif
