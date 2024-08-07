#include <math.h>
#include <DxLib.h>
#include "../Manager/DataManager/SoundManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Utility/Utility.h"
#include "../Object/Unit/UnitBase.h"
#include "../Object/Unit/Status/Command.h"

#include "../_debug/_DebugDispOut.h"

#include"DeathStaging.h"
#include "BattleSystem.h"


BattleSystem::BattleSystem()
{
	targetNum_= 0;
	randUnit_= 0;
	targetSelectSE_= 0;	
	targetDecSE_= 0;		
	damageSE_= 0;
	HealSE_= 0;	
	paraSE_= 0;	
	poisonSE_= 0;	
	avoiSE_= 0;	
	statusUpSE_= 0;	
	statusDownSE_= 0;	
	totalTime_=0.0f;

}

BattleSystem::~BattleSystem()
{
}

void BattleSystem::Init(void)
{
	targetNum_ = 0;
	totalTime_ = 0.0f;
	randUnit_ = 0;

	//SE登録
	auto& snd = SoundManager::GetInstance();
	targetSelectSE_ = snd.LoadSound(SoundManager::SOUND_TYPE::TARGET_SELECT);
	targetDecSE_ == snd.LoadSound(SoundManager::SOUND_TYPE::TARGET_DEC);

	damageSE_ = snd.LoadSound(SoundManager::SOUND_TYPE::DAMAGE);
	HealSE_ = snd.LoadSound(SoundManager::SOUND_TYPE::HEAL);

	paraSE_		= snd.LoadSound(SoundManager::SOUND_TYPE::PARA);
	poisonSE_	= snd.LoadSound(SoundManager::SOUND_TYPE::POISON);
	avoiSE_		= snd.LoadSound(SoundManager::SOUND_TYPE::AVOI);
	statusUpSE_ = snd.LoadSound(SoundManager::SOUND_TYPE::STATUS_UP);
	statusDownSE_ = snd.LoadSound(SoundManager::SOUND_TYPE::STATUS_DOWN);

}

void BattleSystem::Release(void)
{
	targetUnits_.clear();
	selectedUnits_.clear();
}

void BattleSystem::ProcessDamege(void)
{
	//ダメージSE
	auto& snd = SoundManager::GetInstance();
	snd.PlaySoundBack(damageSE_);

	//ダメージ計算
	auto dmg = roundf(actUnit_->GetAttack() * actCmd_->GetTimes());
	for (auto& unit : targetUnits_)
	{
		//AVOIDANCE状態のユニットは、ダメージ処理をしない
		if (unit->CheckOwnBuff(Buff::BUFF_TYPE::AVOIDANCE))continue;

		unit->Damage(dmg);
	}
}

void BattleSystem::ProcessHeal(void)
{
	//ヒールSE
	auto& snd = SoundManager::GetInstance();
	snd.PlaySoundBack(HealSE_);

	//回復計算
	auto heal = roundf(actUnit_->GetAttack() * actCmd_->GetTimes());
	for (auto& unit : targetUnits_)
	{
		unit->Heal(heal);
	}
}

void BattleSystem::ProcessBuff(void)
{
	//バフの付与
	auto buff = actCmd_->GetCmdBuff();
	for (auto& unit : targetUnits_)
	{
		unit->GiveBuff(buff);
	}

	//バフSE再生
	PlayBuffSE(buff);
}

void BattleSystem::PlayBuffSE(const Buff::BUFF_TYPE& type)
{
	auto& snd = SoundManager::GetInstance();

	switch (type)
	{
	case Buff::BUFF_TYPE::PALALYSIS:
		snd.PlaySoundBack(paraSE_);
		break;
	case Buff::BUFF_TYPE::POISON:
		snd.PlaySoundBack(poisonSE_);
		break;
	case Buff::BUFF_TYPE::AVOIDANCE:
		snd.PlaySoundBack(avoiSE_);
		break;
	case Buff::BUFF_TYPE::P_UP:
	case Buff::BUFF_TYPE::D_UP:
	case Buff::BUFF_TYPE::S_UP:
		snd.PlaySoundBack(statusUpSE_);
		break;
	case Buff::BUFF_TYPE::P_DOWN:
	case Buff::BUFF_TYPE::D_DOWN:
	case Buff::BUFF_TYPE::S_DOWN:
		snd.PlaySoundBack(statusDownSE_);
		break;
	default:
		break;
	}
}

