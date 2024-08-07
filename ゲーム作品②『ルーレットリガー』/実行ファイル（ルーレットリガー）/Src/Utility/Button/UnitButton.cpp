#include <DxLib.h>
#include "../../Manager/DataManager/SoundManager.h"
#include "../../Manager/DataManager/UnitDataManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/InputManager.h"
#include "../DrawShader.h"
#include "../Utility.h"
#include "UnitButton.h"

using PsType = DrawShader::PS_TYPE;

//フレームカラー：通常
constexpr COLOR_F FRAME_COLOR_NORMAL = { 0.6f, 0.6f, 0.6f, 1.0f };
//フレームカラー：マウスが上に位置している
constexpr COLOR_F FRAME_COLOR_ON_MOUSE = { 1.0f, 0.9f, 0.0f, 1.0f };
//フレームカラー：押下状態
constexpr COLOR_F FRAME_COLOR_PUSH_BUTTON = { 1.0f, 0.3f, 0.2f, 1.0f };

//フォントサイズ
constexpr int FONT_SIZE = 10;
//フォントの太さ
constexpr int FONT_THICK = 20;
//フォントカラー
constexpr int FONT_COLOR = 0x111111;

//カード枠オフセット値
constexpr int CARD_FRAME_OFFSET = 10;
//ユニット画像オフセット値Y
constexpr int UNIT_IMAGE_OFFSET_Y = 20;
//ユニット名前オフセット値Y
constexpr int UNIT_NAME_OFFSET_Y = 150;


UnitButton::UnitButton()
{
	size_ = Vector2{};
	unitImg_=0;
	name_="";
	fontHandle_=0;
	fColor_=COLOR_F();
}

UnitButton::~UnitButton()
{
}

void UnitButton::Init(void)
{

	//フォントの登録
	fontHandle_ = CreateFontToHandle("游明朝", FONT_SIZE, FONT_THICK,
		DX_FONTTYPE_ANTIALIASING_4X4);

	//フレームカラー
	fColor_ = FRAME_COLOR_NORMAL;

	//SE
	auto& snd = SoundManager::GetInstance();
	clickSE_ = snd.LoadSound(SoundManager::SOUND_TYPE::BTN_CLICK);
	onBtnSE_ = snd.LoadSound(SoundManager::SOUND_TYPE::BTN_ON);

	//ボタン上にマウスが乗っているかどうか
	onBtnBefor_ = false;
	onBtn_ = false;

}

void UnitButton::Update(void)
{
	//SE再生
	PlayOnBtnSE();
	PlayDecSE();

	//ボタンを離した
	if (ButtonDecision())return;

	//ボタン押下中
	if (PushButton())
	{
		fColor_ = FRAME_COLOR_PUSH_BUTTON;
		return;
	}

	//マウスがボタン上にある
	if (MouseOnButton())
	{
		fColor_ = FRAME_COLOR_ON_MOUSE;
		return;
	}

	fColor_ = FRAME_COLOR_NORMAL;
	return;

}

void UnitButton::Draw(void)
{
	auto& ds = DrawShader::GetInstance();
	auto totalTime = SceneManager::GetInstance().GetTotalTime();

	//カード枠
	int offset = CARD_FRAME_OFFSET;
	Vector2 framePos = { pos_.x - offset / 2,pos_.y - offset / 2 };
	Vector2 frameSize = { size_.x + offset,size_.y + offset };
	COLOR_F buf = COLOR_F{ totalTime };
	ds.DrawExtendGraphToShader(framePos, frameSize, -1, PsType::CARD_FRAME, fColor_, buf);

	//背面画像
	DrawGraph(pos_.x, pos_.y, backImg_, true);

	//ユニット画像
	ds.DrawExtendGraphToShader({ pos_.x,pos_.y + UNIT_IMAGE_OFFSET_Y }, { size_.x,size_.x }, unitImg_);


	//中心位置
	int center = pos_.x + size_.x / 2;
	//文字列の（半分の）長さを取得
	center -= Utility::CalcStringLength(name_, fontHandle_) / 2;
	DrawStringToHandle(center, pos_.y + UNIT_NAME_OFFSET_Y, name_.c_str(), FONT_COLOR, fontHandle_);
}

void UnitButton::Release(void)
{
	DeleteFontToHandle(fontHandle_);
}

void UnitButton::Create(Vector2 pos, int back, UnitData unit)
{
	//初期化
	Init();

	pos_ = pos;
	backImg_ = back;
	GetGraphSize(backImg_, &size_.x, &size_.y);

	//画像
	unitImg_ = unit.imgHandle;
	//名前
	name_ = unit.name;
}

bool UnitButton::MouseOnButton(void)
{
	//マウス位置
	auto& ins = InputManager::GetInstance();
	Vector2 mPos = ins.GetMousePos();

	onBtn_ = false;
	if (IsMouseInRect(mPos, pos_, size_))onBtn_ = true;

	return onBtn_;
}

void UnitButton::ChengeUnitData(const UnitData& data)
{
	//画像
	unitImg_ = data.imgHandle;
	//名前
	name_ = data.name;
}

bool UnitButton::IsMouseInRect(const Vector2& inside, const Vector2& outside, const Vector2& size)
{
	if (
		inside.x > outside.x &&
		inside.x < outside.x + size.x &&
		inside.y > outside.y &&
		inside.y < outside.y + size.y
		)
	{
		return true;
	}

	return false;
}
