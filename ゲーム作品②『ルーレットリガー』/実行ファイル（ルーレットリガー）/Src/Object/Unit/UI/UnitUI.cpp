#include<DxLib.h>
#include<string>
#include"../../../_debug/_DebugConOut.h"
#include"../../../Manager/SceneManager.h"
#include"../../../Utility/Utility.h"
#include"../../../Utility/DrawShader.h"
#include "UnitUI.h"

//HPゲージ
constexpr int  HP_GAUGE_X = 20;
constexpr int  HP_GAUGE_Y = 120;
//ダメージ用フォント
constexpr int DAMAGE_FONT_SIZE = 40;
constexpr int DAMAGE_FONT_THICK = 20;
constexpr int DAMAGE_FONT_COLOR = 0xff0000;
//回復用フォント
constexpr int HEAL_FONT_SIZE = 40;
constexpr int HEAL_FONT_THICK = 20;
constexpr int HEAL_FONT_COLOR = 0xffffff;
//ユニット関連用フォント
constexpr int UNIT_FONT_SIZE = 15;
constexpr int UNIT_FONT_THICK = 20;
constexpr int UNIT_FONT_COLOR = 0xe6e6e6;
//HP変化時演出
constexpr int HP_CHANGE_BACK_OFFSET_X = 50;				//バック画像の表示オフセットX
constexpr int HP_CHANGE_NUM_SHOW_OFFSET_Y = 20;			//変化HPの表示オフセットY
const Vector2 DAMAGE_VALUE_LOCAL_POS = Vector2{ 0,-5 };	//ダメージ量のローカル座標
const Vector2 HEAL_VALUE_LOCAL_POS = Vector2{ -18,18 };	//ヒール量のローカル座標
//HPバー
constexpr int HP_FRAME_SIZE = 1;			//HPバーフレームの太さ
constexpr int HP_NUM_SHOW_OFFSET_Y = 5;		//HP数値表示オフセット
//ユニット名
const Vector2 UNIT_NAME_FRAME_LOCAL_POS = Vector2{ -25,150 };	//ユニット名フレームのローカル座標
const Vector2 UNIT_NAME_LOCAL_POS = Vector2{ 0,157 };			//ユニット名のローカル座標
//バフアイコン
constexpr int BUFF_ICON_SIZE = 32;					//バフアイコンの画像サイズ
constexpr int BUFF_ICON_SHOW_SPAN_X = 36;			//バフアイコンの表示間隔Ｘ
const Vector2 BUFF_ICON_SHOW_LOCAL_POS = Vector2{ 10, 120 };	//バフアイコンのローカル座標

//シェーダータイプ
using PsType = DrawShader::PS_TYPE;

UnitUI::UnitUI(
	Vector2 pos, std::string& name, int& hp, int& nowHp, int& maxHp, bool& active) :
	unitPos_(pos), name_(name), hp_(hp), nowHp_(nowHp), maxHp_(maxHp), active_(active)
{
	nameFrameImg_= 0;
	targetImg_= 0;
	dmgFrameImg_= 0;
	healFrameImg_= 0;
	dmg_= 0;
	dmgFontHandle_= 0;
	healFontHandle_= 0;
	unitFontHandle_= 0;
	totalTime_=0.0f;
	dmgNumDrawing_=false;	
	healNumDrawing_ = false;
}

UnitUI::~UnitUI()
{
}

void UnitUI::Init(void)
{
	//名前枠,ターゲット画像の登録
	nameFrameImg_ = LoadGraph("./Data/Image/UI/NameFrame.png");
	targetImg_ = LoadGraph("./Data/Image/UI/target.png");

	//バフアイコンの画像登録
	LoadDivGraph(
		"Data/Image/Icon/BuffIcons.png", static_cast<int>(Buff::BUFF_TYPE::MAX),
		1, static_cast<int>(Buff::BUFF_TYPE::MAX),
		BUFF_ICON_SIZE, BUFF_ICON_SIZE, buffIconHandles_);

	//ダメージフレーム画像
	dmgFrameImg_ = LoadGraph("./Data/Image/UI/DmgNum2.png");
	//ダメージ描画判定
	dmgNumDrawing_ = false;	

	//回復値フレーム画像
	healFrameImg_ = LoadGraph("./Data/Image/UI/HealNum.png");
	//回復値描画判定
	healNumDrawing_ = false;	

	//フォントの読み込み
	LPCSTR font_path = "./Data/Font/ZeroGothic.otf";
	if(AddFontResourceEx(font_path, FR_PRIVATE, NULL) <= 0)
	{
		//失敗
		printf("フォントデータの登録失敗\n");
	}
	//ダメージフォント
	dmgFontHandle_ = CreateFontToHandle("零ゴシック", DAMAGE_FONT_SIZE, DAMAGE_FONT_THICK, -1,
		DX_FONTTYPE_ANTIALIASING_4X4);
	//回復フォント
	healFontHandle_ = CreateFontToHandle("游明朝", HEAL_FONT_SIZE, HEAL_FONT_THICK, -1,
		DX_FONTTYPE_ANTIALIASING_EDGE_4X4, -1, 1);
	//ユニットフォント
	unitFontHandle_ = CreateFontToHandle("游明朝", UNIT_FONT_SIZE, UNIT_FONT_THICK, -1,
		DX_FONTTYPE_ANTIALIASING_4X4);


	totalTime_ = 0.0f;

}

