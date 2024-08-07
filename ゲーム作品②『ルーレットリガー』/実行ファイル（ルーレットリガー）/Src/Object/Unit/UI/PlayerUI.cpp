#include "PlayerUI.h"

//HPゲージカラー(緑)
constexpr COLOR_F HP_GAUGE_COLOR= { 0.4f,0.8f,0.4f,1.0f };
//HPゲージローカル座標
const Vector2 HP_GAUGE_LOCAL_POS = Vector2{ -70,20 };
//ターゲット画像のローカル座標
const Vector2 TARGET_IMG_LOCAL_POS = Vector2{ UnitBase::DRAWING_SIZE ,UnitBase::DRAWING_SIZE/4 };

PlayerUI::PlayerUI(
	Vector2 pos, std::string& name, int& hp, int& nowHp, int& maxHp, bool& active) :
	UnitUI(pos, name, hp, nowHp, maxHp,active)
{
}

PlayerUI::~PlayerUI()
{
}

void PlayerUI::Init(void)
{
	//初期化
	UnitUI::Init();
}

void PlayerUI::Draw(void)
{
	//ユニットのサイズ
	auto unitSize = UnitBase::DRAWING_SIZE;

	//名前描画
	DrawName(name_, unitPos_, HP_GAUGE_COLOR);

	//HPが0以下の時、HPゲージを描画しない
	if (nowHp_ <= 0)return;

	//バフアイコンの描画
	DrawBuffIcon();


	//HP枠の表示
	Vector2 pos = unitPos_ + HP_GAUGE_LOCAL_POS;
	DrawHpFrame(pos);

	//HPシェーダー
	DrawHpShader(pos, HP_GAUGE_COLOR);

	//描画
	UnitUI::Draw();

}

void PlayerUI::DrawRockOn(void)
{
	Vector2 pos = unitPos_+ TARGET_IMG_LOCAL_POS; 
	DrawGraph(pos.x, pos.y, targetImg_, true);
}
