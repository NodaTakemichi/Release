#include <cmath>
#include <DxLib.h>
#include "../Application.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/DataManager/EffectManager.h"
#include "../Manager/DataManager/SoundManager.h"
#include "../Manager/DeckManager.h"
#include "../Manager/DataManager/BattleDataManager.h"

#include "../Utility/Utility.h"
#include "../Utility/DrawShader.h"
#include "../Battle/BattleSystem.h"
#include "../Battle/DeathStaging.h"

#include "../Object/Unit/Status/Command.h"
#include "../Object/Unit/Status/Buff.h"
#include "../Object/Unit/UnitManager.h"
#include "../Object/Unit/UnitBase.h"
#include "../Object/Roulette.h"
#include "../Object/GameUI.h"

#include "../_debug/_DebugConOut.h"
#include "../_debug/_DebugDispOut.h"

#include "GameScene.h"

//UI用フォント
constexpr int UI_FONT_SIZE = 36;
constexpr int UI_FONT_EDGE_SIZE = 2;
constexpr int UI_FONT_COLOR = 0xdddddd;
const Vector2 UI_POS = { 300,120 };
//画面揺れ
constexpr float AMPLITUDE_VALUE = 50.0f;		//最大揺れ幅
constexpr int AMPLITUDE_ADJUSTMENT = 3;			//揺れ幅調整値
constexpr float AMPLITUDE_DECREASE = 0.9f;		//減少値
//フレーム表示座標
const Vector2 FRAME_SHOW_POS = { -20, 110 };
//スタート画像表示座標
const Vector2 START_IMAGE_SHOW_POS = { 800,500 };
//現在の行動ユニットのターン文字カラー
constexpr int PLAUER_UNIT_TURN_TEXT_COLOR = 0x00ff00;	//味方；緑
constexpr int ENEMY_UNIT_TURN_TEXT_COLOR = 0xff0000;	//敵：赤
//クリック指示テキスト
constexpr float OPERATION_SHOE_TIME = 3.0f;		//表示時間
constexpr float OPERATION_SHOE_INTERVAL = 0.5f;	//表示間隔
constexpr int OPERATION_SHOE_POS_Y = 600;	//表示位置Y
constexpr int OPERATION_SHOE_COLOR = 0xffa500;	//カラー
constexpr int OPERATION_SHOE_COLOR_EDGE = 0x0a0a0a;	//エッジカラー
//セピア完了時間
constexpr float SEPIA_FINISH_TIME = 2.0f;	//エッジカラー

GameScene::GameScene(void)
{
	screenPsBuf_ = COLOR_F{};
	screenPos_ = Vector2{};
	topFrameImg_=0;
	mainScreen_ = 0;
	frame_=0;
	winBgm_=0;	
	loseBgm_=0;		
	resultBgm_=0;		
	turnChangeSE_=0;	
	uiFontHandle_=0;
	resultImg_=0;		
	winImg_=0;	
	loseImg_=0;	
	battleStartImg_=0;
	bStartFadeImg_=0;
	isPlayerWin_=false;
	actUnitAoutm_ = true;
	resutlTime_=0.0f;
	amplitude_=0.0f;


}

GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
{

	// 死亡演出初期化
	DeathStaging::CreateInstance();

	//ルーレット
	roulette_ = new Roulette();
	roulette_->Init();

	//UI
	GameUi_ = new GameUI();
	GameUi_->Init();

	//バトルシステム
	battleSys_ = new BattleSystem();
	battleSys_->Init();

	//ユニットマネージャー
	unitMng_ = new UnitManager();
	unitMng_->Init();
	//ユニットの生成
	auto& bd = BattleDataManager::GetInstance().GetBattleData();
	unitMng_->CreateUnit(bd.pDeck, bd.eDeck);



	//背景画像の登録
	backImgs_.push_back(LoadGraph("Data/Image/bg/red.jpg"));
	backImgs_.push_back(LoadGraph("Data/Image/bg/blue.jpg"));
	backImgs_.push_back(LoadGraph("Data/Image/bg/earth.jpg"));
	backImgs_.push_back(LoadGraph("Data/Image/bg/green.jpg"));
	backImgs_.push_back(LoadGraph("Data/Image/bg/holy.jpg"));
	backImgs_.push_back(LoadGraph("Data/Image/bg/dark.jpg"));

	bgImg_ = backImgs_[bd.bgNum];
	frameImg_ = LoadGraph("./Data/Image/UI/frame_full.png");
	topFrameImg_= LoadGraph("./Data/Image/UI/Turn.png");


	//フェーズの変更
	ChangeGamePhase(GAME_PHASE::BATTLE_START);

	//サウンドの登録
	auto& snd = SoundManager::GetInstance();
	bgmHandle_ = snd.LoadSound(SoundManager::SOUND_TYPE::BATTLE1_BGN);
	winBgm_ = snd.LoadSound(SoundManager::SOUND_TYPE::VICTORY_BGN);
	loseBgm_ = snd.LoadSound(SoundManager::SOUND_TYPE::LOSE_BGN);
	resultBgm_ = 0;
	turnChangeSE_ = snd.LoadSound(SoundManager::SOUND_TYPE::TURN_CHANGE);
	//BGMの再生
	snd.PlaySoundBGM(bgmHandle_);

	//初期化
	resutlTime_ = 0.0f;
	//スクリーン関連
	mainScreen_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);
	psType_ = DrawShader::PS_TYPE::TEXTURE();
	screenPsBuf_ = COLOR_F{};
	
	screenPos_ = {0,0};	//スクリーン座標
	amplitude_=0.0f;	//スクリーン振幅
	frame_=0;			//フレーム

	//結果
	isPlayerWin_ = true;
	//結果画像
	resultImg_ = 0;
	loseImg_ = LoadGraph("Data/Image/UI/Game/Lose.png");
	winImg_ = LoadGraph("Data/Image/UI/Game/Win.png");

	//バトルスタート用画像
	battleStartImg_ = LoadGraph("Data/Image/UI/Game/BattleStart.png");
	bStartFadeImg_ = LoadGraph("Data/Image/Fade/136.png");


	//UIフォント
	uiFontHandle_ = CreateFontToHandle("游明朝", UI_FONT_SIZE, -1,
		DX_FONTTYPE_ANTIALIASING_EDGE_4X4, -1, UI_FONT_EDGE_SIZE);
} 

void GameScene::Update(void)
{
	//デルタタイム
	auto delta = SceneManager::GetInstance().GetDeltaTime();
	totalTime_ += delta;


	//入力
	auto& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		// バトルの中断：シーン遷移
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::SELECT, true);
		return;
	}

	//揺れ演出検知
	if (DeathStaging::GetInstance().CheckStartStanging())
	{
		amplitude_ = AMPLITUDE_VALUE;
	}
	//死亡演出の更新
	DeathStaging::GetInstance().Update();


	//更新
	unitMng_->Update();
	GameUi_->Update();

	//フェーズ別更新
	switch (phase_)
	{
	case GameScene::GAME_PHASE::BATTLE_START:
		//ロゴフェードが往復したとき、バトルシーンへ遷移、
		if (START_IMAGE_FADE_TIME * 2.0f <= totalTime_) {
			//スタート
			ChangeGamePhase(GAME_PHASE::RULLET_TIME);
		}

		break;
	case GameScene::GAME_PHASE::RULLET_TIME:
		UpdateRouTime();
		break;
	case GameScene::GAME_PHASE::AIM:
		UpdateAIM();
		break;
	case GameScene::GAME_PHASE::EFFECT:
		UpdateEffect();
		break;
	case GameScene::GAME_PHASE::BATTLE:
		UpdateBattle();
		break;
	case GameScene::GAME_PHASE::BUFF_EFFECT :
		UpdateBuffEffect();
		break;
	case GameScene::GAME_PHASE::TURN_END:
		UpdateTurnEnd();
		break;
	case GameScene::GAME_PHASE::GAME_END:
		//決着：ゲームシーン終了
		UpdateGameEnd();
		break;
	default:
		break;
	}


}

