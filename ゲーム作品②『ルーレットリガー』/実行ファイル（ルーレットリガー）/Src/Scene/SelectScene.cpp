#include <DxLib.h>
#include <cstdlib>
#include "../Application.h"
#include "../Manager/DataManager/UnitDataManager.h"
#include "../Manager/DataManager/BattleDataManager.h"
#include "../Manager/DataManager/SoundManager.h"
#include "../Manager/DeckManager.h"
#include "../Manager/DataManager/SoundManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Common/Vector2.h"

#include "../Utility/Utility.h"
#include "../Utility/DrawShader.h"
#include "../Utility/Button/RectButton.h"
#include "../Utility/Button/CircleButton.h"
#include"../_debug/_DebugDispOut.h"
#include "SelectScene.h"

using PsType = DrawShader::PS_TYPE;
//ステージ、キャラの選択ボックス
const Vector2 SELECT_BOX_POS = { 660,180 };
const Vector2 SELECT_BOX_SIZE = { 560,320 };
//セレクトモード画像
const Vector2 SELECT_MOODE_LOGO_POS = { 680,0 };
const Vector2 SELECT_MOODE_LOGO_SIZE = { 550,550 };
//決定ボタンの座標
const Vector2 DECI_BOX_POS = { 820,610 };

//つかいま
const Vector2 DEVIL_POS = { 0,100 };	//座標
const int DEVIL_SHAKE_VALUE = 30;		//ずらし値


//バックボタンのローカル座標
const Vector2 BACK_BUTTON_LOCAL_POS = { 0, 20 };
//コメントボックスの座標
const Vector2 COMMENT_BOX_POS = { 0, 470 };


//ゲームモード用フォント
constexpr int MODE_FONT_SIZE = 50;
constexpr int MODE_FONT_THICK = 20;
constexpr int MODE_FONT_COLOR = 0xdddddd;
const Vector2 MODE_FONT_OFFSET_POS = { 0, 60 };
//コメント用フォント
constexpr int COMMNET_FONT_SIZE = 20;
constexpr int COMMNET_FONT_THICK = 20;
constexpr int COMMNET_FONT_COLOR = 0xdddddd;
const Vector2 COMMNET_FONT_OFFSET_POS = { 40,40 };

//スクロール速度
constexpr float SCROLL_SPEED = 0.5f;

//ユニット表示
const Vector2 UNIT_SHOW_SIZE = { 200,200 };	//サイズ
const Vector2 UNIT_SHOW_LEFT_POS = { 50,110 };	//左
const Vector2 UNIT_SHOW_CENTER_POS = { 190,10 };	//中
const Vector2 UNIT_SHOW_RIGHT_POS = { 330,110 };	//右


SelectScene::SelectScene(void)
{
	scrollTime_=0.0f;
	isMoveWheel_=false;

	 page_= SELECT_PAGE::MODE;
	 mode_= SELECT_MODE::TITLE;

	devilCmt_="";
	nowMode_="";

	selectImgPos_ = Vector2{};
	cmtPos_ = Vector2{};
	devilPos_ = Vector2{};
	modePos_ = Vector2{};

	backImg_=0;
	deciBtnImg_ = 0;
	nowEnemysNum_ = 0;
	nowStageNum_ = 0;
	selectedBoxImg_ = 0;
	selectedBoxMaskImg_ = 0;
	selectMaskScreen_ = 0;
	cmtBox_ = 0;
	cmtFontHandle_ = 0;
	devilImg_ = 0;
	shakeY_ = 0;
	scrollSE_ = 0;
	modeBox_ = 0;
	modeFontHandle_ = 0;
	modeBackBtnImg_ = 0;
	wheelSign_ = 0;
}

SelectScene::~SelectScene(void)
{
}

