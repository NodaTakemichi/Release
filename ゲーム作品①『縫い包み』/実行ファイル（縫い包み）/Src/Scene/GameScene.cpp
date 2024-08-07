#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../Application.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/SoundManager.h"
#include "../Manager/Camera.h"
#include "../Utility/Utility.h"

#include "../Object/Stage/Stage.h"
#include "../Object/Stage/GoalGate.h"
#include "../Object/Stage/GoalDoor.h"
#include "../Object/Stage/WarpGate.h"
#include "../Object/Character/Player.h"
#include "../Object/Character/Tracker.h"
#include "../Object/Item/ItemBase.h"
#include "../Object/Item/Dool.h"
#include "../Object/Item/Amulet.h"
#include "../UI/PlayerUI.h"
#include "../UI/OperationUI.h"

#include "../Graphics/Drawable/PostEffect/PostEffectMaterial.h"
#include "../Graphics/Drawable/PostEffect/PostEffectRenderer.h"
#include "../Sound/SoundController.h"

#include "PauseScene.h"
#include "GameOverScene.h"
#include "GameScene.h"

//ゴールSE音量
constexpr int GOAL_SE_VOLUME = 150;
constexpr int GOAL_SE_VOLUMsE = 150;

//ワープゴール座標
constexpr int WARP_GATE_CREATE_NUM = 6;
constexpr VECTOR WARP_GATE_POS_FLOOR_LEFT_BOTTOM	= { -910.0f,400.0f,-2330.0f };	//一階　左下
constexpr VECTOR WARP_GATE_POS_FLOOR_RIGHT_BOTTOM	= { 910.0f,400.0f,-2330.0f };	//一階　右下
constexpr VECTOR WARP_GATE_POS_FLOOR_LEFT_TOP		= { -910.0f,400.0f, 2330.0f };	//一階　左上
constexpr VECTOR WARP_GATE_POS_FLOOR_RIGHT_TOP		= { 910.0f,400.0f, 2330.0f };	//一階　右上
constexpr VECTOR WARP_GATE_POS_BASEMENT_RIGHT		= { 570.0f,-110.0f, 200.0f };	//地下　右
constexpr VECTOR WARP_GATE_POS_BASEMENT_LEFT		= { -370.0f,-110.0f,1000.0f };	//地下　左

//ぬいぐるみ
constexpr float DOOL_MODEL_SCALE = 0.25f;	//大きさ
//サル
constexpr VECTOR MONKEY_POS = { -1059.0f,435.0f,-599.0f };	//座標
constexpr VECTOR MONKEY_ROT = { 0.0f,-90.0f,0.0f };			//角度
//ウサギ
constexpr VECTOR RABBIT_POS = { 448.0f,-60.0f,560.0f };		//座標
constexpr VECTOR RABBIT_ROT = { 0.0f,60.0f,0.0f };			//角度
//ヒツジ
constexpr VECTOR SHEEP_POS  = { 514.0f, 430.0f,1430.0f };	//座標
constexpr VECTOR SHEEP_ROT  = { 0.0f,150.0f,0.0f };			//角度

//護符
constexpr float AMULET_MODEL_SCALE = 1.0f;	//大きさ
//ゴール前
constexpr VECTOR AMULET_POS_GOAL_FRONT		 = { -105.0f,500.0f,-1518.0f };
constexpr VECTOR AMULET_ROT_GOAL_FRONT		 = { 0,0,10 };
//手前通路
constexpr VECTOR AMULET_POS_PASSAGEWAY_FRONT = { 780.0f,500.0f,1316.0f };
constexpr VECTOR AMULET_ROT_PASSAGEWAY_FRONT = { 0,180,-23 };
//サルの横
constexpr VECTOR AMULET_POS_BESIDE_MONKEY	 = { -1082.0f,500.0f,-480.0f };
constexpr VECTOR AMULET_ROT_BESIDE_MONKEY 	 = { 0,-90,0 };
//右奥の小部屋	
constexpr VECTOR AMULET_POS_SMALL_ROOM_RIGHT = { 370.0f,500.0f,-510.0f };
constexpr VECTOR AMULET_ROT_SMALL_ROOM_RIGHT = { 0,0,-38 };
//左奥の小部屋
constexpr VECTOR AMULET_POS_SMALL_ROOM_LEFT	 = { -600.0f,500.0f,1900.0f };
constexpr VECTOR AMULET_ROT_SMALL_ROOM_LEFT	 = { 0,0,25 };
//大部屋
constexpr VECTOR AMULET_POS_LARG_ROOM		 = { -500.0f,500.0f,295.0f };
constexpr VECTOR AMULET_ROT_LARG_ROOM		 = { 0,0,25 };
//地下の右
constexpr VECTOR AMULET_POS_BASEMENT_RIGHT	 = { 245.0f,-2.0f,1155.0f };
constexpr VECTOR AMULET_ROT_BASEMENT_RIGHT	 = { 0,30,0 };
//地下の左
constexpr VECTOR AMULET_POS_BASEMENT_LEFT	 = { -295.0f,-2.0f,1155.0f };
constexpr VECTOR AMULET_ROT_BASEMENT_LEFT	 = { 0,-90,30 };