void GameScene::Draw(void)
{
	int sx = Application::SCREEN_SIZE_X;
	int sy = Application::SCREEN_SIZE_Y;

	auto& ds = DrawShader::GetInstance();

	//スクリーンセット
	SetDrawScreen(mainScreen_);
	ClearDrawScreen();


	//背景
	ds.DrawExtendGraphToShader({ 0,0 }, {sx,sy}, bgImg_);
	//フレーム
	DrawGraph(FRAME_SHOW_POS.x, FRAME_SHOW_POS.y, frameImg_, true);
	DrawGraph(0, 0, topFrameImg_, true);

	//オブジェクト
	unitMng_->Draw();
	roulette_->Draw();

	//UI関連
	GameUi_->Draw();


	//スクリーンセット
	SetDrawScreen(DX_SCREEN_BACK);

	//揺れ計算
	CalcScreenShake();
	//スクリーン描画
	ds.DrawGraphToShader(
		screenPos_, mainScreen_, psType_, screenPsBuf_);

	//フェーズ別描画
	switch (phase_)
	{
	case GameScene::GAME_PHASE::BATTLE_START:
		DrawBattleStart();
		break;
	case GameScene::GAME_PHASE::RULLET_TIME:
		break;
	case GameScene::GAME_PHASE::AIM:
		GameUi_->DrawActivSkill();
		break;
	case GameScene::GAME_PHASE::EFFECT:
		GameUi_->DrawActivSkill();
		break;
	case GameScene::GAME_PHASE::BATTLE:
		break;
	case GameScene::GAME_PHASE::BUFF_EFFECT:
		break;
	case GameScene::GAME_PHASE::TURN_END:
		break;
	case GameScene::GAME_PHASE::GAME_END:
		//結果描画
		DrawResult();
		break;
	}

	//死亡演出
	DeathStaging::GetInstance().PlayDeathStaging();

#ifdef DEBUG
	DrawPosition();
#endif // _DEBUG


}

void GameScene::Release(void)
{
	//死亡演出クラスの開放
	DeathStaging::GetInstance().Release();

	//結果BGMの停止
	StopSoundMem(resultBgm_);
	StopSoundMem(bgmHandle_);

	//破棄
	unitMng_->Release();
	delete unitMng_;
	roulette_->Release();
	delete roulette_;
	GameUi_->Release();
	delete GameUi_;
	battleSys_->Release();
	delete battleSys_;

	//画像解放
	DeleteGraph(bgImg_);
	DeleteGraph(frameImg_);
	DeleteGraph(topFrameImg_);

	DeleteGraph(loseImg_);
	DeleteGraph(winImg_);
	DeleteGraph(resultImg_);

	DeleteGraph(battleStartImg_);
	DeleteGraph(bStartFadeImg_);


}

void GameScene::UpdateRouTime(void)
{
	//ルーレットの更新
	roulette_->Update();

	//ルーレットの停止操作（ユニットによって自動か手動か決める）
	roulette_->StopRoulette(actUnitAoutm_);

	//バフ判断
	//行動ユニット
	if (unitMng_->GetActivUnit()->CheckOwnBuff(Buff::BUFF_TYPE::PALALYSIS))
	{
		printf(unitMng_->GetActivUnit()->GetUnitName().c_str());
		printf("はマヒしている\n\n");

		//麻痺状態の場合、バフエフェクト再生後ターンエンドする
		nextPhase_ = GAME_PHASE::TURN_END;
		ChangeGamePhase(GAME_PHASE::BUFF_EFFECT);
		return;
	}

	//ルーレットが停止したら、フェーズ移動
	bool next = roulette_->GetRouStop();
	if(next)ChangeGamePhase(GAME_PHASE::AIM);
	
}

void GameScene::UpdateAIM(void)
{	
	//対象を選択したら、エフェクト再生に進む
	bool next = battleSys_->SelectUnit(actUnitAoutm_);
	if(next)ChangeGamePhase(GAME_PHASE::EFFECT);
}

void GameScene::UpdateEffect(void)
{
	//エフェクト再生が終了したかどうか判断
	bool next = EffectManager::GetInstance().FinishEffect();

	//コマンドタイプによって、フェーズ先を変更
	if (next) {
		//エフェクトアニメーションが終了したら
		if (roulette_->GetCmd()->GetCmdType() == Command::CMD_TYPE::BUFF)
		{
			//バフエフェクトフェーズに進む
			nextPhase_ = GAME_PHASE::NONE;
			ChangeGamePhase(GAME_PHASE::BUFF_EFFECT);
		}
		else
		{
			//バトルフェーズに進む
			ChangeGamePhase(GAME_PHASE::BATTLE);
		}
	}
}

void GameScene::UpdateBattle(void)
{
	//ダメージ減少が終了したかどうか判断
	bool next = battleSys_->FinishedDecHP();

	//死亡演出中の場合、進行しない
	if(DeathStaging::GetInstance().PlayingStaging())return;

	//バトル（ダメージ減少）が終了したら、ターン終了に進む
	if (next)ChangeGamePhase(GAME_PHASE::TURN_END);
}

