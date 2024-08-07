#include <DxLib.h>
#include <cmath>
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/DeckManager.h"
#include "../Manager/DataManager/SoundManager.h"
#include "../Utility/Utility.h"
#include "../Utility/Button/CircleButton.h"
#include "../Utility/Button/RectButton.h"
#include "../Utility/Button/UnitButton.h"
#include "../Utility/Button/CmdButton.h"

#include"../_debug/_DebugConOut.h"
#include"../_debug/_DebugDispOut.h"
#include "EditScene.h"

//マイデッキの表示座標、オフセット値
const Vector2 FIRST_UNIT_POOL_POS = { 106, 291 };
const Vector2 OFFSET_UNIT_POOL_POS = { 151, 207 };
//カードプールの表示座標、オフセット値
const Vector2 FIRST_UNIT_DECK_POS = { 336, 47 };
constexpr int OFFSET_UNIT_DECK_POS = 148;
//デッキプールの最大表示数
constexpr int DECK_POOL_MAX = 10;
constexpr int DECK_POOL_WIGTH_NUM = 5;	//横方向の最大数


//ページボタン座標
const Vector2 NEXT_PAUGE_BTN_POS = { 840,440 };		//次ページボタン：座標
const Vector2 BACK_PAUGE_BTN_POS = { 60,440 };		//前ページボタン：座標
const Vector2 PAUGE_BTN_SIZE = { 30, 80 };		//大きさ

//ステータス用フォント
constexpr int STUTAS_FONT_SIZE = 20;
constexpr int STUTAS_FONT_THICK = 20;
constexpr int STUTAS_FONT_COLOR = 0xe6e6e6;

//コマンド表示
const Vector2 CMD_SHOE_SIZE = { 163,40 };			//大きさ
const Vector2 CMD_SHOE_BASE_POS = { 915, 500 };		//ベース座標
constexpr int CMD_SHOE_NUM = 8;						//コマンド数
//ステータス表示
const Vector2 UNIT_NAME_SHOE_POS = { 1080,80 };			//ユニット名表示座標
const Vector2 UNIT_IMAGE_SHOE_POS = { 950,120 };		//ユニット画像表示座標
const Vector2 STATUS_SHOE_OFFSET = { 140,50 };			//ステータスオフセット値
const Vector2 ICON_SHOW_POS = { 960,390 };				//アイコン表示座標
const Vector2 STATUS_TEXT_SHOW_OFFSET = { 40,5 };		//ステータス値表示オフセット
constexpr int STATUS_WIDTH_NUM = 2;		//ステータスを横に並べられる数



EditScene::EditScene(void)
{
	int backImg_=0;
	int nextPB_=0;
	int backPB_=0;
	int unitBackImg_=0;
	int hpIcon_=0;
	int attackIcon_=0;
	int speedIcon_=0;
	int pickUpUnit_=0;
	int sFontHandle_=0;
	int sFontColor_=0;
	int page_=0;
}

EditScene::~EditScene(void)
{
}

void EditScene::Init(void)
{
	//画像の登録
	//背景
	bgImg_ = LoadGraph("Data/Image/bg/EditScene_bg.png");
	//ユニットカード裏
	unitBackImg_ = LoadGraph("Data/Image/UI/UnitBack.png");
	//ボタン画像
	backImg_ = LoadGraph("Data/Image/UI/BackBtn.png");

	//アイコン画像
	hpIcon_ = LoadGraph("Data/Image/Icon/HP.png");
	attackIcon_ = LoadGraph("Data/Image/Icon/Attack.png");
	speedIcon_ = LoadGraph("Data/Image/Icon/Speed.png");

	//バックボタンの生成
	int sy = Application::SCREEN_SIZE_Y;
	backBtn_ = new CircleButton();
	backBtn_->Create({ 0,sy }, backImg_, 0);


	//前ページボタン生成
	backPB_ = LoadGraph("Data/Image/UI/Edit/backPageBtn.png");
	pageBtn_[0] = new RectButton();
	pageBtn_[0]->Create(BACK_PAUGE_BTN_POS, PAUGE_BTN_SIZE, backPB_);
	//次ページボタン生成
	nextPB_ = LoadGraph("Data/Image/UI/Edit/nextPageBtn.png");
	pageBtn_[1] = new RectButton();
	pageBtn_[1]->Create(NEXT_PAUGE_BTN_POS, PAUGE_BTN_SIZE, nextPB_);


	//デッキユニット、所持ユニットの初期化
	InitMyUnit();
	//ステータスコマンドボタン生成
	CreateCmd();

	//ステータス画面フォント
	sFontHandle_= CreateFontToHandle("游明朝", STUTAS_FONT_SIZE, STUTAS_FONT_THICK,
		DX_FONTTYPE_ANTIALIASING_4X4);
	sFontColor_ = STUTAS_FONT_COLOR;

	//BGM再生
	auto& snd = SoundManager::GetInstance();
	bgmHandle_ = snd.LoadSound(SoundManager::SOUND_TYPE::EDIT_BGM);
	snd.PlaySoundBGM(bgmHandle_);

	
	//ピックアップユニット
	pickUpUnit_ = -1;

	//ページ
	page_ = 1;
}

