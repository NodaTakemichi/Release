#include <DxLib.h>
#include "../Manager/DataManager/UnitDataManager.h"
#include "../Manager/DataManager/BattleDataManager.h"
#include "../Manager/DataManager/SoundManager.h"
#include "../Manager/DeckManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Utility/Utility.h"
#include "../Utility/DrawShader.h"
#include "../Utility/Button/UnitButton.h"
#include "../Common/Vector2.h"
#include "../Application.h"
#include "../_debug/_DebugDispOut.h"
#include "ResultScene.h"

//コメント用フォント
constexpr int COMMNET_FONT_SIZE = 20;
constexpr int COMMNET_FONT_THICK = 20;
constexpr int COMMNET_FONT_COLOR = 0xdddddd;
const Vector2 COMMNET_POS = { 300,120 };
//コメントボックス
const Vector2 COMMNET_BOX_POS = Vector2{ 280,100 };
const Vector2 COMMNET_BOX_SIZE = Vector2{ 360,60 };
//使い魔
const Vector2 DEVIL_SIZE = Vector2{ 360,360 };
//指示テキストオフセット
constexpr int OPERATOR_TEXT_OFFSET_Y = 30;
constexpr float OPERATOR_TEXT_SHOW_TIME = 3.0f;	//表示時間

//ユニット画像
const Vector2 UNIT_SHOW_BASE_POS = Vector2{ 630,330 };	//表示座標：ベース
const int UNIT_SHOW_OFFSET_X= 200;	//オフセットX


ResultScene::ResultScene(void)
{
	fontHandle_=0;
	devilImg_ = 0;
	devilPos_ = Vector2{};
	cmtBoxImg_ = 0;
	unitBackImg_ = 0;

}

ResultScene::~ResultScene(void)
{
}

void ResultScene::Init(void)
{
	//背景
	bgImg_ = LoadGraph("Data/Image/bg/result.png");

	//フォント
	fontHandle_ = CreateFontToHandle("游明朝", COMMNET_FONT_SIZE, COMMNET_FONT_THICK,
		DX_FONTTYPE_ANTIALIASING_4X4);
	//コメントボックス画像
	cmtBoxImg_ = LoadGraph("Data/Image/UI/CommentBox.png");
	//つかいま
	devilImg_ = LoadGraph("Data/Image/UI/つかいま_口開き.png");
	devilPos_ = { -20,50 };

	//ユニットカード裏
	unitBackImg_ = LoadGraph("Data/Image/UI/UnitBack.png");
	//加入ユニットのボタン生成
	CreateUnitBtn();


	//サウンドの登録
	auto& snd = SoundManager::GetInstance();
	bgmHandle_ = snd.LoadSound(SoundManager::SOUND_TYPE::RESULT_BGN);
	//BGMの再生
	snd.PlaySoundBGM(bgmHandle_);

	//敵ユニットをデッキにセット
	auto& bd = BattleDataManager::GetInstance().GetBattleData();
	int num = bd.eDeck.dArray_[0];
	DeckManager::GetInstance().AddUnit(num);
	num = bd.eDeck.dArray_[1];
	DeckManager::GetInstance().AddUnit(num);
	num = bd.eDeck.dArray_[2];
	DeckManager::GetInstance().AddUnit(num);

}

void ResultScene::Update(void)
{
	//デルタタイム
	auto delta = SceneManager::GetInstance().GetDeltaTime();
	totalTime_ += delta;

	//シーン遷移
	auto& ins = InputManager::GetInstance();
	if (ins.IsTrgMouseLeft())
	{
		SceneManager::GetInstance().ChangeScene(
			SceneManager::SCENE_ID::SELECT,true);
		return;
	}

	//ボタン更新
	for (auto& btn : unitBtns_)
	{
		btn->Update();
	}
}

void ResultScene::Draw(void)
{
	int sx = Application::SCREEN_SIZE_X;
	int sy = Application::SCREEN_SIZE_Y;
	auto& ds = DrawShader::GetInstance();

	//背景描画
	ds.DrawExtendGraphToShader(
		{ 0,0 }, { sx,sy }, bgImg_);

	//コメントボックス
	ds.DrawExtendGraphToShader(
		COMMNET_BOX_POS, COMMNET_BOX_SIZE, cmtBoxImg_);
	//コメント
	DrawFormatStringFToHandle(
		COMMNET_POS.x, COMMNET_POS.y, COMMNET_FONT_COLOR, fontHandle_, "新しい仲間が加わったぜ");
	//使い魔
	ds.DrawExtendGraphToShader(
		devilPos_, DEVIL_SIZE, devilImg_);

	//加入ユニットボタンの描画
	DrawGetUnits();


	//クリック指示
	std::string inst = "クリックして、セレクト画面へ戻る";
	auto len = Utility::CalcStringLength(inst, fontHandle_);
	if (fmodf(totalTime_, OPERATOR_TEXT_SHOW_TIME) >= 1.0f)
	{
		DrawFormatStringFToHandle(
			sx - len, sy - OPERATOR_TEXT_OFFSET_Y, COMMNET_FONT_COLOR, fontHandle_, inst.c_str());
	}

#ifdef DEBUG
	DrawPosition();
#endif // _DEBUG

}

void ResultScene::Release(void)
{
	DeleteGraph(bgImg_);

	//BGMの停止
	StopSoundMem(bgmHandle_);
}

void ResultScene::CreateUnitBtn(void)
{
	auto& bDataMng = BattleDataManager::GetInstance();
	auto& uDataMng = UnitDataManager::GetInstance();

	Vector2 basePos = UNIT_SHOW_BASE_POS;


	//ユニットボタンの生成
	int count = 0;
	auto& enemyUnits = bDataMng.GetBattleData().eDeck.dArray_;
	for (auto& num : enemyUnits)
	{
		if (num <= 0)continue;

		//ユニットデータ
		auto uData = uDataMng.GetUnitData(num);
		//表示座標をずらす
		Vector2 drawPos = basePos;
		if (count == 1)
		{
			//左にずらす
			drawPos += {-UNIT_SHOW_OFFSET_X, 0};
		}
		if (count == 2)
		{
			//右にずらす
			drawPos += {UNIT_SHOW_OFFSET_X, 0};
		}

		//ボタン生成
		UnitButton* ub = new UnitButton();
		ub->Create(drawPos, unitBackImg_, uData);
		//挿入
		unitBtns_.emplace_back(ub);

		count++;
	}
}

void ResultScene::DrawGetUnits(void)
{
	for (auto& btn : unitBtns_)
	{
		btn->Draw();
	}
}
