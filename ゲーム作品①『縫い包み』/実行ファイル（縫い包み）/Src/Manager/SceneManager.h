#pragma once
#include <chrono>
#include <memory>
#include <assert.h>
#include <list>
#include "../Utility/StaticSingleton.h"
#include "../Common/Vector2.h"
class SceneBase;
class InputManager;

class Fader;
class Camera;
class Grid;

class SceneManager :
	public StaticSingleton<SceneManager>
{
public:
	//FPS値
	static constexpr float DEFAULT_FPS = 60.0f;
	//重力
	static constexpr float GRAVITY = 9.81f;
	//デルタタイム
	static constexpr float DELTA_TIME = 1.0f / 60.0f;
	//フォグのスタート距離
	static constexpr float FOG_START_DISTANCE = 500.0f;
	//フォグのエンド距離
	static constexpr float FOG_END_DISTANCE = 1200.0f;

	// シーン管理用
	enum class SCENE_ID
	{
		NONE,
		TITLE,
		GAME,
		GAMEOVER,
		RESULT,
		PAUSE,
		KEYCONFIG
	};
	
	//初期化
	void Init(void)override;
	//３D情報の初期化
	void Init3D(void);

	/// <summary>
	/// 先頭の（Updataが呼ばれる）シーンを切り替える
	/// </summary>
	/// <param name="scene">切り替え先のシーン</param>
	void CreateScene(std::shared_ptr<SceneBase> scene);

	/// <summary>
	/// すべてのシーンを切り替える
	/// </summary>
	/// <param name="scene">切り替え先のシーン</param>
	void ChangeAllScene(std::shared_ptr<SceneBase> scene);

	/// <summary>
	/// シーンをプッシュする。スタックの数が増える
	/// 一番上のシーンのUpdataしか呼ばれません。
	/// </summary>
	/// <param name="scene">積むシーン</param>
	void PushScene(std::shared_ptr<SceneBase> scene);

	/// <summary>
	/// スタックの頭のシーンを削除する。
	/// ただし、スタック上にシーンが一つしかない場合は、削除しない。
	/// </summary>
	void PopScene();

	/// <summary>
	/// 持っているシーンスタックの先頭のUpdataを呼び出す。
	/// </summary>
	/// <param name="input">入力IO</param>
	void Updata(InputManager& input);

	/// <summary>
	/// 持っているシーンすべてを描画する。
	/// ただし、描画順は下から描画される。
	/// </summary>
	void Draw();

	// 状態遷移
	void ChangeScene(SCENE_ID nextId);

	// シーンIDの取得
	SCENE_ID GetSceneID(void);

	//カメラの取得
	Camera& GetCamera(void);

	// デルタタイムの取得
	const float& GetDeltaTime(void) const;

	// 経過時間の取得
	const float& GetTotalTime(void) const;

	// メインスクリーンの取得
	const int& GetMainScreen(void) const { return mainScreen_; }

	//画面揺れの振幅をセット
	void SetScreenAmplitude(const float& amp) { amplitude_ = amp; }

	//フェードイン開始
	void StartFadeIn(void);	

	//フェードカラーの指定
	void SetFadeColor(unsigned int color);	

	//描画スクリーンのセット
	void SetDrawingScreen(const int& screenID);

private:
	//シングルトン
	THIS_CLASS_IS_STATIC_SINGLETON(SceneManager);

	SCENE_ID sceneId_;
	SCENE_ID waitSceneId_;

	// フェード
	std::unique_ptr<Fader> fader_;
	// カメラ
	std::unique_ptr<Camera> camera_;

	// 現在実行中のシーン
	std::list<std::shared_ptr<SceneBase>> scenes_;

	// シーン遷移中判定
	bool isSceneChanging_;

	// デルタタイム
	std::chrono::system_clock::time_point preTime_;
	float deltaTime_;
	//経過時間
	float totalTime_;

	//メインスクリーン
	int mainScreen_;
	//スクリーン座標
	Vector2 screenPos_;
	//スクリーン振幅
	float amplitude_;
	//フレーム
	int frame_;

	SceneManager(void);
	~SceneManager(void);

	// デルタタイムをリセットする
	void ResetDeltaTime(void);

	// シーン遷移
	void DoChangeScene(SCENE_ID sceneId);

	// フェード
	void Fade(void);

	//スクリーン座標揺れ計算
	void CalcScreenShake(void);


};