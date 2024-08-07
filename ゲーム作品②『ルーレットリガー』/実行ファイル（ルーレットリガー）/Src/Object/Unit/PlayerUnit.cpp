#include "../../_debug/_DebugConOut.h"
#include "../../_debug/_DebugDispOut.h"

#include "./UI/PlayerUI.h"
#include "PlayerUnit.h"


PlayerUnit::PlayerUnit(const int& dataNum, const int& unitNum) :
	UnitBase(dataNum, unitNum)
{
	type_ = UNIT_TYPE::PLAYER;
}


PlayerUnit::~PlayerUnit()
{
}

void PlayerUnit::Init(void)
{
	//座標初期化
	SetDrawingPos(DRAWING_OFFSET_X);


	//UI情報の初期化
	unitUi_ = new PlayerUI(pos_, name_, hp_ ,changeHp_, maxHp_, isAct_);
	unitUi_->Init();

	//初期化
	UnitBase::Init();
	
}

void PlayerUnit::Update(void)
{

}

void PlayerUnit::Draw(void)
{
	//ユニット画像描画
	DrawUnitShader(1.0f);


	//UI描画
	unitUi_->Draw();

	//ベースユニットの描画関数
	UnitBase::Draw();

}


