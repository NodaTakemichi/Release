#include "Amulet.h"

Amulet::Amulet(void)
{
}

Amulet::~Amulet(void)
{
}

void Amulet::LoadData(void)
{
}

void Amulet::Init(void)
{
}

void Amulet::Update(void)
{
}

void Amulet::Draw(void)
{
	if (GetHasItem())return;
	MV1DrawModel(transform_.modelId_);
}