void EditScene::Update(void)
{
	//バック処理
	if (backBtn_->ButtonDecision())
	{
		//デッキの確定
		DeckDecision();
		//シーン遷移
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::SELECT, true);
		return;
	}


	//ボタン更新
	//バックボタン
	backBtn_->Update();

	//ページボタン
	PageBtnProcess();

	//デッキ
	for (auto& deck : myDeckBtn_)
	{
		deck.second->Update();
	}
	//ユニットカードプール
	for (auto& card : unitCards_)
	{
		card.second->Update();
	}

	//技一覧
	for (auto& cmdBtn : cmdBtns_)
	{
		cmdBtn->Update();
	}


	//デッキ編集処理
	DeckEditProcess();
	//押したユニットカードをピックアップにセット
	SelectPickUpUnit();
}

void EditScene::Draw(void)
{
	//背景
	DrawGraph(0, 0, bgImg_, true);


	//ボタンUI
	//マイデッキ
	for (auto& deck : myDeckBtn_)
	{
		deck.second->Draw();
	}
	//ユニットプール
	for (auto& card : unitCards_)
	{
		card.second->Draw();
	}

	//バックボタン
	backBtn_->Draw();

	//ページボタン
	for (auto& pb : pageBtn_)
	{
		pb->Draw();
	}

	//ユニット情報
	DrawUnitStatus();

#ifdef DEBUG
	auto cx = Application::SCREEN_SIZE_X;
	auto cy = Application::SCREEN_SIZE_Y;
	auto span = 20;
	for (size_t i = 0; i < 100; i++)
	{
		//X
		DrawLine(0, i * span, cx, i * span, 0xff0000);

		//Y
		DrawLine(i * span, 0, i * span, cy, 0x0000ff);
	}
#endif // _DEBUG

}

void EditScene::Release(void)
{
	//画像の解放
	DeleteGraph(bgImg_);
	DeleteGraph(unitBackImg_);
	DeleteGraph(backImg_);

	DeleteGraph(hpIcon_);
	DeleteGraph(attackIcon_);
	DeleteGraph(speedIcon_);

	//ボタンクラス開放
	backBtn_->Release();
	delete backBtn_;

	//サウンドの終了
	StopSoundMem(bgmHandle_);


	for (auto& du : myDeckBtn_)
	{
		du.second->Release();
		delete du.second;
	}
	myDeckBtn_.clear();

	for (auto& cu : unitCards_)
	{
		cu.second->Release();
		delete cu.second;
	}
	unitCards_.clear();

	for (auto& cb : cmdBtns_)
	{
		cb->Release();
		delete cb;
	}
	cmdBtns_.clear();
}

