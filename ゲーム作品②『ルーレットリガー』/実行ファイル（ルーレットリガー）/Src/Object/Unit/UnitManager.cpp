#include<algorithm>
#include<iostream>
#include<tuple>
#include "./PlayerUnit.h"
#include "./EnemyUnit.h"
#include"../../_debug/_DebugConOut.h"
#include "UnitManager.h"

UnitManager::UnitManager()
{
}

UnitManager::~UnitManager()
{
}

void UnitManager::Init(void)
{

}

void UnitManager::Update(void)
{
	//ユニットの更新
	for (auto& unit : units_)
	{
		unit->Update();
	}
}

void UnitManager::Draw(void)
{
	//ユニットの描画
	for (auto& unit : units_)
	{
		unit->Draw();
	}
}

void UnitManager::Release(void)
{
	//ユニットの破棄
	for (auto& unit : units_)
	{
		unit->Release();
		delete unit;
	}
	units_.clear();
}

void UnitManager::CreateUnit(const Deck& pDeck, const Deck& eDeck)
{
	//ユニットの生成
	//味方
	int pNum = 1;
	for (auto& unitNum : pDeck.dArray_)
	{
		//生成しない
		if (unitNum == -1)continue;

		PlayerUnit* pUnit = new PlayerUnit(unitNum, pNum);
		pUnit->Init();
		units_.push_back(pUnit);

		pNum++;
	}

	//敵
	int eNum = 1;
	for (auto& unitNum : eDeck.dArray_)
	{
		//生成しない
		if (unitNum == -1)continue;

		EnemyUnit* eUnit = new EnemyUnit(unitNum, eNum);
		eUnit->Init();
		units_.push_back(eUnit);

		eNum++;
	}



	//スピード降順ソート
	SpeedSort();

}

std::vector<Command*> UnitManager::GetCommand(void)
{
	//行動ユニットのコマンドを渡す
	for (auto& unit : units_)
	{
		if (unit->IsAct())return unit->GetCommands();
	}

	TRACE("行動ユニットのコマンドが見つかりませんでした");
	return std::vector<Command*>();
}

UnitBase* UnitManager::GetActivUnit(void)
{
	//各ユニットから,行動が出来て最もスピードが速いユニットを探す。
	for (auto& unit : units_)
	{
		if (unit->IsAct())return unit;
	}

	TRACE("最速ユニットが見つかりませんでした");
	return nullptr;
}


void UnitManager::SpeedSort(void)
{
	//ユニットの配列を、スピードが大きい順にソートする。

	//スピード速度の比較
	auto compare = [&](UnitBase* x, UnitBase* y) {
		auto speedX = x->GetSpeed();
		auto speedY = y->GetSpeed();
		return  speedX > speedY;
	};
	//ソート
	std::sort(units_.begin(), units_.end(), compare);
}

void UnitManager::ChangeActivUnit(void)
{
	//全ユニットを、行動中ユニットから外す
	NotActUnitAll();

	//各ユニットから,行動が出来て最もスピードが速いユニットを探す。
	for (auto& unit : units_)
	{
		if (!unit->IsAlive())continue;	//死亡時、処理をしない
		if (unit->IsActed())continue;	//行動済みの場合、処理をしない

		//行動済み状態にする
		unit->SetActed(true);
		//行動中ユニットにする
		unit->SetAct(true);

		//行動できる中で、最も行動速度が速いユニットを行動ユニットにする
		return;
	}

	//すべて行動不可の場合、生存ユニットを行動可能状態にする
	for (auto& unit : units_)
	{
		if (!unit->IsAlive())continue;	//死亡時、処理をしない

		//行動可能状態にする
		unit->SetActed(false);
	}
	//もう一度、ユニット検索を行う。
	ChangeActivUnit();

	//return;
}

bool UnitManager::IsAnniUnit(void)
{
	//片方の生存ユニットを確認できなければ、全滅判定

	bool pUnit = false;
	bool eUnit = false;


	for (auto& unit : units_)
	{
		//両陣営、生存している
		if (pUnit && eUnit)return false;

		//生存ユニット
		if (!unit->IsAlive())continue;

		if (unit->GetUnitType() == UnitBase::UNIT_TYPE::PLAYER)
		{
			pUnit = true;
		}
		else
		{
			eUnit = true;
		}
	}


	//排他的論理和
	//片方陣営が全滅していたら、true
	return pUnit ^ eUnit;
}

void UnitManager::NotActUnitAll(void)
{
	for (auto& unit : units_)
	{
		//行動中ユニットから外す
		unit->SetAct(false);
	}
}
