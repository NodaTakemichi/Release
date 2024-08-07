#include "Buff.h"

Buff::Buff()
{
	isAlive_=false;
	turn_=0;
}

Buff::~Buff()
{
}

void Buff::Init(void)
{
	CheckBuffTurn(buff_);
	isAlive_ = true;
}

void Buff::Release(void)
{
}

void Buff::CreateBuff(const BUFF_TYPE& buff)
{
	buff_ = buff;
	CheckBuffTurn(buff_);
	isAlive_ = true;
}

void Buff::CheckBuffTurn(const BUFF_TYPE& type)
{
	//バフの持続ターン
	int turn = 0;
	switch (type)
	{
	case Buff::BUFF_TYPE::PALALYSIS:
		turn = 1;
		break;
	case Buff::BUFF_TYPE::AVOIDANCE:
		turn = 2;
		break;
	default:
		//その他
		turn = 3;
		break;
	}


	//データ代入
	turn_ = turn;
}

bool Buff::CheckOwnBuff(const BUFF_TYPE& type)
{
	//目当てのバフを所有していた場合、trueを返す
	if (buff_ == type)return true;

	return false;
}

bool Buff::DecBuffTurn(void)
{
	turn_--;
	if (turn_ <= 0)return true;
	return false;
}
