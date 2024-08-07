#pragma once
#include<vector>
#include<string>
#include"./Buff.h"

struct Par
{
	//コマンド名
	std::string name_;
	//コマンドタイプ(キャスト前)
	std::string type_;
	//ターゲット(キャスト前)
	std::string target_;

	//技：倍率
	float times_;

	//バフタイプ(キャスト前)
	std::string buff_;

	//エフェクト番号
	int efNum_;
};


class Command
{
public:

	//技のタイプ
	enum class CMD_TYPE
	{
		NONE = 0, 
		MISS,
		ATTACK,
		HEAL,
		BUFF,
		CMD_UP,
		END
	};


	//技の対象
	enum class CMD_TARGET
	{
		NONE = 0,
		ENEMY,
		ENEMY_ALL,
		ENEMY_RAND,
		SELF,
		PLAYER,
		PLAYER_ALL,
		END
	};

	Command(const int& num);
	~Command();

	void Init(void);
	void Release(void);

	//名前の取得
	const std::string& GetName(void) { return par_.name_; }

	//倍率の取得
	const float& GetTimes(void) { return par_.times_; }

	//コマンドタイプの取得
	const CMD_TYPE& GetCmdType(void) { return type_; }

	//ターゲットの取得
	const CMD_TARGET& GetCmdTarget(void) { return target_; }

	//バフのタイプの取得
	const Buff::BUFF_TYPE& GetCmdBuff(void) { return buff_; }

	//エフェクト番号の取得
	const int& GetEffectNum(void) { return par_.efNum_; }

private:
	//コマンド情報
	Par par_;

	//コマンド番号
	const int& num_;

	//コマンドタイプ
	CMD_TYPE type_;

	//ターゲット
	CMD_TARGET target_;

	//バフの種類
	Buff::BUFF_TYPE buff_;


	//スキルタイプ、ターゲット、バフタイプをキャストする
	void CastCmdType(std::string type);
	void CastCmdTarget(std::string target);
	void CastCmdBuff(std::string buff);




};