void SelectScene::Init(void)
{
	int sx = Application::SCREEN_SIZE_Y;
	int sy = Application::SCREEN_SIZE_Y;

	//ボタン画像
	backImg_ = LoadGraph("Data/Image/UI/BackBtn.png");
	//バックボタンの生成
	backBtn_ = new CircleButton();
	auto backBtn = Vector2{ 0,sy } + BACK_BUTTON_LOCAL_POS;
	backBtn_->Create(backBtn, backImg_, -1);

	//決定ボタン画像
	deciBtnImg_ = LoadGraph("Data/Image/UI/Select/DeciBtnBack.png");
	//決定ボタンの生成
	deciBtn_ = new RectButton();
	deciBtn_->Create(DECI_BOX_POS,deciBtnImg_);

	//モードボタンUI生成
	CreateModeBtn();

	//画像の登録
	bgImg_ = LoadGraph("Data/Image/bg/select.jpg");
	modeBox_ = LoadGraph("Data/Image/UI/ModeBox.png");
	cmtBox_ = LoadGraph("Data/Image/UI/CommentBox.png");

	//選択ボックス画像
	selectedBoxImg_ = LoadGraph("Data/Image/UI/Select/SelectedBack.png");
	selectedBoxMaskImg_ = LoadGraph("Data/Image/UI/Select/SelectedBackMask.png");

	//ステージ情報の登録
	StageDataInit();

	//選択用マスクスクリーン
	selectMaskScreen_ = MakeScreen(
		SELECT_BOX_SIZE.x, SELECT_BOX_SIZE.y, true);

	//ステージ、エネミー選択関連初期化
	isMoveWheel_ = true;		
	selectImgPos_ = { 0,0 };	//背景画像位置
	scrollTime_ = 0.0;			//スクロール経過時間
	wheelSign_ = 0;				//ホイール符号
	nowStageNum_ = 0;			//現ステージ番号
	nowEnemysNum_ = 0;			//現エネミーズ番号
	
								
	//初期化
	totalTime_ = 0.0f;
	//現在ページ
	page_ = SELECT_PAGE::MODE;

	//モードボックス
	modePos_ = { 0,0 };
	nowMode_ = "MODE SELECT";
	modeFontHandle_ = CreateFontToHandle("游明朝", MODE_FONT_SIZE, MODE_FONT_THICK,
		DX_FONTTYPE_ANTIALIASING_4X4);

	//コメントボックス
	cmtPos_ = COMMENT_BOX_POS;
	SetDevilCmt("ゲームモードを選択してくれよな！");
	cmtFontHandle_ = CreateFontToHandle("游明朝", COMMNET_FONT_SIZE, COMMNET_FONT_THICK,
		DX_FONTTYPE_ANTIALIASING_4X4);

	//つかいま
	devilImg_ = LoadGraph("Data/Image/UI/つかいま_口開き.png");
	devilPos_ = DEVIL_POS;
	shakeY_ = 0;


	//モード
	ChangeSelectMode(SELECT_MODE::MAX);

	//ホイール初期化
	GetMouseWheelRotVol();

	//音声関連初期化
	auto& snd = SoundManager::GetInstance();
	scrollSE_	= snd.LoadSound(SoundManager::SOUND_TYPE::SCROLL);
	//BGM再生
	bgmHandle_= snd.LoadSound(SoundManager::SOUND_TYPE::SELECT_BGM);
	snd.PlaySoundBGM(bgmHandle_);


}

void SelectScene::Update(void)
{
	//デルタタイム
	auto delta = SceneManager::GetInstance().GetDeltaTime();
	totalTime_ += delta;

	//音声
	auto& snd = SoundManager::GetInstance();

	//マスコット画像のずらし値
	int moveS = DEVIL_SHAKE_VALUE;
	shakeY_ = sinf(totalTime_) * moveS;

	//各モードボタン更新
	BtnProcess();

	//現在のステージ情報
	StageData& nowStageData =
		stages_.at(static_cast<SELECT_STAGE>(nowStageNum_));
	if (page_ == SELECT_PAGE::MODE)
	{

	}
	else if (page_ == SELECT_PAGE::STAGE)
	{
		//ステージ選択
		SelectSceollProcess();
		//ステージ画像スライド
		ScrollSelectImg(nowStageNum_, static_cast<int>(SELECT_STAGE::MAX));
		//スクロール中は処理しない
		if (!isMoveWheel_)return;

		//決定ボタン
		deciBtn_->Update();
		if (deciBtn_->ButtonDecision())
		{
			SetDevilCmt("ホイールで対戦相手を選べるぜ！\n決まったらバトルスタートだ！");
			//次フェーズに進む
			page_= SELECT_PAGE::ENEMY;
			return;
		}

		//バックボタン
		backBtn_->Update();
		if (backBtn_->ButtonDecision())
		{
			SetDevilCmt("モンスターと戦うモードだぞ\n勝てる相手を選ぶんだな");

			int btnSizeX, btnSizeY;
			GetGraphSize(modeBackBtnImg_, &btnSizeX, &btnSizeY);
			//モードボタン座標変更
			for (auto& mode : modeBtn_)
			{
				mode.second.btn->SetMovedPos({ 0,-btnSizeY });
			}

			//前フェーズに戻る
			page_= SELECT_PAGE::MODE;
			return;
		}
	}
	else
	{
		//対戦相手選択
		SelectSceollProcess();
		//対戦相手画像スライド
		ScrollSelectImg(nowEnemysNum_, nowStageData.enemys.size());

		//スクロール中は処理しない
		if (!isMoveWheel_)return;


		//バックボタン
		backBtn_->Update();
		if (backBtn_->ButtonDecision())
		{
			SetDevilCmt("ホイールでバトルステージを選べるぜ！\n敵の種類が変わるぜ！");
			//前フェーズに戻る
			page_=SELECT_PAGE::STAGE;
			return;
		}

		//決定ボタン
		deciBtn_->Update();
		if (deciBtn_->ButtonDecision())
		{
			//デッキ、バトル情報のセット
			auto deck = DeckManager::GetInstance().GetDeck();
			Deck enemys = nowStageData.enemys[nowEnemysNum_];
			BattleDataManager::GetInstance().SetBattleData({
				deck,enemys,nowStageNum_,0 });


			//シーン遷移
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME, true);
			return;
		}
	}

}

