#include "EnemyUI.h"
//HPゲージカラー(赤)
constexpr COLOR_F HP_GAUGE_COLOR = { 0.8f,0.4f,0.4f,1.0f };
//HPゲージローカル座標
const Vector2 HP_GAUGE_LOCAL_POS = Vector2{ 50,20 };
//ターゲット画像のローカル座標
const Vector2 TARGET_IMG_LOCAL_POS = Vector2{ -100,UnitBase::DRAWING_SIZE / 4 };


EnemyUI::EnemyUI(
	Vector2 pos, std::string& name, int& hp, int& nowHp, int& maxHp, bool& active) :
	UnitUI(pos, name, hp, nowHp, maxHp,active)
{
}

EnemyUI::~EnemyUI()
{
}

void EnemyUI::Init(void)
{
	//初期化
	UnitUI::Init();
}

void EnemyUI::Draw(void)
{
	//名前描画
	DrawName(name_, unitPos_, HP_GAUGE_COLOR);

	//HPが0以下の時、HPゲージを描画しない
	if (nowHp_ <= 0)return;

	//バフアイコンの描画
	DrawBuffIcon();

	//ユニットのサイズ
	auto unitSize = UnitBase::DRAWING_SIZE;
	//HP枠の表示
	Vector2 pos = unitPos_ + HP_GAUGE_LOCAL_POS;
	pos.x += unitSize;
	DrawHpFrame(pos);

	//HPシェーダー
	DrawHpShader(pos, HP_GAUGE_COLOR);

	//描画
	UnitUI::Draw();
}

void EnemyUI::DrawRockOn(void)
{

	Vector2 pos = unitPos_ + TARGET_IMG_LOCAL_POS;
	DrawGraph(pos.x, pos.y, targetImg_, true);
}
