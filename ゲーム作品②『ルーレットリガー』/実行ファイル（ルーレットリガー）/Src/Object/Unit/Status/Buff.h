#pragma once
#include<vector>
//#include<array>
class Buff
{
public:
	//バフのタイプ
	enum class BUFF_TYPE
	{
		P_DOWN,
		P_UP,
		AVOIDANCE,
		D_DOWN,
		D_UP,
		S_DOWN,
		S_UP,
		POISON,
		CONFUSION,
		PALALYSIS,
		MAX
	};


	Buff();
	~Buff();

	void Init(void);
	void Release(void);

	//バフの生成関数
	void CreateBuff(const BUFF_TYPE& buff);

	//生存判定
	const bool& IsAlive(void)const { return isAlive_; }

	//バフの取得関数
	const BUFF_TYPE& GetBuff(void) const{ return buff_; }

	//バフの所有チェック
	bool CheckOwnBuff(const BUFF_TYPE& type);

	//バフの持続ターン計算
	bool DecBuffTurn(void);

	void SetAlive(bool alive) { isAlive_ = alive; }	//生死状態をセットする


private:
	//所有判定
	bool isAlive_;
	//持続ターン数
	int turn_;

	//所有バフ
	BUFF_TYPE buff_;


	//バフの付与
	void CheckBuffTurn(const BUFF_TYPE& type);

};