void GameScene::UpdateBuffEffect(void)
{
	//(付与時)バフエフェクト処理
	bool next = battleSys_->FinishedBuffEffect();

	//状態異常シェーダー観測地点
	nextPhase_ = GAME_PHASE::TURN_END;

	//バフエフェクト終了時、次のフェーズに進む
	if(next)ChangeGamePhase(nextPhase_);
}

void GameScene::UpdateTurnEnd(void)
{
	//ダメージ減少が終了したかどうか判断（毒ダメージ）
	bool next = unitMng_->GetActivUnit()->DecHpProcess();


	//全滅ならばゲーム終了、
	//そうでない且、必要処理終了後ならルーレットフェーズに進む。
	if (next) 
	{
		if (unitMng_->IsAnniUnit())ChangeGamePhase(GAME_PHASE::GAME_END);
		else 
		{
			//ターンチェンジ
			auto& snd = SoundManager::GetInstance();
			snd.PlaySoundBack(turnChangeSE_);

			ChangeGamePhase(GAME_PHASE::RULLET_TIME);
		}
	}
}

void GameScene::UpdateGameEnd(void)
{
	//結果画面：経過時間
	resutlTime_ += SceneManager::GetInstance().GetDeltaTime();

	//入力
	auto& ins = InputManager::GetInstance();
	// シーン遷移
	if (ins.IsClickMouseLeft())
	{
		if (isPlayerWin_)
		{
			//リザルトシーンへ遷移
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::RESULT, true);
		}
		else
		{
			//セレクトシーンへ遷移
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::SELECT, true);
		}
	}

}

void GameScene::ChangeGamePhase(GAME_PHASE phase)
{
	//フェーズ
	phase_ = phase;

	switch (phase_)
	{
	case GameScene::GAME_PHASE::BATTLE_START: {
		break;
	}
	case GameScene::GAME_PHASE::RULLET_TIME: {
		//選択ユニットのリセット
		battleSys_->ResetSelectUnits();

		//行動ユニットの整理
		unitMng_->ChangeActivUnit();

		//行動ユニット
		//ルーレットの停止が手動か自動か判断
		actUnitAoutm_ = (unitMng_->GetActivUnit()->GetUnitType() == UnitBase::UNIT_TYPE::ENEMY);

		//ルーレットにコマンド技をセット
		roulette_->SetCommand(unitMng_->GetCommand());
		//ルーレット状況のリセット
		roulette_->ResetRouSpin();


		//現在の行動ユニットのターン文字 
		bool turn = unitMng_->GetActivUnit()->GetUnitType() == UnitBase::UNIT_TYPE::PLAYER;
		int color = turn ? PLAUER_UNIT_TURN_TEXT_COLOR : ENEMY_UNIT_TURN_TEXT_COLOR;
		auto& name = unitMng_->GetActivUnit()->GetUnitName();
		GameUi_->SetTurnString(name, color);

		break;
	}
	case GameScene::GAME_PHASE::AIM: {
		//決定したコマンドをゲームUIに受け渡す
		std::string cmd = roulette_->GetCmd()->GetName();
		GameUi_->SetCmdName(cmd);

		//バトルデータに各情報を渡す
		battleSys_->SetBattleData(unitMng_->GetActivUnit(),
			roulette_->GetCmd(), unitMng_->GetUnits());

		//ホイールのリセット
		GetMouseWheelRotVol();

		//コマンドの選択対象を判断する
		battleSys_->CheckSelectTarget();

		//操作が自動の場合、ユニットをランダムに決める
		if (actUnitAoutm_)battleSys_->SetRandUnit();

		break;
	}
	case GameScene::GAME_PHASE::EFFECT: {

		//コマンドからエフェクト番号の取得
		int num = roulette_->GetCmd()->GetEffectNum();

		//ターゲットの座標取得(ターゲットが存在しない場合、自分座標)
		Vector2 pos = unitMng_->GetActivUnit()->GetUnitPos();
		auto& target = battleSys_->GetTargetUnit();

		if (target.size() <= 0)
		{
			//エフェクト再生(ターゲットが存在しない場合)
			EffectManager::GetInstance().PlayEffect(num, pos);
			return;
		}

		for (auto& unit : target)
		{
			//ユニットの座標を取得
			pos=unit->GetUnitPos();
			//エフェクト再生
			EffectManager::GetInstance().PlayEffect(num, pos);
		}

		break;
	}
	case GameScene::GAME_PHASE::BATTLE: {
		//コマンド処理
		battleSys_->CmdProcess();

		break;
	}
	case GameScene::GAME_PHASE::BUFF_EFFECT: {
		//コマンド処理をしない
		if (nextPhase_ == GameScene::GAME_PHASE::TURN_END)return;

		//コマンド処理
		battleSys_->CmdProcess();
		break;
	}
	case GameScene::GAME_PHASE::TURN_END: {
		//行動ユニットのエンド処理
		unitMng_->GetActivUnit()->TurnEndProcess();

		break;
	}
	case GameScene::GAME_PHASE::GAME_END: {
		//勝者判定
		isPlayerWin_ = CheckVictory();


		//判定画像のセット
		SettingScreenShader();


		//音楽の再生
		StopSoundMem(bgmHandle_);
		resultBgm_ = isPlayerWin_ ? winBgm_ : loseBgm_;
		auto& snd = SoundManager::GetInstance();
		snd.PlaySoundBGM(resultBgm_);


		break;
	}
	}
}

