#include "DeckManager.h"

DeckManager* DeckManager::instance_ = nullptr;

void DeckManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new DeckManager();
	}
	instance_->Init();
}

DeckManager& DeckManager::GetInstance(void)
{
	return *instance_;
}

void DeckManager::Init(void)
{
	//初期デッキ
	deck_ = { 100,101,102 };

	//初期所持ユニット
	unitPool_ = { 100,101,102,103};

}

void DeckManager::Release(void)
{
}

void DeckManager::SetDeck(const Deck& deck)
{
	deck_ = deck;
}

void DeckManager::AddUnit(const int& unitNum)
{
	if (unitNum <= -1)return;

	//既に存在する要素がある場合、追加処理をしない
	for (auto& num : unitPool_)
	{
		if (num == unitNum)return;
	}

	//ユニットの追加
	unitPool_.push_back(unitNum);

}

DeckManager::DeckManager(void)
{
}

DeckManager::~DeckManager(void)
{
}