void SelectScene::Draw(void)
{
	auto& sx = Application::SCREEN_SIZE_X;
	auto& sy = Application::SCREEN_SIZE_Y;
	auto& ds = DrawShader::GetInstance();

	//背景
	ds.DrawExtendGraphToShader(
		{ 0,0 }, { sx,sy }, bgImg_, PsType::TEXTURE, COLOR_F{});

	//つかいま描画
	ds.DrawGraphToShader(
		{ devilPos_.x ,devilPos_.y + shakeY_ }, devilImg_, PsType::TEXTURE);

	//モードボックス描画
	DrawModeBox();
	//コメントボックス
	DrawCmtBox();


	//モードボタン
	for (auto& btn : modeBtn_)
	{
		btn.second.btn->Draw();
	}

	if (page_ == SELECT_PAGE::MODE)
	{
		//セレクトモード画像
		if (mode_ != SELECT_MODE::MAX)
		{
			ds.DrawExtendGraphToShader(
				SELECT_MOODE_LOGO_POS, SELECT_MOODE_LOGO_SIZE, modeBtn_.at(mode_).modeImg);
		}
	}
	else if (page_ == SELECT_PAGE::STAGE)
	{
		//ステージ選択
		DrawSelectBoxStage();

		//決定ボタン
		deciBtn_->Draw();

		//バックボタン
		backBtn_->Draw();
	}
	else if (page_ == SELECT_PAGE::ENEMY)
	{
		//選択エネミー描画
		DrawSelectBoxEnemy();

		//バックボタン
		backBtn_->Draw();

		//決定ボタン
		deciBtn_->Draw();
	}
}

void SelectScene::Release(void)
{
	//音声停止
	StopSoundMem(bgmHandle_);

	//画像開放
	DeleteGraph(backImg_);
	DeleteGraph(deciBtnImg_);
	DeleteGraph(bgImg_);
	DeleteGraph(modeBox_);
	DeleteGraph(cmtBox_);
	DeleteGraph(selectedBoxImg_);
	DeleteGraph(modeBackBtnImg_);


	backBtn_->Release();
	delete backBtn_;

	deciBtn_->Release();
	delete deciBtn_;
}

void SelectScene::DrawModeBox(void)
{
	DrawGraph(modePos_.x, modePos_.y, modeBox_, true);
	int x, y;
	GetGraphSize(modeBox_, &x, &y);

	auto len = Utility::CalcStringLength(nowMode_, modeFontHandle_) / 2;
	DrawFormatStringToHandle(
		modePos_.x + (x / 2) - len,
		modePos_.y + MODE_FONT_OFFSET_POS.y,
		MODE_FONT_COLOR, modeFontHandle_, nowMode_.c_str());
}

