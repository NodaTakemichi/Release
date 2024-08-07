#pragma once
#include<memory>
class Player;
class OperationUI
{
public:

	OperationUI(void);
	~OperationUI(void);

	void LoadData(void);
	void Init(void);
	void Update(void);
	void Draw(void);

private:

	int opeMoveImg_;
	int opeDashImg_;
	int opeLeftClickImg_;
	int opeRightClickImg_;
	int opePauseImg_;

};

