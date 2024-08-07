#include "../../Application.h"
#include "./UI//EnemyUI.h"
#include "EnemyUnit.h"



EnemyUnit::EnemyUnit(const int& dataNum, const int& unitNum) :
	UnitBase(dataNum, unitNum)
{
	type_ = UNIT_TYPE::ENEMY;
}

EnemyUnit::~EnemyUnit()
{
}

void EnemyUnit::Init(void)
{
	//座標初期化
	auto ScreenX = Application::SCREEN_SIZE_X;
	SetDrawingPos(ScreenX - DRAWING_SIZE - DRAWING_OFFSET_X);

	//UI情報の初期化
	unitUi_ = new EnemyUI(pos_, name_, hp_ ,changeHp_, maxHp_, isAct_);
	unitUi_->Init();


	//初期化
	UnitBase::Init();

}


void EnemyUnit::Update(void)
{
}

void EnemyUnit::Draw(void)
{
	//ユニット画像描画
	DrawUnitShader(0.0f);

	//UI描画
	unitUi_->Draw();

	//ベースユニットの描画関数
	UnitBase::Draw();

}