void SelectScene::DrawCmtBox(void)
{
	DrawGraph(cmtPos_.x, cmtPos_.y, cmtBox_, true);

	Vector2 offset = Vector2{ cmtPos_.x,cmtPos_.y } + COMMNET_FONT_OFFSET_POS;
	DrawFormatStringToHandle(
		offset.x, offset.y,
		COMMNET_FONT_COLOR, cmtFontHandle_, devilCmt_.c_str());

}

void SelectScene::BtnProcess()
{
	//ボタン更新
	for (auto& btn : modeBtn_)
	{
		btn.second.btn->Update();
	}

	//ボタン：マウスがボタン上にあるか判断
	for (auto& btn : modeBtn_)
	{
		if (btn.second.btn->MouseOnButton())
		{
			ChangeSelectMode(btn.first);
		}
	}

	//ボタン：クリック判定
	for (auto& btn : modeBtn_)
	{
		if (!btn.second.btn->ButtonDecision())continue;

		//ボタンが押された場合、そのボタン処理を行う
		SelectBtnProcess();
	}
}

void SelectScene::BattleBtnProcess(void)
{
	SetDevilCmt("ホイールでバトルステージを選べるぜ！\n敵の種類が変わるぜ！");

	//ステージ選択へ移動
	page_= SELECT_PAGE::STAGE;

	//モードボタン座標の変更
	int btnSizeX, btnSizeY;
	GetGraphSize(modeBackBtnImg_, &btnSizeX, &btnSizeY);
	for (auto& mode : modeBtn_)
	{
		mode.second.btn->SetMovedPos({ 0,btnSizeY });
	}
}

void SelectScene::EditBtnProcess(void)
{
	//シーン遷移
	SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::DECK_EDIT, true);
}

void SelectScene::TitleBtnProcess(void)
{
	//シーン遷移
	SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE, true);
}

void SelectScene::CreateModeBtn(void)
{
	//スクリーンサイズ
	auto& sx = Application::SCREEN_SIZE_X;
	auto& sy = Application::SCREEN_SIZE_Y;

	//モードボタンの背面画像
	modeBackBtnImg_ = LoadGraph("./Data/Image/UI/ModeBtnImg.png");
	int btnSizeX, btnSizeY;
	GetGraphSize(modeBackBtnImg_, &btnSizeX, &btnSizeY);


	std::string path = "Data/Image/UI/";
	int n = 0;
	// ボタン生成ラムダ式
	auto createBtn = [&](SELECT_MODE mode, std::string btnImgPath, std::string modeImgPath) {

		//モード画像
		std::string fileName = path + modeImgPath;
		int modeImg = LoadGraph(fileName.c_str());

		//座標
		Vector2 pos = {
			(sx / 2) - (btnSizeX ) - (btnSizeX / 2) + (btnSizeX * n)  ,
			sy - btnSizeY };
		//ボタン画像
		fileName = path + btnImgPath;
		int btnImg = LoadGraph(fileName.c_str());

		//ボタン生成
		RectButton* b = new RectButton();
		b->Create(pos, modeBackBtnImg_, btnImg);

		//挿入
		ModeData md = { modeImg,b };
		modeBtn_.emplace(std::make_pair(mode, md));

		n++;
	};


	createBtn(SELECT_MODE::BATTLE,		"バトル.png",		"Select/Mode_Battle.png");
	createBtn(SELECT_MODE::DECK_EDIT,	"デッキ編集.png",	"Select/Mode_DeckEdit.png");
	createBtn(SELECT_MODE::TITLE,		"退出.png",			"Select/Mode_Title.png");
}

void SelectScene::SelectBtnProcess(void)
{
	switch (mode_)
	{
	case SelectScene::SELECT_MODE::BATTLE:
		BattleBtnProcess();
		break;
	case SelectScene::SELECT_MODE::DECK_EDIT:
		EditBtnProcess();
		break;
	case SelectScene::SELECT_MODE::RULE_BOOK:
		break;
	case SelectScene::SELECT_MODE::CREDIT:
		break;
	case SelectScene::SELECT_MODE::TITLE:
		TitleBtnProcess();
		break;
	case SelectScene::SELECT_MODE::MAX:
		break;
	default:
		break;
	}
}