void GameScene::CalcScreenShake(void)
{
	//スクリーンの座標
	if (amplitude_ > 0.0f) {
		//スクリーン揺らし
		screenPos_.x = ((frame_ % AMPLITUDE_ADJUSTMENT) - 1) * amplitude_;
		screenPos_.y = (((frame_ + rand()) % AMPLITUDE_ADJUSTMENT) - 1) * amplitude_;
	}

	//揺れを徐々に小さくしていく
	amplitude_ *= AMPLITUDE_DECREASE;
	if (amplitude_ < 1.0f) {
		amplitude_ = 0.0f;
	}
	++frame_;
}

bool GameScene::CheckVictory(void)
{
	bool win = false;
	for (auto& unit : unitMng_->GetUnits())
	{
		if (!unit->IsAlive())continue;
		if (unit->GetUnitType() == UnitBase::UNIT_TYPE::PLAYER)
		{
			win = true;
			break;
		}
	}

	return win;
}

void GameScene::DrawBattleStart(void)
{
	//画面サイズ
	auto sx = Application::SCREEN_SIZE_X / 2;
	auto sy = Application::SCREEN_SIZE_Y / 2;

	auto& ds = DrawShader::GetInstance();

	Vector2 size = START_IMAGE_SHOW_POS;
	Vector2 pos = { sx - size.x / 2,sy - size.y / 2 };

	//パーセント
	float time = START_IMAGE_FADE_TIME;
	float par =  totalTime_/ time;
	if (par >= 1.0f)
	{
		par -= totalTime_ - time;
	}
	COLOR_F buf = COLOR_F{ par };

	ds.DrawGraphAndSubToShader(pos, size, battleStartImg_,bStartFadeImg_,
		DrawShader::PS_TYPE::FADE_TEXTURE,buf);
}

void GameScene::DrawResult(void)
{
	//画面サイズ
	auto sx = Application::SCREEN_SIZE_X / 2;
	auto sy = Application::SCREEN_SIZE_Y / 2;

	auto& ds = DrawShader::GetInstance();

	//定数バッファ
	if (!isPlayerWin_)
	{
		//画面のセピア調
		screenPsBuf_ = COLOR_F{ resutlTime_ ,resutlTime_ / SEPIA_FINISH_TIME };
	}

	//勝敗文字描画
	int rx, ry;
	GetGraphSize(resultImg_, &rx, &ry);
	Vector2 pos = { sx - rx / 2, sy - ry / 2 };
	COLOR_F buf = COLOR_F{ totalTime_ };
	ds.DrawGraphToShader(
		pos, resultImg_, 
		DrawShader::PS_TYPE::DISTORTION , buf);


	//「クリックしてね」
	std::string inst = "クリックしてね";
	auto len = Utility::CalcStringLength(inst, uiFontHandle_) / 2;
	if (fmodf(totalTime_, OPERATION_SHOE_TIME) >= OPERATION_SHOE_INTERVAL)
	{
		DrawStringToHandle(
			sx - len, OPERATION_SHOE_POS_Y, inst.c_str(),
			OPERATION_SHOE_COLOR,
			uiFontHandle_,
			OPERATION_SHOE_COLOR_EDGE
		);
	}
}

void GameScene::SettingScreenShader(void)
{
	//スクリーン画面シェーダーの設定
	if (!isPlayerWin_)
	{
		//負けた場合、画面をモノクロにする
		psType_ = DrawShader::PS_TYPE::SIMPLE_SEPIA;
	}

	//画像選択
	resultImg_ = isPlayerWin_ ? winImg_ : loseImg_;
}

