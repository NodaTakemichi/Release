#pragma once
#include<vector>
#include<array>
#include"../Common/Deck.h"

class DeckManager
{
public:
	// �����I�ɃC���X�e���X�𐶐�����
	static void CreateInstance(void);

	// �ÓI�C���X�^���X�̎擾
	static DeckManager& GetInstance(void);

	// ������
	void Init(void);

	// ���
	void Release(void);

	//�f�b�L���̎擾
	const Deck& GetDeck(void) { return deck_; }
	//���j�b�g�v�[�����̎擾
	const std::vector<int>& GetUnitPool(void) { return unitPool_; }

	//�f�b�L���̃Z�b�g
	void SetDeck(const Deck& deck);

	//���j�b�g�̒ǉ�
	void AddUnit(const int& unitNum);

private:
	// �ÓI�C���X�^���X
	static DeckManager* instance_;

	//�f�b�L�J�[�h
	Deck deck_;

	//���j�b�g�v�[��
	std::vector<int>unitPool_;


	// �O�����琶���ł��Ȃ��l�ɂ���
	DeckManager(void);
	// �f�X�g���N�^�����l
	~DeckManager(void);

};