void EditScene::InitMyUnit(void)
{
	//ユニット情報
	auto& deckMng = DeckManager::GetInstance();
	auto& dataMng = UnitDataManager::GetInstance();

	//デッキユニットボタンの生成
	int noDeck = 0;
	for (auto& num : deckMng.GetDeck().dArray_)
	{
		//ユニットデータ
		auto u = dataMng.GetUnitData(num);
		//表示座標
		Vector2 cardPos = FIRST_UNIT_DECK_POS;
		cardPos.x += OFFSET_UNIT_DECK_POS * noDeck;

		//ボタン生成
		UnitButton* ub = new UnitButton();
		ub->Create(cardPos, unitBackImg_, u);
		myDeckBtn_.emplace(std::make_pair(num, ub));

		noDeck++;
	}

	//所持ユニット番号取得
	unitCardNums_ = deckMng.GetUnitPool();
	//所持ユニットボタンの生成
	int no = 0;
	for (auto& num : unitCardNums_)
	{
		//生成数の上限確認
		if (no >= DECK_POOL_MAX)break;


		//ユニットデータ
		auto u = dataMng.GetUnitData(num);
		//表示座標
		Vector2 cardPos = FIRST_UNIT_POOL_POS;
		if (no < DECK_POOL_WIGTH_NUM)
		{
			cardPos.x += OFFSET_UNIT_POOL_POS.x * no;
		}
		else
		{
			cardPos.x += OFFSET_UNIT_POOL_POS.x * (no - DECK_POOL_WIGTH_NUM);
			cardPos.y += OFFSET_UNIT_POOL_POS.y;
		}
		//ボタン生成
		UnitButton* ub = new UnitButton();
		ub->Create(cardPos, unitBackImg_, u);
		unitCards_.emplace(std::make_pair(num, ub));

		no++;
	}

}

void EditScene::CreateCmd(void)
{
	//コマンドボタンの生成
	int noX, noY;
	noX = noY = 0;
	Vector2 size = CMD_SHOE_SIZE;
	Vector2 posBase = CMD_SHOE_BASE_POS;
	for (int i = 0; i < CMD_SHOE_NUM;i++)
	{
		CmdButton* b = new CmdButton();

		//座標決め
		Vector2 pos = { 
			posBase.x + size.x * noX, 
			posBase.y + size.y * noY };
		b->Create(pos, size, 0);
		cmdBtns_.push_back(b);

		//座標ずらし。コマンド数の半分ずつ表示する
		noY++;
		if (noY >= CMD_SHOE_NUM/2)
		{
			noY = 0;
			noX++;
		}
	}
}

void EditScene::DrawUnitStatus(void)
{
	if (pickUpUnit_ == -1)return;


	//文字列の（半分の）長さを取得
	auto halfLen = Utility::CalcStringLength(pickUpUnitData_.name, sFontHandle_) / 2;
	//名前
	DrawStringToHandle(
		UNIT_NAME_SHOE_POS.x - halfLen, UNIT_NAME_SHOE_POS.y,
		pickUpUnitData_.name.c_str(), sFontColor_, sFontHandle_);

	//ユニット画像
	DrawGraph(UNIT_IMAGE_SHOE_POS.x, UNIT_IMAGE_SHOE_POS.y, pickUpUnitData_.imgHandle, true);

	//ステータス
	int noX = 0;
	int noY = 0;
	int offsetX = STATUS_SHOE_OFFSET.x;
	int offsetY = STATUS_SHOE_OFFSET.y;

	auto drawIcon = [&](int icon,int value) {

		Vector2 iconPos = { ICON_SHOW_POS.x + (offsetX * noX), ICON_SHOW_POS.y + (offsetY * noY) };

		DrawGraph(iconPos.x, iconPos.y, icon, true);
		DrawFormatStringToHandle(
			iconPos.x + STATUS_TEXT_SHOW_OFFSET.x,
			iconPos.y + STATUS_TEXT_SHOW_OFFSET.y,
			sFontColor_, sFontHandle_, "%d", value);

		//横2列に並べる
		noX++;
		if (noX >= STATUS_WIDTH_NUM)
		{
			noX = 0;
			noY++;
		}

		return;
	};

	drawIcon(hpIcon_, pickUpUnitData_.hp);
	drawIcon(attackIcon_, pickUpUnitData_.attack);
	drawIcon(speedIcon_, pickUpUnitData_.speed);

	//技一覧
	for (auto& cmdBtn : cmdBtns_)
	{
		cmdBtn->Draw();
	}
	//技詳細一覧
	for (auto& cmdBtn : cmdBtns_)
	{
		cmdBtn->DrawCmdDetail();
	}
}