//死亡演出時間
constexpr float DETH_SHOW_TIME = 2.25f;

GameScene::GameScene(SceneManager& manager):SceneBase(manager)
{
	//更新関数のセット
	updataFunc_ = [&](InputManager& input) {
		LoadingUpdate(input);
	};
	//描画関数のセット
	drawFunc_ = std::bind(&GameScene::LoadingDraw, this);
}

GameScene::~GameScene(void)
{
}

void GameScene::LoadData(void)
{
	auto& res = ResourceManager::GetInstance();
	loadingTime_ = 0.0f;

	//非同期読み込みを有効にする
	SetUseASyncLoadFlag(true);


	//プレイヤー
	player_ = std::make_unique<Player>();
	player_->LoadData();
	//ステージ
	stage_ = std::make_unique<Stage>(*player_);
	stage_->LoadData();
	//ゴールゲート
	goalGate_ = std::make_unique<GoalGate>();
	goalGate_->LoadData();
	//追跡者
	tracker_ = std::make_unique<Tracker>(*player_);
	tracker_->LoadData();
	//ゴールドア
	goalDoor_ = std::make_unique<GoalDoor>();
	goalDoor_->LoadData();
	//アイテムの生成
	LoadItem();
	//ワープゲートの生成
	for (int i = 0; i < WARP_GATE_CREATE_NUM; i++)
	{
		warpGates_.push_back(std::make_unique<WarpGate>());
		warpGates_[i]->LoadData();
	}

	//プレイヤーUI
	playerUI_ = std::make_unique<PlayerUI>(*player_);
	playerUI_->LoadData();
	//操作UI
	operationUI_ = std::make_unique<OperationUI>();
	operationUI_->LoadData();

	//ゴールSE
	goalSE_ = std::make_shared<SoundController>();
	goalSE_->DataLoad(ResourceManager::SRC::GOAL_SND, false);

	//フォントの登録
	res.Load(ResourceManager::SRC::SELECT_CMD_FONT);
	buttnFontHandle_ = CreateFontToHandle(L"EbiharaNoKuseji", FONT_SIZE, 0);

}

void GameScene::Init(void)
{
	//ステージ
	stage_->Init();
	//ゴールゲート
	goalGate_->Init();
	//ゴールドア
	goalDoor_->Init();
	//プレイヤー
	player_->Init();
	//追跡者
	tracker_->Init();
	//アイテムの生成
	CreateItems();
	//ワープゲートの生成
	for (const auto& warpGate: warpGates_)
	{
		warpGate->Init();
	}
	//ワープゲートの座標、角度の設定
	int count = 0;
	auto createWarpGate = [&](VECTOR pos, VECTOR rot)
	{
		if (count >= warpGates_.size())return;
		warpGates_[count]->SetPos(pos);
		warpGates_[count]->SetRot(rot);
		count++;
	};
	createWarpGate(WARP_GATE_POS_FLOOR_LEFT_BOTTOM, {});
	createWarpGate(WARP_GATE_POS_FLOOR_RIGHT_BOTTOM, {});
	createWarpGate(WARP_GATE_POS_FLOOR_LEFT_TOP, {});
	createWarpGate(WARP_GATE_POS_FLOOR_RIGHT_TOP, {});
	createWarpGate(WARP_GATE_POS_BASEMENT_RIGHT, { 0.0f,Utility::ANGLE_90_RAD,0.0f });
	createWarpGate(WARP_GATE_POS_BASEMENT_LEFT, { 0.0f,Utility::ANGLE_90_RAD,0.0f });


	//プレイヤーUI
	playerUI_->Init();
	//操作UI
	operationUI_->Init();

	//ステージコライダー情報の受け渡し
	player_->AddStageCollider(*(stage_->GetCollStageTransform().collider_));
	player_->AddStageCollider(*(goalGate_->GetTransform().collider_));
	player_->AddStageCollider(*(goalDoor_->GetTransform().collider_));
	for (const auto& warpGate : warpGates_)
	{
		player_->AddStageCollider(*(warpGate->GetTransform().collider_));
	}
	tracker_->AddStageCollider(*(stage_->GetCollStageTransform().collider_));
	//ゴールゲート情報の受け渡し
	player_->SetGoalGate(goalGate_);

	//ポストエフェクト
	InitPE();

	//カメラモードのセット（一人称カメラ）
	sceneManager_.GetCamera().ChangeMode(Camera::MODE::FPS);
	//カメラ追従対象の設定
	sceneManager_.GetCamera().SetFollow(&player_->GetTransform());


	//ゴールSE
	goalSE_->Init(GOAL_SE_VOLUME);
}

