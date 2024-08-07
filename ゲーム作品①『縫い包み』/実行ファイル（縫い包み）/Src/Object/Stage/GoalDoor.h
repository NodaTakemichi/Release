#pragma once
#include "StageBase.h"

class GoalDoor :
    public StageBase
{
public:
	GoalDoor(void);
	virtual ~GoalDoor(void);

	void LoadData(void)override;
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;


private:
};