void SelectScene::ChangeSelectMode(const SELECT_MODE& mode)
{
	//同じだった場合、何もしない
	if (mode_ == mode)return;

	mode_ = mode;

	//音声を鳴らす

	switch (mode_)
	{
	case SelectScene::SELECT_MODE::BATTLE:
		SetDevilCmt("モンスターと戦うモードだぞ\n勝てる相手を選ぶんだな");

		break;
	case SelectScene::SELECT_MODE::DECK_EDIT:
		SetDevilCmt("バトルユニットを選べるぜ");

		break;
	case SelectScene::SELECT_MODE::RULE_BOOK:
		SetDevilCmt("現在、開発中です");

		break;
	case SelectScene::SELECT_MODE::CREDIT:
		SetDevilCmt("現在、開発中です");

		break;
	case SelectScene::SELECT_MODE::TITLE:
		SetDevilCmt("タイトルに戻るぜ");

		break;
	case SelectScene::SELECT_MODE::MAX:
		break;
	default:
		break;
	}
}

void SelectScene::SetDevilCmt(const std::string& cmt)
{
	devilCmt_ = cmt;
}

void SelectScene::StageDataInit()
{
	StageData data;

	//火
	data.backImg = LoadGraph("Data/Image/bg/red.jpg");
	data.enemys.push_back({ 0, 1, 2 });
	data.enemys.push_back({ 3, 4, 5 });
	data.enemys.push_back({ 6, 7, 8 });
	data.enemys.push_back({ 9,-1,-1 });
	stages_.insert(std::make_pair(SELECT_STAGE::VOLCANO, data));
	data.enemys.clear();

	//水
	data.backImg = LoadGraph("Data/Image/bg/blue.jpg");
	data.enemys.push_back({ 10,11,12 });
	data.enemys.push_back({ 13,14,15 });
	data.enemys.push_back({ 16,17,18 });
	data.enemys.push_back({ 19,-1,-1 });
	stages_.insert(std::make_pair(SELECT_STAGE::OCEAN, data));
	data.enemys.clear();

	//地
	data.backImg = LoadGraph("Data/Image/bg/earth.jpg");
	data.enemys.push_back({ 20,21,22 });
	data.enemys.push_back({ 23,24,25 });
	data.enemys.push_back({ 26,27,28 });
	data.enemys.push_back({ 29,-1,-1 });
	stages_.insert(std::make_pair(SELECT_STAGE::WILDRNESS, data));
	data.enemys.clear();

	//風
	data.backImg = LoadGraph("Data/Image/bg/green.jpg");
	data.enemys.push_back({ 30,31,32 });
	data.enemys.push_back({ 33,34,35 });
	data.enemys.push_back({ 36,37,38 });
	data.enemys.push_back({ 39,-1,-1 });
	stages_.insert(std::make_pair(SELECT_STAGE::FOREST, data));
	data.enemys.clear();

	//聖
	data.backImg = LoadGraph("Data/Image/bg/holy.jpg");
	data.enemys.push_back({ 40,41,42 });
	data.enemys.push_back({ 43,44,45 });
	data.enemys.push_back({ 46,47,48 });
	data.enemys.push_back({ 49,-1,-1 });
	stages_.insert(std::make_pair(SELECT_STAGE::SANCTUARY, data));
	data.enemys.clear();

	//闇
	data.backImg = LoadGraph("Data/Image/bg/dark.jpg");
	data.enemys.push_back({ 50,51,52 });
	data.enemys.push_back({ 53,54,55 });
	data.enemys.push_back({ 56,57,58 });
	data.enemys.push_back({ 59,-1,-1 });
	stages_.insert(std::make_pair(SELECT_STAGE::ABYSS, data));
	data.enemys.clear();

}

void SelectScene::SelectSceollProcess(void)
{
	//スクロール中は処理しない
	if (!isMoveWheel_)return;

	//ホイール操作
	int wheelValue = GetMouseWheelRotVol();
	if (wheelValue == 0)return;

	//SE再生
	auto& snd = SoundManager::GetInstance();
	snd.PlaySoundBack(scrollSE_);

	//画像の移動方向を求める
	wheelSign_ = wheelValue / abs(wheelValue);
	//ホイール動作を拒否状態にする
	isMoveWheel_ = false;
}

void SelectScene::ScrollSelectImg(int& num, const int& size)
{
	if (isMoveWheel_)return;

	//デルタタイム
	scrollTime_ += SceneManager::GetInstance().GetDeltaTime();

	int movePow = wheelSign_ * SELECT_BOX_SIZE.y; //移動量
	//移動の線形補間
	float time = scrollTime_ / SCROLL_SPEED;
	selectImgPos_.y = Utility::Lerp(0, movePow, time);

	if (time >= 1.0f)
	{
		scrollTime_ = 0.0f;
		selectImgPos_.y = 0;
		//ホイール動作可能状態にする
		isMoveWheel_ = true;

		//番号の決定
		num -= wheelSign_;
		num = Utility::Wrap(num, 0, size);
	}
}

