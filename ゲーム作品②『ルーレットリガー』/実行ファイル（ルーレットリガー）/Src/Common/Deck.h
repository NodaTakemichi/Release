#pragma once
#include<array>
class Deck
{
public:
	//３体セット可能
	std::array<int, 3>dArray_;

	//コンストラクタ
	Deck(void);
	Deck(int x, int y, int z);
	~Deck(void);

	//代入
	Deck& operator=(const Deck& vec);

};