void UnitUI::Draw(void)
{
	//HP変動数値表示
	auto DrawChangeHP = [&](
		int img,int value,int fontHandle,int color, Vector2 offsetPos) {
		//ダメージ表記画像
		int imgPosX = unitPos_.x - HP_CHANGE_BACK_OFFSET_X;
		int imgPosY = unitPos_.y;
		DrawGraph(imgPosX, imgPosY, img, true);

		//ダメージ数値
		//文字列の幅、高さ
		std::string dmgString = std::to_string(value);
		int halfWidth = Utility::CalcStringLength(dmgString, fontHandle) / 2;
		//画像サイズ
		int imgSizeX, imgSizeY;
		GetGraphSize(dmgFrameImg_, &imgSizeX, &imgSizeY);
		//数字表示座標
		Vector2 stringPos = {
			imgPosX + (imgSizeX / 2) - halfWidth + offsetPos.x ,
			imgPosY + (imgSizeY / 2) - HP_CHANGE_NUM_SHOW_OFFSET_Y + offsetPos.y ,
		};
		//数字描画
		DrawStringToHandle(
			stringPos.x, stringPos.y, dmgString.c_str(),
			color, fontHandle);
	};

	//ダメージ数値の表示
	if (dmgNumDrawing_)
	{
		DrawChangeHP(dmgFrameImg_, dmg_, dmgFontHandle_, 
			DAMAGE_FONT_COLOR, DAMAGE_VALUE_LOCAL_POS);
	}

	//回復量表示
	if (healNumDrawing_)
	{
		DrawChangeHP(healFrameImg_, dmg_, healFontHandle_, 
			HEAL_FONT_COLOR, HEAL_VALUE_LOCAL_POS);
	}
}

void UnitUI::Release(void)
{
	DeleteGraph(nameFrameImg_);
	DeleteGraph(targetImg_);
	DeleteGraph(dmgFrameImg_);
}

void UnitUI::SetBuff(std::vector<Buff*> buffs)
{
	buffs_ = buffs;
}

void UnitUI::SetDmgValue(const bool& drawing, const int& dmg)
{
	dmgNumDrawing_ = drawing;
	dmg_ = dmg;
}

void UnitUI::SetHealValue(const bool& drawing, const int& heal)
{
	healNumDrawing_ = drawing;
	dmg_ = heal;
}

void UnitUI::DrawHpShader(const Vector2& pos , const COLOR_F& color)
{
	//HPの割合
	float nowRatio = static_cast<float>(nowHp_) / static_cast<float>(maxHp_);
	float hpRatio = static_cast<float>(hp_) / static_cast<float>(maxHp_);
	COLOR_F subBuf = COLOR_F{
		nowRatio,
		hpRatio
	};

	//描画
	DrawShader::GetInstance().DrawExtendGraphToShader(
		pos, { HP_GAUGE_X ,HP_GAUGE_Y }, -1, PsType::HP_SHADER, color, subBuf
	);

}

void UnitUI::DrawHpFrame(const Vector2& pos)
{
	//HPフレームの描画
	auto frameSize = HP_FRAME_SIZE;
	DrawBox(pos.x - frameSize, pos.y - frameSize,
		pos.x + HP_GAUGE_X + frameSize, pos.y + HP_GAUGE_Y + frameSize,
		0xffffff, true);

	//HPの数値表示
	DrawFormatStringToHandle(
		pos.x, pos.y + HP_GAUGE_Y + frameSize + HP_NUM_SHOW_OFFSET_Y,
		0xffffff, unitFontHandle_, "HP\n%d", nowHp_);

}

void UnitUI::DrawName(const std::string& name, const Vector2& uPos, const COLOR_F& color)
{
	totalTime_ = SceneManager::GetInstance().GetTotalTime();
	auto unitSize = static_cast<int>(UnitBase::DRAWING_SIZE);	

	//枠座標
	Vector2 framePos = uPos ;			
	framePos += UNIT_NAME_FRAME_LOCAL_POS;		
	//名前枠の表示
	//アクティブ状態かどうかで色を変更
	COLOR_F c = active_ ? color : COLOR_F{ 1.0f,1.0f,1.0f,1.0f };
	COLOR_F buf = COLOR_F{
		c.r,
		c.g,
		c.b,
		totalTime_
	};
	auto& ds = DrawShader::GetInstance();
	ds.DrawGraphToShader(framePos, nameFrameImg_, PsType::IUMI_FRAME, buf);

	//名前座標
	Vector2 namePos = { };	
	namePos = Vector2{ uPos.x + unitSize / 2, uPos.y } + UNIT_NAME_LOCAL_POS;
	//文字列の（半分の）長さを取得
	auto halfLen = Utility::CalcStringLength(name, unitFontHandle_) / 2;
	//名前描画
	DrawStringToHandle(
		namePos.x - halfLen, namePos.y,
		name.c_str(), 0xffffff, unitFontHandle_);

}

void UnitUI::DrawBuffIcon()
{
	int count = 0;
	for (auto& buff : buffs_)
	{
		if (!buff->IsAlive())continue;

		//バフの検索
		Buff::BUFF_TYPE iconType= buff->GetBuff();

		const int spanX = BUFF_ICON_SHOW_SPAN_X;
		const Vector2 localPos = BUFF_ICON_SHOW_LOCAL_POS;
		//バフアイコン座標
		Vector2 pos = {};
		pos = Vector2{ unitPos_.x + (count * spanX), unitPos_.y } + localPos;

		DrawGraph(pos.x, pos.y, buffIconHandles_[static_cast<int>(iconType)], true);

		count++;
	}
}