void EditScene::DeckEditProcess(void)
{
	if (pickUpUnit_ == -1)return;

	//ピックアップユニットをセットする
	for (auto& deck : myDeckBtn_)
	{
		if (!deck.second->ButtonDecision())continue;

		//デッキに同じユニットが存在したら、スキップ
		auto u = myDeckBtn_.find(pickUpUnit_);
		if (u != myDeckBtn_.end())return;


		//古いキーの値を取得
		UnitButton* c = myDeckBtn_[deck.first];
		//削除
		myDeckBtn_.erase(deck.first);

		//ボタンのデータ更新
		c->ChengeUnitData(pickUpUnitData_);

		//新しいキーで要素を追加
		myDeckBtn_[pickUpUnit_] = c;

		return;
	}
}

void EditScene::SelectPickUpUnit(void)
{
	for (auto& card : unitCards_)
	{
		//押されていない場合、処理をしない
		if (!card.second->ButtonDecision())continue;
		
		//番号をセット
		pickUpUnit_ = card.first;

		//ユニットデータの取得
		auto& dataMng = UnitDataManager::GetInstance();
		pickUpUnitData_ = dataMng.GetUnitData(pickUpUnit_);
		//コマンドのセット
		int n = 0;
		for (auto& cmd : pickUpUnitData_.cmdNum)
		{
			cmdBtns_[n]->SetCmd(cmd);
			n++;
		}
	}
}

void EditScene::DeckDecision(void)
{
	auto& dMng = DeckManager::GetInstance();
	std::vector<int>  num;
	for (auto& deck : myDeckBtn_)
	{
		num.push_back(deck.first);
	}
	dMng.SetDeck({ num[0],num[1],num[2] });
}

void EditScene::PageBtnProcess(void)
{
	//更新
	//ページボタン
	for (auto& pb : pageBtn_)
	{
		pb->Update();
	}

	//所持ユニット数
	int uNum = unitCardNums_.size();
	//最大ページ数
	int maxPage = ceil(static_cast<float>(uNum) / static_cast<float>(DECK_POOL_MAX));

	//前ボタンクリック処理
	if (pageBtn_[0]->ButtonDecision())
	{
		page_--;
		page_=Utility::Wrap(page_, 1, maxPage + 1);
		ChangeDeckPoolPage();
		return;
	}

	//次ボタンクリック処理
	if (pageBtn_[1]->ButtonDecision())
	{
		page_++;
		page_=Utility::Wrap(page_, 1, maxPage + 1);
		ChangeDeckPoolPage();
		return;
	}

}

void EditScene::ChangeDeckPoolPage(void)
{
	auto& uDataMng = UnitDataManager::GetInstance();

	//入れ物
	std::unordered_map<int, UnitButton*> unitCards;

	int number = 0;
	int count = 0;
	for (auto& uCardNum : unitCardNums_)
	{
		//
		if (number >= unitCardNums_.size())break;

		//以上の場合、処理終了
		if (number >= page_ * DECK_POOL_MAX)break;

		//未満
		int n = number - ((page_ - 1) * DECK_POOL_MAX);
		if (0 > n)
		{ 
			number++;
			continue;
		}

		//ユニットデータ
		auto uData = uDataMng.GetUnitData(uCardNum);
		//表示座標
		Vector2 cardPos = FIRST_UNIT_POOL_POS;
		//横に並べるカードの数
		if (count < DECK_POOL_WIGTH_NUM)
		{
			cardPos.x += OFFSET_UNIT_POOL_POS.x * count;
		}
		else
		{
			cardPos.x += OFFSET_UNIT_POOL_POS.x * (count - DECK_POOL_WIGTH_NUM);
			cardPos.y += OFFSET_UNIT_POOL_POS.y;
		}
		//新しいページのボタン生成
		UnitButton* ub = new UnitButton();
		ub->Create(cardPos, unitBackImg_, uData);
		unitCards.emplace(std::make_pair(uCardNum, ub));

		number++;
		count++;
	}
	
	//前ページのボタンの解放処理
	for (auto& btn : unitCards_) {
		btn.second->Release();
	}
	//配列の削除
	unitCards_.clear();

	//配列の代入
	unitCards_ = unitCards;
}
