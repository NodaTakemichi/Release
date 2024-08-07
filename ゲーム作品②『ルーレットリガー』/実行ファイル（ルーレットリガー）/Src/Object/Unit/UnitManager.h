#pragma once
#include<vector>
#include<array>
#include<map>
#include"../../Common/Deck.h"

class UnitBase;
class PlayerUnit;
class EnemyUnit;
class Command;

class UnitManager
{
public:
	UnitManager();
	~UnitManager();

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	//���j�b�g�̐���
	void CreateUnit(const Deck& pDeck, const Deck& eDeck);


	//�s�����̃��j�b�g�̃R�}���h�����擾
	std::vector<Command*> GetCommand(void);

	//�s�����j�b�g�̎擾
	UnitBase* GetActivUnit(void);
	//�S���j�b�g�̎擾
	std::vector <UnitBase*> GetUnits(void) { return units_; }

	//�s�����j�b�g��ύX����֐�
	void ChangeActivUnit(void);
	
	//���j�b�g���S�ł��ǂ������f
	bool IsAnniUnit(void);

private:
	//�ÓI�C���X�^���X
	static UnitManager* instance_;
	
	//���j�b�g�̔z��
	std::vector<UnitBase*> units_;


	//�X�s�[�h���������ɕ��ѕς���
	void SpeedSort(void);

	//�S���j�b�g���s�������j�b�g����͂���
	void NotActUnitAll(void);
};

