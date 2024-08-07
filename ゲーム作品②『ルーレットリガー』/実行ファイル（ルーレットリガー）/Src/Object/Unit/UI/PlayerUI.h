#pragma once
#include "UnitUI.h"
class PlayerUI :
    public UnitUI
{
public:
	PlayerUI(Vector2 pos, std::string& name, int& hp, int& nowHp, int& maxHp,bool& active);
	~PlayerUI();

	void Init(void)override;
	void Draw(void)override;


private:
	//ÉçÉbÉNÉIÉì
	void DrawRockOn(void)override;

};

