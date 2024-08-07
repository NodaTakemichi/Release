//インクルードガード
#if !defined(VERTEX_INPUT_TYPE_HEADER)
	#define VERTEX_INPUT_TYPE_HEADER

	//通常のモデル
	#define VERTEX_INPUT_TYPE_1FRAME		(1)		//1フレームの影響を受ける頂点
	#define VERTEX_INPUT_TYPE_4FRAME		(2)		//1～4フレームの影響を受ける頂点
	#define VERTEX_INPUT_TYPE_8FRAME		(3)		//5～8フレームの影響を受ける頂点

	//法線マップ付き
	#define VERTEX_INPUT_TYPE_NMAP_1FRAME	(4)		//法線マップの情報が含まれる1フレームの影響を受ける頂点
	#define VERTEX_INPUT_TYPE_NMAP_4FRAME	(5)		//法線マップの情報が含まれる1～4フレームの影響を受ける頂点
	#define VERTEX_INPUT_TYPE_NMAP_8FRAME	(6)		//法線マップの情報が含まれる5～8フレームの影響を受ける頂点

	//VERTEX3DSHADER構造体
	#define VERTEX_INPUT_TYPE_ORIGIN		(7)		//DrawPolygon3D関数等を使用した場合に送られる頂点
	
	//以下、DXライブラリ上の表記
	//通常のモデル
	#define DX_MV1_VERTEX_TYPE_1FRAME	VERTEX_INPUT_TYPE_1FRAME
	#define DX_MV1_VERTEX_TYPE_4FRAME	VERTEX_INPUT_TYPE_4FRAME
	#define DX_MV1_VERTEX_TYPE_8FRAME   VERTEX_INPUT_TYPE_8FRAME

	//法線マップ付き
	#define DX_MV1_VERTEX_TYPE_NMAP_1FRAME	VERTEX_INPUT_TYPE_NMAP_1FRAME
	#define DX_MV1_VERTEX_TYPE_NMAP_4FRAME	VERTEX_INPUT_TYPE_NMAP_4FRAME
	#define DX_MV1_VERTEX_TYPE_NMAP_8FRAME	VERTEX_INPUT_TYPE_NMAP_8FRAME

#endif
