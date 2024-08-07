#pragma once
#include "../ObjBase.h"

class StageBase : public ObjBase
{
public:
    StageBase(void);
    virtual ~StageBase(void);
    virtual void LoadData(void) = 0;
    virtual void Init(void) = 0;
    virtual void Update(void) = 0;
    virtual void Draw(void) = 0;

protected:

private:
};

