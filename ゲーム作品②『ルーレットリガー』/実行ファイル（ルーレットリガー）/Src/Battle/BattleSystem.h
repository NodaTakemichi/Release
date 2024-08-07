#pragma once
#include<vector>
#include"../Object/Unit/Status/Buff.h"

class UnitBase;
class Command;

class BattleSystem
{
public:
	BattleSystem();
	~BattleSystem();

	void Init(void);
	void Release(void);



	//バトルに必要な情報をセットする
	void SetBattleData(UnitBase* unit, Command* cmd, 
		std::vector<UnitBase*> units);

	//選択ユニットのリセットする
	void ResetSelectUnits(void);
	//コマンドの効果対象によって、対象を判断する
	void CheckSelectTarget(void);
	//実際にアクションをする対象を選択する
	bool SelectUnit(const bool& autom);

	//コマンド処理
	void CmdProcess(void);

	//選択ユニットからランダムに決める
	void SetRandUnit(void);

	//HPのダメージ減少が終了したか判断（ture:終了 , false:未終了）
	bool FinishedDecHP(void);

	//ユニットのバフエフェクトが終了したか判断（ture:終了 , false:未終了）
	bool FinishedBuffEffect(void);

	//攻撃ターゲットユニットの座標取得
	const std::vector<UnitBase*>& GetTargetUnit(void) { return targetUnits_; }

private:
	//対象選択までの待機時間
	const float STOP_WAIT_TIME_AIM = 0.8f;



	UnitBase* actUnit_;				//行動ユニット
	Command* actCmd_;				//発動コマンド
	std::vector<UnitBase*> units_;	//全ユニット

	//対象ナンバー
	int targetNum_;

	//選択できるユニット情報を格納
	std::vector<UnitBase*> selectedUnits_;	

	//選択ユニットから、実際に対象とするユニットを格納
	std::vector<UnitBase*> targetUnits_;

	//合計時間
	float totalTime_;
	//ランダムユニット
	int randUnit_;

	//SE関連
	int targetSelectSE_;	//対象選択SE
	int targetDecSE_;		//対象決定SE

	int damageSE_;	//ダメージSE
	int HealSE_;	//ヒールSE

	int paraSE_;	//麻痺SE
	int poisonSE_;	//毒SE
	int avoiSE_;	//回避SE
	int statusUpSE_;	//ステータスアップSE
	int statusDownSE_;	//ステータスダウンSE



	/// 対象のユニット情報を取得する
	/// <param name="equal">true:味方、false:相手</param>
	void SetTargetUnits(const bool& equal);
	//対象がいる中から、選択する
	bool SelectInTarget(const bool& autom);

	//ダメージ処理
	void ProcessDamege(void);
	//回復処理
	void ProcessHeal(void);
	//バフ付与処理
	void ProcessBuff(void);

	//バフSE再生
	void PlayBuffSE(const Buff::BUFF_TYPE& type);
};