void BattleSystem::SetBattleData(UnitBase* unit, Command* cmd, std::vector<UnitBase*> units)
{
	actUnit_ = unit;
	actCmd_ = cmd;
	units_ = units;
}

void BattleSystem::ResetSelectUnits(void)
{
	targetNum_ = 0;
	totalTime_ = 0.0f;

	//ターゲット
	for (auto target : targetUnits_)
	{
		target->SetTargeted(false);
	}
	targetUnits_.clear();

	//選択できるユニット
	for (auto select : selectedUnits_)
	{
		select->SetTargeted(false);
	}
	selectedUnits_.clear();
}

void BattleSystem::CheckSelectTarget(void)
{
	//Select配列に入れたユニットを、
	//全員対象とするか、その中から一体選ぶか判断する

	//コマンドの対象
	auto& target = actCmd_->GetCmdTarget();

	switch (target)
	{
	case Command::CMD_TARGET::NONE:
		return;
	case Command::CMD_TARGET::ENEMY:
		SetTargetUnits(false);
		targetUnits_.push_back(selectedUnits_[0]);
		break;
	case Command::CMD_TARGET::ENEMY_ALL:
		SetTargetUnits(false);
		targetUnits_ = selectedUnits_;
		break;
	case Command::CMD_TARGET::ENEMY_RAND:
		SetTargetUnits(false);
		break;
	case Command::CMD_TARGET::SELF:
		selectedUnits_.push_back(actUnit_);
		targetUnits_ = selectedUnits_;
		break;
	case Command::CMD_TARGET::PLAYER:
		SetTargetUnits(true);
		targetUnits_.push_back(selectedUnits_[0]);
		break;
	case Command::CMD_TARGET::PLAYER_ALL:
		SetTargetUnits(true);
		targetUnits_ = selectedUnits_;
		break;
	case Command::CMD_TARGET::END:
		break;
	default:
		break;
	}
}

bool BattleSystem::SelectUnit(const bool& autom)
{
	//コマンドの対象になっているユニットを、協調表示させる
	for (auto& target : targetUnits_)
	{
		target->SetTargeted(true);
	}

	//コマンドの対象選択
	auto& target = actCmd_->GetCmdTarget();
	switch (target)
	{
	case Command::CMD_TARGET::ENEMY:
	case Command::CMD_TARGET::PLAYER:
		return SelectInTarget(autom);
	case Command::CMD_TARGET::ENEMY_RAND:
		return SelectInTarget(true);
	default:
		break;
	}

	//対象が既に選択済み、又は必要なし
	if (autom)
	{
		return Utility::OverTime(totalTime_, STOP_WAIT_TIME_AIM);
	}
	else
	{
		//入力情報
		auto& ins = InputManager::GetInstance();
		//クリックしたら、選択完了
		return ins.IsTrgMouseLeft();
	}
}

void BattleSystem::CmdProcess(void)
{
	auto& type = actCmd_->GetCmdType();
	switch (type)
	{
	case Command::CMD_TYPE::MISS:
		break;
	case Command::CMD_TYPE::ATTACK:
		ProcessDamege();
		break;
	case Command::CMD_TYPE::HEAL:
		ProcessHeal();
		break;
	case Command::CMD_TYPE::BUFF:
		ProcessBuff();
		break;
	case Command::CMD_TYPE::CMD_UP:
		break;
	default:
		break;
	}

}

