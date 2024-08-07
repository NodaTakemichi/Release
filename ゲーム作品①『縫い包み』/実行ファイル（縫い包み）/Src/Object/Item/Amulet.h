#pragma once
#include "ItemBase.h"
class Amulet :
    public ItemBase
{
public:
    Amulet(void);
    virtual ~Amulet(void);
    void LoadData(void)override;
    void Init(void)override;
    void Update(void)override;
    void Draw(void)override;

private:

};

