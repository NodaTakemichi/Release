#pragma once
#include "UnitUI.h"
class EnemyUI :
    public UnitUI
{
public:
	EnemyUI(Vector2 pos, std::string& name, int& hp, int& nowHp, int& maxHp, bool& active);
	~EnemyUI();

	void Init(void)override;
	void Draw(void)override;


private:
	//ÉçÉbÉNÉIÉì
	void DrawRockOn(void)override;

};

