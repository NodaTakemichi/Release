#pragma once
#include<DxLib.h>
#include<unordered_map>

class Vector2;

class DrawShader
{
public:

	enum class PS_TYPE
	{
		TEXTURE,
		REVERSE_TEXTURE,
		MONOTONE,
		NORMAL_MAP,
		MASK,
		APPEARANCE,
		DISTORTION,
		SIMPLE_SEPIA,
		FADE,
		FADE_TEXTURE,

		ON_BUTTON,
		ON_CIRCLE_BUTTON,
		CARD_FRAME,
		IUMI_FRAME,
		HP_SHADER,
		DEATH_SHADER,
		DEATH_UNIT,

		STATUS_UP,
		STATUS_DOWN,
		PARALYSIS,
		POISON,
		BAYER_DITHE
	};


	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static DrawShader& GetInstance(void);

	// 初期化
	void Init(void);

	// 解放(シーン切替時に一旦解放)
	void Release(void);

	//画像通常描画
	void DrawGraph(
		const Vector2& pos, const int& handle);
	//画像描画（定数なし）
	void DrawGraphToShader(
		const Vector2& pos,const int& handle, const PS_TYPE& type);
	//画像描画（定数付き）
	void DrawGraphToShader(
		const Vector2& pos,const int& handle, const PS_TYPE& type,
		const COLOR_F& buf,const COLOR_F& subBuf = COLOR_F{});
	//サイズ指定描画（通常）
	void DrawExtendGraphToShader(
		const Vector2& pos, const Vector2& size , const int& handle);
	//サイズ指定描画
	void DrawExtendGraphToShader(
		const Vector2& pos, const Vector2& size , 
		const int& handle, const PS_TYPE& type,
		const COLOR_F& buf, const COLOR_F& subBuf = COLOR_F{});
	
	
	//サブテクスチャ付き描画
	void DrawGraphAndSubToShader(
		const Vector2& pos,
		const int& mHandle, const int& sHandle,
		const PS_TYPE& type, const COLOR_F& buf, const COLOR_F& subBuf = COLOR_F{});

	//サブテクスチャ付き描画(マスクサイズ)
	void DrawGraphAndSubToShader(
		const Vector2& pos, const Vector2& size,
		const int& mHandle, const int& sHandle,
		const PS_TYPE& type, const COLOR_F& buf, const COLOR_F& subBuf = COLOR_F{});


private:
	// ピクセルシェーダ用オリジナル定数バッファの使用開始スロット
	static constexpr int CONSTANT_BUF_SLOT_BEGIN_PS = 3;
	// 頂点数
	static constexpr int NUM_VERTEX = 4;
	// 頂点インデックス数
	static constexpr int NUM_VERTEX_IDX = 6;
	// ポリゴン数
	static constexpr int NUM_POLYGON = 2;

	// 静的インスタンス
	static DrawShader* instance_;

	//頂点情報
	VERTEX2DSHADER vertex_[NUM_VERTEX];
	WORD index_[NUM_VERTEX_IDX];

	//シェーダー定数バッファ
	int psConstBuf_;

	//シェーダーのリソースマップ
	std::unordered_map< PS_TYPE, int>psMap_;

	//描画用の四角頂点の作成
	void MakeSquereVertex(Vector2 pos, Vector2 size);
	
	//シェーダーのロード
	void InitPS(void);

	//シェーダーの検索
	const int& SearchPS(const PS_TYPE& ps)const;


	DrawShader(void);
	~DrawShader(void);

};

