#include "Deck.h"

Deck::Deck(void)
{
	dArray_ = { 0,0,0 };
}

Deck::Deck(int x, int y, int z)
{
	dArray_ = { x,y,z };
}

Deck::~Deck(void)
{
}

Deck& Deck::operator=(const Deck& vec)
{
	dArray_ = { vec.dArray_ };

	return *this;
}
