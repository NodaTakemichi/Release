#pragma once
#include "UnitBase.h"
class PlayerUnit :
    public UnitBase
{
public:
    PlayerUnit(const int& dataNum, const int& unitNum);
    ~PlayerUnit();

    void Init(void)override;
    void Update(void)override;
    void Draw(void)override;


private:
};

