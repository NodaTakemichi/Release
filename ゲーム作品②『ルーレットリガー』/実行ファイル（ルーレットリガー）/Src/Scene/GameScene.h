#pragma once
#include <vector>
#include "SceneBase.h"
#include "../Utility/DrawShader.h"
#include "../Common/Vector2.h"

class UnitManager;
class Roulette;
class GameUI;

class BattleSystem;
class DeathStaging;
class EffectManager;

class GameScene : public SceneBase
{

	const float START_IMAGE_FADE_TIME = 2.0f;

public:
	enum class GAME_PHASE
	{
		NONE,
		BATTLE_START,
		RULLET_TIME,
		AIM,
		EFFECT,
		BATTLE,
		BUFF_EFFECT,
		TURN_END,
		GAME_END
	};


	// コンストラクタ
	GameScene(void);

	// デストラクタ
	~GameScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

	void UpdateRouTime(void);
	void UpdateAIM(void);
	void UpdateEffect(void);
	void UpdateBattle(void);
	void UpdateBuffEffect(void);
	void UpdateTurnEnd(void);
	void UpdateGameEnd(void);


private:
	//ユニットマネージャー
	UnitManager* unitMng_;
	//ルーレット
	Roulette* roulette_;
	//UI
	GameUI* GameUi_;
	//ユニットのバトルシステム
	BattleSystem* battleSys_;

	//ゲームフェーズ
	GAME_PHASE phase_;
	//ネクストフェーズ
	GAME_PHASE nextPhase_;

	//フレーム
	int frameImg_;
	int topFrameImg_;

	//行動ユニットの操作が自動か手動か判断
	bool actUnitAoutm_;

	//画面スクリーン
	int mainScreen_;
	DrawShader::PS_TYPE psType_;
	COLOR_F screenPsBuf_;

	//スクリーン座標
	Vector2 screenPos_;
	//スクリーン振幅
	float amplitude_;
	//フレーム
	int frame_;

	//音声関連
	int winBgm_;		//勝利BGM
	int loseBgm_;		//敗北BGM
	int resultBgm_;		//結果BGM

	int turnChangeSE_;	//ターン変更SE

	//UIフォントハンドル
	int uiFontHandle_;
	
	//勝負判定
	bool CheckVictory(void);
	bool isPlayerWin_;

	//勝負結果画像
	int resultImg_;		//結果画像
	int winImg_;		//勝利画像
	int loseImg_;		//敗北画像
	float resutlTime_;	//結果画面経過時間

	//バトルスタート画像
	int battleStartImg_;
	//バトルスタート用フェード画像
	int bStartFadeImg_;
	//背景画像
	std::vector<int>backImgs_;


	//スタート画面描画
	void DrawBattleStart(void);

	//結果画面描画
	void DrawResult(void);
	//結果画面の描画設定
	void SettingScreenShader(void);

	//フェーズ変更
	void ChangeGamePhase(GAME_PHASE phase);

	//スクリーン座標揺れ計算
	void CalcScreenShake(void);

};
