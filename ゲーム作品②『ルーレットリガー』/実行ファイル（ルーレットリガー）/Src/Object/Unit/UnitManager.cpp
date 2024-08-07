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
	//���j�b�g�̍X�V
	for (auto& unit : units_)
	{
		unit->Update();
	}
}

void UnitManager::Draw(void)
{
	//���j�b�g�̕`��
	for (auto& unit : units_)
	{
		unit->Draw();
	}
}

void UnitManager::Release(void)
{
	//���j�b�g�̔j��
	for (auto& unit : units_)
	{
		unit->Release();
		delete unit;
	}
	units_.clear();
}

void UnitManager::CreateUnit(const Deck& pDeck, const Deck& eDeck)
{
	//���j�b�g�̐���
	//����
	int pNum = 1;
	for (auto& unitNum : pDeck.dArray_)
	{
		//�������Ȃ�
		if (unitNum == -1)continue;

		PlayerUnit* pUnit = new PlayerUnit(unitNum, pNum);
		pUnit->Init();
		units_.push_back(pUnit);

		pNum++;
	}

	//�G
	int eNum = 1;
	for (auto& unitNum : eDeck.dArray_)
	{
		//�������Ȃ�
		if (unitNum == -1)continue;

		EnemyUnit* eUnit = new EnemyUnit(unitNum, eNum);
		eUnit->Init();
		units_.push_back(eUnit);

		eNum++;
	}



	//�X�s�[�h�~���\�[�g
	SpeedSort();

}

std::vector<Command*> UnitManager::GetCommand(void)
{
	//�s�����j�b�g�̃R�}���h��n��
	for (auto& unit : units_)
	{
		if (unit->IsAct())return unit->GetCommands();
	}

	TRACE("�s�����j�b�g�̃R�}���h��������܂���ł���");
	return std::vector<Command*>();
}

UnitBase* UnitManager::GetActivUnit(void)
{
	//�e���j�b�g����,�s�����o���čł��X�s�[�h���������j�b�g��T���B
	for (auto& unit : units_)
	{
		if (unit->IsAct())return unit;
	}

	TRACE("�ő����j�b�g��������܂���ł���");
	return nullptr;
}


void UnitManager::SpeedSort(void)
{
	//���j�b�g�̔z����A�X�s�[�h���傫�����Ƀ\�[�g����B

	//�X�s�[�h���x�̔�r
	auto compare = [&](UnitBase* x, UnitBase* y) {
		auto speedX = x->GetSpeed();
		auto speedY = y->GetSpeed();
		return  speedX > speedY;
	};
	//�\�[�g
	std::sort(units_.begin(), units_.end(), compare);
}

void UnitManager::ChangeActivUnit(void)
{
	//�S���j�b�g���A�s�������j�b�g����O��
	NotActUnitAll();

	//�e���j�b�g����,�s�����o���čł��X�s�[�h���������j�b�g��T���B
	for (auto& unit : units_)
	{
		if (!unit->IsAlive())continue;	//���S���A���������Ȃ�
		if (unit->IsActed())continue;	//�s���ς݂̏ꍇ�A���������Ȃ�

		//�s���ςݏ�Ԃɂ���
		unit->SetActed(true);
		//�s�������j�b�g�ɂ���
		unit->SetAct(true);

		//�s���ł��钆�ŁA�ł��s�����x���������j�b�g���s�����j�b�g�ɂ���
		return;
	}

	//���ׂčs���s�̏ꍇ�A�������j�b�g���s���\��Ԃɂ���
	for (auto& unit : units_)
	{
		if (!unit->IsAlive())continue;	//���S���A���������Ȃ�

		//�s���\��Ԃɂ���
		unit->SetActed(false);
	}
	//������x�A���j�b�g�������s���B
	ChangeActivUnit();

	//return;
}

bool UnitManager::IsAnniUnit(void)
{
	//�Е��̐������j�b�g���m�F�ł��Ȃ���΁A�S�Ŕ���

	bool pUnit = false;
	bool eUnit = false;


	for (auto& unit : units_)
	{
		//���w�c�A�������Ă���
		if (pUnit && eUnit)return false;

		//�������j�b�g
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


	//�r���I�_���a
	//�Е��w�c���S�ł��Ă�����Atrue
	return pUnit ^ eUnit;
}

void UnitManager::NotActUnitAll(void)
{
	for (auto& unit : units_)
	{
		//�s�������j�b�g����O��
		unit->SetAct(false);
	}
}
