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
	//�����f�b�L
	deck_ = { 100,101,102 };

	//�����������j�b�g
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

	//���ɑ��݂���v�f������ꍇ�A�ǉ����������Ȃ�
	for (auto& num : unitPool_)
	{
		if (num == unitNum)return;
	}

	//���j�b�g�̒ǉ�
	unitPool_.push_back(unitNum);

}

DeckManager::DeckManager(void)
{
}

DeckManager::~DeckManager(void)
{
}
