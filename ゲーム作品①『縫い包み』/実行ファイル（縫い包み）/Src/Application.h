#pragma once
#include <string>

class Application
{

public:

	// スクリーンサイズ
	static constexpr int SCREEN_SIZE_X = 1280;
	static constexpr int SCREEN_SIZE_Y = 800;


	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static Application& GetInstance(void);

	// 初期化
	void Init(void);
	// ゲームループの開始
	void Run(void);
	// リソースの解放
	void Release(void);
	//破棄
	void Destroy(void);

	// 初期化成功／失敗の判定
	bool IsInitFail(void) const;

	// 解放成功／失敗の判定
	bool IsReleaseFail(void) const;

	//ウィンドウの終了
	void CloseWindows(void);

private:

	// 静的インスタンス
	static Application* instance_;

	// 初期化失敗
	bool isInitFail_;

	// 解放失敗
	bool isReleaseFail_;

	//ウィンドウズの終了判断	true/終了　false/継続
	bool isExitWindows_;

	Application(void);
	Application(const Application&);
	~Application(void);

	//Effekseerの初期化
	void InitEffekseer(void);

};