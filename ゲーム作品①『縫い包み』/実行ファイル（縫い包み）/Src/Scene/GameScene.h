#pragma once
#include<memory>
#include<vector>
#include "SceneBase.h"
class Stage;
class GoalGate;
class Player;
class Tracker;
class ItemBase;
class PlayerUI;
class OperationUI;
class GoalDoor;
class WarpGate;

class PostEffectMaterial;
class PostEffectRenderer;

class GameScene :
	public SceneBase
{
public:

	GameScene(SceneManager& manager);
	~GameScene(void);

	virtual void LoadData(void)override;	
	virtual void Init(void) override;
	virtual void Update(InputManager& input) override;
	virtual void Draw(void) override;

private:
	// ステージ
	std::unique_ptr<Stage> stage_;
	// ゴールゲート
	std::shared_ptr<GoalGate> goalGate_;
	// ゴールドア
	std::unique_ptr<GoalDoor> goalDoor_;
	// プレイヤー(キャラクター)
	std::unique_ptr<Player> player_;
	// 幽霊(キャラクター)
	std::unique_ptr<Tracker> tracker_;
	// アイテム
	std::vector<std::shared_ptr<ItemBase>> items_;
	// ワープゲート
	std::vector<std::shared_ptr<WarpGate>> warpGates_;

	// プレイヤーUI
	std::shared_ptr<PlayerUI> playerUI_;
	// 操作UI
	std::shared_ptr<OperationUI> operationUI_;

	//ゲームオーバー判定	true:ゲームオーバー/false:ゲームオーバーではない
	bool isGameOver_=false;
	//ゲームオーバー時間
	float gameOverTime_ = 0.0f;

	//BGM
	std::shared_ptr<SoundController>goalSE_;

	//ポストエフェクトの初期化
	void InitPE(void);

	//アイテムの生成
	void CreateItems(void);
	//アイテムのロード
	void LoadItem(void);

	//Update関数
	void LoadingUpdate(InputManager& input) ;
	void NormalUpdate(InputManager& input) ;
	//描画関数
	void LoadingDraw(void) ;
	void NormalDraw(void) ;

	//ゲームオーバー判定
	bool CheckGameOver(void);


};

