#pragma once
#include<array>
class Deck
{
public:
	//�R�̃Z�b�g�\
	std::array<int, 3>dArray_;

	//�R���X�g���N�^
	Deck(void);
	Deck(int x, int y, int z);
	~Deck(void);

	//���
	Deck& operator=(const Deck& vec);

};

