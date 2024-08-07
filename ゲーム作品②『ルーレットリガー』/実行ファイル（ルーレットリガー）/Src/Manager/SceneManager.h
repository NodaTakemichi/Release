#pragma once
#include <chrono>
#include <memory>
#include <array>
#include <random>
class SceneBase;
class Fader;

class SceneManager
{

public:

	// シーン管理用
	enum class SCENE_ID
	{
		NONE,
		TITLE,
		SELECT,
		DECK_EDIT,
		GAME,
		RESULT,
	};

	
	// インスタンスの生成
	static void CreateInstance(void);

	// インスタンスの取得
	static SceneManager& GetInstance(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	// 状態遷移
	void ChangeScene(SCENE_ID nextId, bool isFading);

	// シーンIDの取得
	SCENE_ID GetmSceneID(void);

	// デルタタイムの取得
	float GetDeltaTime(void) const;
	//経過時間の取得
	float GetTotalTime(void) const;

	//乱数の取得
	int GetRand(const int& min, const int& max);

private:

	// 静的インスタンス
	static SceneManager* instance_;

	SCENE_ID sceneID_;
	SCENE_ID waitSceneID_;

	SceneBase* scene_;
	Fader* fader_;

	bool isSceneChanging_;

	// デルタタイム
	std::chrono::system_clock::time_point preTime_;
	float deltaTime_;
	//経過時間
	float totalTime_;

	//乱数生成器
	std::random_device rd_;
	std::mt19937 gen_;

	//マウス画像
	int mouseImg_;
	//クリックエフェクトハンドル
	int clickEfHandle_;
	//エフェクト再生ハンドル
	int efPlayHandle_;

	//シーン遷移SE
	int sceneMoveSE_;
	//描画スクリーン
	int screen_;
	//フレーム
	int frame_;
	//画面振幅
	float amplitude_;

	SceneManager(void);
	~SceneManager(void);

	void ResetDeltaTime(void);

	// シーン遷移
	void DoChangeScene(void);
	// フェード
	void Fade(void);
	//クリックエフェクト描画
	void DrawClickEf(void);


};