void GameScene::Update(InputManager& input)
{
	updataFunc_(input);
	return;
}

void GameScene::Draw(void)
{
	drawFunc_();
	return;
}

void GameScene::InitPE(void)
{
	auto& res = ResourceManager::GetInstance();

	//メインスクリーン
	int mainScreen = sceneManager_.GetMainScreen();
	//ノイズ画像
	int noiseImg = res.Load(ResourceManager::SRC::NOISE_IMG).handleId_;

}

void GameScene::CreateItems(void)
{
	std::shared_ptr<ItemBase> item;

	auto createItem = [&](
		ItemBase::ITEM_TYPE type,VECTOR scl, VECTOR pos, VECTOR rot) 
	{
		item->SetModelInfo(type,scl,pos,rot);

		//アイテムコライダー情報の受け渡し
		player_->AddItemInfo(item);
		items_.emplace_back(std::move(item));
	};

	//ぬいぐるみの大きさ
	const float doolScl = DOOL_MODEL_SCALE;

	//ぬいぐるみ生成（サル）
	item = std::make_shared<Dool>();
	createItem(
		ItemBase::ITEM_TYPE::MONKEY,
		{ doolScl,doolScl,doolScl },
		MONKEY_POS,	MONKEY_ROT
	);
	//ぬいぐるみ生成（ウサギ）
	item = std::make_shared<Dool>();
	createItem(
		ItemBase::ITEM_TYPE::RABBIT,
		{ doolScl,doolScl ,doolScl },
		RABBIT_POS,	RABBIT_ROT
	);
	////ぬいぐるみ生成（ヒツジ）
	//item = std::make_shared<Dool>();
	//createItem(
	//	ItemBase::ITEM_TYPE::SHEEP,
	//	{ doolScl,doolScl ,doolScl },
	//	SHEEP_POS,SHEEP_ROT
	//);


	//護符アイテムの生成
	//護符の大きさ
	const float amuletScl = AMULET_MODEL_SCALE;

	item = std::make_shared<Amulet>();		//ゴール前
	createItem(
		ItemBase::ITEM_TYPE::AMULET,
		{ amuletScl,amuletScl,amuletScl },
		AMULET_POS_GOAL_FRONT, AMULET_ROT_GOAL_FRONT
	);
	item = std::make_shared<Amulet>();		//手前通路
	createItem(
		ItemBase::ITEM_TYPE::AMULET,
		{ amuletScl ,amuletScl ,amuletScl },
		AMULET_POS_PASSAGEWAY_FRONT, AMULET_ROT_PASSAGEWAY_FRONT
	);
	item = std::make_shared<Amulet>();		//サルの縫い包み
	createItem(
		ItemBase::ITEM_TYPE::AMULET,
		{ amuletScl ,amuletScl ,amuletScl },
		AMULET_POS_BESIDE_MONKEY, AMULET_ROT_BESIDE_MONKEY
	);
	item = std::make_shared<Amulet>();		//右奥の小部屋
	createItem(
		ItemBase::ITEM_TYPE::AMULET,
		{ amuletScl ,amuletScl ,amuletScl },
		AMULET_POS_SMALL_ROOM_RIGHT, AMULET_ROT_SMALL_ROOM_RIGHT
	);
	item = std::make_shared<Amulet>();		//左奥の小部屋
	createItem(
		ItemBase::ITEM_TYPE::AMULET,
		{ amuletScl ,amuletScl ,amuletScl },
		AMULET_POS_SMALL_ROOM_LEFT, AMULET_ROT_SMALL_ROOM_LEFT
	);
	item = std::make_shared<Amulet>();		//大部屋
	createItem(
		ItemBase::ITEM_TYPE::AMULET,
		{ amuletScl ,amuletScl ,amuletScl },
		AMULET_POS_LARG_ROOM, AMULET_ROT_LARG_ROOM
	);
	//地下
	item = std::make_shared<Amulet>();
	createItem(
		ItemBase::ITEM_TYPE::AMULET,
		{ amuletScl ,amuletScl ,amuletScl },
		AMULET_POS_BASEMENT_LEFT, AMULET_ROT_BASEMENT_LEFT
	);
	item = std::make_shared<Amulet>();
	createItem(
		ItemBase::ITEM_TYPE::AMULET,
		{ amuletScl ,amuletScl ,amuletScl },
		AMULET_POS_BASEMENT_RIGHT, AMULET_ROT_BASEMENT_RIGHT
	);


}