void SelectScene::DrawSelectBoxStage(void)
{
	auto& ds = DrawShader::GetInstance();	
	Vector2 pos = SELECT_BOX_POS;
	Vector2 size = SELECT_BOX_SIZE;

	//選択ボックス(後ろ枠)
	ds.DrawExtendGraphToShader(pos, size, selectedBoxImg_);

	//マスク画面に背景画像を描画
	SetDrawScreen(selectMaskScreen_);
	ClearDrawScreen();

	//現在ステージ画像
	int bgImg = stages_.at(static_cast<SELECT_STAGE>(nowStageNum_)).backImg;
	ds.DrawExtendGraphToShader(selectImgPos_, size, bgImg);

	//スクロール中
	if (!isMoveWheel_)
	{
		Vector2 offset = { 0,size.y };
		offset *= wheelSign_ * -1;
		//スクロール先の背景画像
		int nextNum = nowStageNum_ - wheelSign_;
		nextNum = Utility::Wrap(
			nextNum, 0, static_cast<int>(SELECT_STAGE::MAX));
		bgImg = stages_.at(static_cast<SELECT_STAGE>(nextNum)).backImg;
		ds.DrawExtendGraphToShader(selectImgPos_ + offset , size, bgImg);
	}

	//描画先指定
	SetDrawScreen(DX_SCREEN_BACK);

	//切り抜き画像を描画
	ds.DrawGraphAndSubToShader(
		pos,size,
		selectMaskScreen_, selectedBoxMaskImg_, PsType::MASK, COLOR_F{});
}

void SelectScene::DrawSelectBoxEnemy(void)
{
	auto& ds = DrawShader::GetInstance();
	Vector2 pos = SELECT_BOX_POS;
	Vector2 size = SELECT_BOX_SIZE;

	//選択ボックス(後ろ枠)
	ds.DrawExtendGraphToShader(pos, size, selectedBoxImg_);

	//マスク画面に背景画像を描画
	SetDrawScreen(selectMaskScreen_);
	ClearDrawScreen();


	//バトルエネミー
	auto& uMng = UnitDataManager::GetInstance();

	StageData& data =
		stages_.at(static_cast<SELECT_STAGE>(nowStageNum_));//現在のステージ情報
	auto drawEnemy = [&](int enemysNum ,int unitNum, Vector2 pos) {
		int i = data.enemys[enemysNum].dArray_[unitNum];
		if (i == -1)return;

		//ユニット画像取得
		int uImg = uMng.GetUnitImg(i);
		ds.DrawExtendGraphToShader(pos, UNIT_SHOW_SIZE,uImg);
	};

	//敵対エネミー画像の描画
	drawEnemy(nowEnemysNum_, 2, selectImgPos_ + UNIT_SHOW_RIGHT_POS);
	drawEnemy(nowEnemysNum_, 1, selectImgPos_ + UNIT_SHOW_LEFT_POS);
	drawEnemy(nowEnemysNum_, 0, selectImgPos_ + UNIT_SHOW_CENTER_POS);

	//スクロール中
	if (!isMoveWheel_)
	{
		Vector2 offset = { 0,size.y };
		offset *= wheelSign_ * -1;
		//敵対エネミー画像の描画
		int nextNum = nowEnemysNum_ - wheelSign_;
		nextNum = Utility::Wrap(nextNum, 0, data.enemys.size());
		drawEnemy(nextNum, 2, selectImgPos_ + offset + UNIT_SHOW_RIGHT_POS);
		drawEnemy(nextNum, 1, selectImgPos_ + offset + UNIT_SHOW_LEFT_POS);
		drawEnemy(nextNum, 0, selectImgPos_ + offset + UNIT_SHOW_CENTER_POS);
	}


	//描画先指定
	SetDrawScreen(DX_SCREEN_BACK);

	//切り抜き画像を描画
	ds.DrawGraphAndSubToShader(
		pos, size,
		selectMaskScreen_, selectedBoxMaskImg_, PsType::MASK, COLOR_F{});
}