void BattleSystem::SetRandUnit(void)
{
	//対象がいない敵は無視
	auto& target = actCmd_->GetCmdTarget();
	if (target == Command::CMD_TARGET::NONE)return;

	//ランダム値
	randUnit_ = SceneManager::GetInstance().GetRand(0, selectedUnits_.size() - 1);
}

bool BattleSystem::FinishedDecHP(void)
{
	bool finish = true;
	for (auto& unit : targetUnits_)
	{
		//死亡演出中の場合、処理を停止
		auto st = DeathStaging::GetInstance().PlayingStaging();
		if (st)return false;

		//ユニットのHPを徐々に減少させる処理
		//一つでも未終了がある場合、finishをfalseにする
		finish &= unit->DecHpProcess();
	}

	return finish;
}

bool BattleSystem::FinishedBuffEffect(void)
{
	bool finish = true;
	for (auto& unit : targetUnits_)
	{
		//ユニットのバフエフェクトを再生する処理
		//一つでも未終了がある場合、finishをfalseにする
		finish &= unit->PlayBuffEffect();
	}

	return finish;
}

void BattleSystem::SetTargetUnits(const bool& equal)
{
	for (auto& unit : units_)
	{
		if (!unit->IsAlive())continue;	//死亡状態の場合、処理しない
		
		//コマンド対象に味方を入れるか、相手を入れるかの判断
		if (equal)
		{
			//行動ユニットの相手の場合、処理をしない
			if (actUnit_->GetUnitType() != unit->GetUnitType())continue;
		}
		else
		{
			//行動ユニットの味方の場合、処理をしない
			if (actUnit_->GetUnitType() == unit->GetUnitType())continue;
		}

		//選択ユニットに格納
		selectedUnits_.push_back(unit);
	}
}

bool BattleSystem::SelectInTarget(const bool& autom)
{

	if (autom)
	{
		//待機時間をオーバーしたら、処理
		auto time = Utility::OverTime(totalTime_, STOP_WAIT_TIME_AIM);
		if (!time)return false;

		//時間リセット
		totalTime_ = 0.0f;
		//ランダム値とターゲット番号の合否を判断
		if (randUnit_ == targetNum_)
		{
			return true;
		}
		else
		{
			//対象ユニットしていたリセット
			if (targetUnits_[0] != nullptr)
			{
				targetUnits_[0]->SetTargeted(false);
			}
			targetUnits_.clear();

			//ランダム値と合うように、ターゲット番号を変更
			targetNum_++;
			targetNum_ = Utility::Wrap(targetNum_, 0, selectedUnits_.size());
			//選択したユニットを格納する
			targetUnits_.push_back(selectedUnits_[targetNum_]);

			//対象選択SE
			auto& snd = SoundManager::GetInstance();
			snd.PlaySoundBack(targetSelectSE_);

			return false;
		}
	}
	else
	{
		//ホイールの入力
		int wheel = GetMouseWheelRotVol();

		//選択入力がある場合、対象ユニットを変更する
		if (wheel != 0)
		{
			//対象ユニットをリセット
			if (targetUnits_[0] != nullptr)
			{
				targetUnits_[0]->SetTargeted(false);
			}
			targetUnits_.clear();


			//入力が、正方向か負方向か判断
			//正：プラス１　、　負：マイナス１
			targetNum_ += wheel > 0 ? 1 : -1;

			//ナンバーを範囲内に収める
			int max = selectedUnits_.size();
			targetNum_ = Utility::Wrap(targetNum_, 0, max);

			//選択したユニットを格納する
			targetUnits_.push_back(selectedUnits_[targetNum_]);

			//対象選択SE
			auto& snd = SoundManager::GetInstance();
			snd.PlaySoundBack(targetSelectSE_);
		}

		//入力情報
		auto& ins = InputManager::GetInstance();
		//決定
		if (ins.IsClickMouseLeft())
		{
			return true;
		}
	}


	return false;
}