void GameScene::LoadItem(void)
{
	auto& res = ResourceManager::GetInstance();

	res.Load(ResourceManager::SRC::AMULET_MODEL);
	res.Load(ResourceManager::SRC::DOOL_MONKEY);
	res.Load(ResourceManager::SRC::DOOL_PIG);
	res.Load(ResourceManager::SRC::DOOL_RABBIT);
	res.Load(ResourceManager::SRC::DOOL_SHEEP);
}

void GameScene::LoadingUpdate(InputManager& input)
{
	bool loadTimeOver = Utility::TimeOver(loadingTime_, LOADING_TIME);

	//ロードが完了したか判断
	if (GetASyncLoadNum() == 0 && loadTimeOver)
	{
		//非同期処理を無効にする
		SetUseASyncLoadFlag(false);

		//カーソルモードの変更
		input.ChangeCurrsolMode(false);


		//初期化処理
		Init();
		//フェードイン開始
		sceneManager_.StartFadeIn();

		//更新関数のセット
		updataFunc_ = [&](InputManager& input) {NormalUpdate(input);};
		//描画関数のセット
		drawFunc_ = std::bind(&GameScene::NormalDraw, this);
	}
}

void GameScene::NormalUpdate(InputManager& input)
{
	//ポーズシーンへ遷移
	if (input.IsTrgDown(KEY_INPUT_ESCAPE))
	{
		sceneManager_.PushScene(std::make_shared<PauseScene>(sceneManager_));
		return;
	}

	//ゲームオーバー判定
	if (CheckGameOver()) 
	{
		//ゲームオーバー時処理
		if (!isGameOver_)
		{
			isGameOver_ = true;
			//それぞれの座標を取得
			auto playerPos = player_->GetTransform().GetPos();
			auto trackerPos = tracker_->GetTransform().GetPos();

			//それぞれに相手座標を設定、回転させる
			player_->SetGameOverInfo(trackerPos);
			tracker_->SetGameOverInfo(playerPos);

			//音声の全停止
			SoundManager::GetInstance().StopAllSound();
		}

		//演出終了判定
		gameOverTime_ += sceneManager_.GetDeltaTime();
		if (Utility::TimeOver(gameOverTime_, DETH_SHOW_TIME))
		{
			//カーソルモードの変更
			input.ChangeCurrsolMode(true);

			//ゲームオーバーシーンへ遷移
			sceneManager_.PushScene(std::make_shared<GameOverScene>(sceneManager_));
			return;
		}
	}

	//ゲームクリア判定
	if (player_->IsEscape())
	{
		//クリア音声
		goalSE_->PlayNonPlaying();

		//カーソルモードの変更
		input.ChangeCurrsolMode(true);

		//ゲームクリアシーンへ遷移
		sceneManager_.ChangeScene(SceneManager::SCENE_ID::RESULT);
		return;
	}

	//サウンドリスナー情報の更新
	auto& soundMng = SoundManager::GetInstance();
	VECTOR listenerPos = player_->GetTransform().pos_;
	VECTOR listenerDir = player_->GetDirection();
	Set3DSoundListenerPosAndFrontPos_UpVecY(listenerPos, VAdd(listenerPos, listenerDir));


	//各オブジェクト更新
	stage_->Update();
	goalGate_->Update();
	player_->Update();	
	tracker_->Update();
	for (const auto& item : items_) 
	{
		item->Update();
	}
	for (const auto& warpGate : warpGates_) 
	{
		warpGate->Update();
	}
	playerUI_->Update();

	// Effekseerにより再生中のエフェクトを更新する。
	UpdateEffekseer3D();
}

void GameScene::LoadingDraw(void)
{
	//「now loading......」の描画
	DrawNowLoading();
}

void GameScene::NormalDraw(void)
{
	//各オブジェクトの描画
	stage_->Draw();
	goalGate_->Draw();
	goalDoor_->Draw();
	player_->Draw();
	tracker_->Draw();
	operationUI_->Draw();
	for (const auto& item : items_) {
		item->Draw();
	}
	for (const auto& warpGates : warpGates_) {
		warpGates->Draw();
	}

	// Effekseerにより再生中のエフェクトを描画する。
	DrawEffekseer3D();

	//プレイヤーUI
	playerUI_->Draw();

}

bool GameScene::CheckGameOver(void)
{
	//衝突している場合、true
	float dis = static_cast<float>(Utility::Distance(player_->GetTransform().pos_, 
		tracker_->GetTransform().pos_));
	float radious = player_->GetRadious() + tracker_->GetRadious();
	bool isHit = (dis <= radious);

	//追跡者が気絶中でない場合、true
	bool isNotFainting = !tracker_->GetFainting();


	return isHit && isNotFainting;
}

