#include <algorithm>
#include "../../Manager/SceneManager.h"
#include "../../Manager/DataManager/UnitDataManager.h"
#include "../../Manager/DataManager/SoundManager.h"
#include "../../Utility/Utility.h"
#include "../../Battle/DeathStaging.h"

#include"../../_debug/_DebugConOut.h"
#include"../../_debug/_DebugDispOut.h"

#include "UI/UnitUI.h"
#include "UnitBase.h"
//バフエフェクト完了時間
constexpr float BAUU_EFFECT_COMP_TIME = 1.5f;
//毒のダメージパーセント
constexpr int POISON_DAMAGE_PARCENT = 8;
//ユニット座標
constexpr int ACTION_UNIT_SHOW_OFFSET_X = 70;	//行動ユニットのオフセットX
constexpr int UNIT_SHOW_BASE_POS_Y = 320;	//ベース座標Ｙ
constexpr int UNIT_SHOW_SPAN_POS_Y = 180;	//Ｙ座標間隔
//ダメージ乱数値
constexpr int DAMAGE_RANDOM_VALUE = 90;	
//ユニットの画像揺れ
constexpr float SHALE_VALUE = 50.0f;	//振幅量
constexpr float SHAKE_MOVE_POW = 20.0f;	//揺れの移動量
constexpr float SHAKE_STOP_VALUE = 0.6f;	//揺れ停止値
constexpr float SHAKE_DEC_VALUE = 0.8f;	//揺れ減少値


using PsType = DrawShader::PS_TYPE;

UnitBase::UnitBase(const int& dataNum, const int& unitNum) :
	unitDataNum_(dataNum), unitNum_(unitNum)
{
	unitImg_= 0;
	shakePosX_= 0;
	hp_= 0;
	beforHp_= 0;
	changeHp_= 0;
	maxHp_= 0;
	attack_= 0;
	speed_= 0;
	isActed_= false;		
	isAlive_= false;		
	isTargeted_= false;
	isAct_= false;		
	isPlayBuffEf_= false;
	totalTime_= 0.0f;
	shakeX_= 0.0f;
	shakeValue_= 0.0f;
	movePow_= 0.0f;
	buffEfTime_= 0.0f;
	std::string name_= "";
	pos_ = Vector2{};

}

UnitBase::~UnitBase()
{
}

void UnitBase::Init(void)
{
	//ユニットデータの登録
	LoadData();

	//生死状態、行動状態、現在行動状態をセットする
	SetAlive(true);
	SetActed(false);
	SetAct(false);
	SetTargeted(false);


	//現在のピクセルシェーダー(反転可能シェーダー)
	nowPs_ = PsType::REVERSE_TEXTURE;;

	//合計時間
	totalTime_ = 0.0f;

	//揺れ幅
	shakeX_ = 0.0f;
	shakeValue_ = 0.0f;
	movePow_ = 0.0f;

	//ずらし座標
	shakePosX_ = 0;
}

void UnitBase::Draw(void)
{
	//狙われているユニット
	if (IsTargeted())unitUi_->DrawRockOn();
}

void UnitBase::Release(void)
{
	//解放
	unitUi_->Release();

}

bool UnitBase::DecHpProcess(void)
{
	//HP変化がある時、現在HPが０以上の時のみ
	if (changeHp_ != hp_ && changeHp_ >= 0)
	{
		auto changeTime = 1.0f;		//完了時間

		//経過時間
		totalTime_ += SceneManager::GetInstance().GetDeltaTime();
		//経過　＝（完了する時間ー経過時間）/完了する時間
		auto progress = 1.0f - (changeTime - totalTime_) / changeTime;

		//ダメージ後とダメージ前の線形補間を行う
		changeHp_ = Utility::Lerp(beforHp_, hp_, progress);

		//被ダメの時、画像を揺らす
		if (hp_ < changeHp_)UnitImgShake(progress);


		//超過している、もしくは変化中HPが現在HPに追いついた時
		if (progress >= 1.0f || changeHp_ == hp_ || changeHp_ <= 0)
		{
			//ダメージ表記、回復値表記を非表示する
			unitUi_->SetDmgValue(false, 0);
			unitUi_->SetHealValue(false, 0);
			//経過時間のリセット
			totalTime_ = 0.0f;

			if (!IsAlive()) {
				changeHp_ = 0;
				hp_ = 0;

				//死亡演出の発動
				float reverse;
				if (GetUnitType() == UNIT_TYPE::ENEMY)
				{
					reverse = 0.0f;
				}
				else {
					reverse = 1.0f;
				}
				DeathStaging::GetInstance().SetDeathUnit(unitImg_, reverse);
			}

			//HP減少：終了
			return true;
		}
		//HP減少：未終了
		return false;
	}

	//HP変動：無し
	return true;
}

void UnitBase::TurnEndProcess(void)
{
	for (auto& buff : buffs_)
	{
		if (!buff->IsAlive())continue;

		//毒ダメージの処理
		if (buff->CheckOwnBuff(Buff::BUFF_TYPE::POISON))
		{
			TRACE("毒ダメージ\n");

			//最大HPの 1/8 のダメージ
			int dmg = maxHp_ / POISON_DAMAGE_PARCENT;
			Damage(dmg);
		}

		//バフのターン数減少処理
		if (buff->DecBuffTurn())
		{
			//持続ターンが終了したら、バフを無効状態にする
			buff->SetAlive(false);
		}
	}
}

const int& UnitBase::GetSpeed(void)
{
	//バフ込みのスピード計算
	int nowSpeed = CalcBuffStatus(
		speed_, Buff::BUFF_TYPE::S_UP, Buff::BUFF_TYPE::S_DOWN);

	return nowSpeed;
}

const int& UnitBase::GetAttack(void)
{
	//バフ込みの攻撃力計算
	int nowAttack = CalcBuffStatus(
		attack_, Buff::BUFF_TYPE::P_UP, Buff::BUFF_TYPE::P_DOWN);

	return nowAttack;
}

int UnitBase::CalcBuffStatus(const int& status, 
	const Buff::BUFF_TYPE& up, const Buff::BUFF_TYPE& down)
{
	float value = static_cast<float>(status);

	//バフの計算
	for (auto buff : buffs_)
	{
		if (!buff->IsAlive())continue;

		//ステータスアップ
		if (buff->CheckOwnBuff(up))value *= 1.2;
		//ステータスダウン
		if (buff->CheckOwnBuff(down))value *= 0.8;
	}

	return static_cast<int>(floor(value));
}

void UnitBase::SetAct(bool act)
{
	//行動ユニットは少しX座標を前に押し出す
	shakePosX_ = act ? ACTION_UNIT_SHOW_OFFSET_X : 0;
	//ずらし値
	shakePosX_ *= UNIT_TYPE::PLAYER == type_ ? 1 : -1;
	//行動状態
	isAct_ = act;
}

void UnitBase::Damage(const int& dmg)
{
	TRACE(name_.c_str());
	TRACE("\n攻撃値：%d ||", dmg);

	//バフ込みのダメージ計算
	auto calcDmg = CalcBuffStatus(
		dmg, Buff::BUFF_TYPE::D_DOWN, Buff::BUFF_TYPE::D_UP);

	//ダメージの乱数計算
	float randNum = static_cast<float>(
		SceneManager::GetInstance().GetRand(DAMAGE_RANDOM_VALUE, 100)) / 100.0f;
	calcDmg = Utility::Round(static_cast<float>(calcDmg) * randNum);
	TRACE("被ダメ値：%d\n", calcDmg);

	//直前HPの保持
	beforHp_ = hp_;
	//ダメージ計算
	hp_ -= calcDmg;
	//残り体力
	TRACE("残り体力:%d\n\n",hp_);

	//ダメージ表記を表示する
	unitUi_->SetDmgValue(true, calcDmg);
	//画像の揺れ幅の決定
	shakeValue_ = SHALE_VALUE;
	movePow_ = SHAKE_MOVE_POW;

	//死亡判定
	if (CheckDead())
	{
		TRACE("死亡しました\n");
		//シェーダーの変更(モノトーン)
		nowPs_ = PsType::MONOTONE;

		//死亡状態にする
		SetAlive(false);
	}
}

void UnitBase::Heal(const int& heal)
{
	//直前HPの記憶
	beforHp_ = hp_;
	//HP計算
	hp_ += heal;

	//回復値表記を表示する
	if (hp_ - maxHp_ < heal)unitUi_->SetHealValue(true, heal);
	
	//HPの上限計算
	hp_ = (std::min)(hp_, maxHp_);
}

void UnitBase::GiveBuff(const Buff::BUFF_TYPE& type)
{
	//バフの生成
	CreateBuff(type);

	//UIにバフの登録
	unitUi_->SetBuff(buffs_);

	//バフエフェクトの発動
	ActivBuffEf(type);
}

bool UnitBase::CheckDead(void)
{
	//死亡判定
	if (hp_ <= 0)
	{
		return true;
	}
	return false;
}

bool UnitBase::CheckOwnBuff(const Buff::BUFF_TYPE& type)
{
	for (auto& buff : buffs_)
	{
		if (!buff->IsAlive())continue;

		//指定バフを所有している
		if (buff->CheckOwnBuff(type))
		{
			return true;
		}
	}
	return false;
}

bool UnitBase::PlayBuffEffect(void)
{
	//バフエフェクトの再生終了判断
	float wait = DX_PI_F / BAUU_EFFECT_COMP_TIME;
	if (Utility::OverTime(buffEfTime_, wait))
	{
		isPlayBuffEf_ = false;

		//ユニットシェーダーを戻す
		nowPs_ = PsType::REVERSE_TEXTURE;

		return true;
	}
	return false;
}

void UnitBase::LoadData(void)
{
	//ユニットデーターの取得
	auto& data = UnitDataManager::GetInstance().GetUnitData(unitDataNum_);

	//パラメータセット
	name_ = data.name;
	unitImg_ = data.imgHandle;
	hp_ = data.hp;
	attack_ = data.attack;
	speed_ = data.speed;
	//コマンドセット
	for (auto& cmd : data.cmdNum)
	{
		//コマンドの作成
		CreateCommand(cmd);
	}

	//HP関連の初期化
	maxHp_ = beforHp_= changeHp_ = hp_;

}

void UnitBase::DrawUnitShader(const float& revers)
{
	auto time = SceneManager::GetInstance().GetTotalTime();
	auto& ds = DrawShader::GetInstance();

	//現在座標　＋　揺れ幅　＋　ずらし値
	Vector2 pos = { pos_.x + static_cast<int>(shakeX_) + shakePosX_,pos_.y };
	//定数バッファ
	COLOR_F buf = {
		revers,					//反転するかどうか
		buffEfTime_,			//経過時間
		BAUU_EFFECT_COMP_TIME	//バフエフェクト完了時間
	};

	//描画
	ds.DrawExtendGraphToShader(
		pos, { DRAWING_SIZE ,DRAWING_SIZE }, unitImg_, nowPs_, buf);

}

void UnitBase::UnitImgShake(const float& leap)
{
	if (leap >= SHAKE_STOP_VALUE)
	{
		shakeX_ = 0.0f;
		return;
	}

	//画像座標の振動値加算
	shakeX_ += movePow_;

	// 値が最大値または最小値を超えた場合、反転させる
	if (shakeValue_ <= shakeX_ || (shakeValue_ * -1) >= shakeX_)
	{
		//反転
		movePow_ *= -SHAKE_DEC_VALUE;

		//振幅値を減少させる
		shakeValue_ *= (1.0f - leap);

		auto i = 1;
		if ((shakeValue_ * -1) >= shakeX_)i = -1;
		shakeX_=shakeValue_* i + movePow_;
	}
}

void UnitBase::CreateCommand(const int& num)
{
	//コマンドの生成
	auto cmd = new Command(num);
	cmd->Init();
	commands_.push_back(cmd);
}

void UnitBase::CreateBuff(const Buff::BUFF_TYPE& type)
{
	for (auto& buff : buffs_)
	{
		//死んでいたら、処理をしない
		if (!buff->IsAlive())continue;

		//重複していた場合、追加しない
		if (buff->GetBuff() == type)return;

		//反発しあうバフの場合、対消滅させる
		auto checkBuff = [&](Buff::BUFF_TYPE get, Buff::BUFF_TYPE give) {
			if (buff->GetBuff() == get)
			{
				if (type == give)
				{
					//消滅
					buff->SetAlive(false);

					//生成せずに、処理を終了
					return true;
				}
			}
			return false;
		};

		//反発しあうバフ
		if (checkBuff(Buff::BUFF_TYPE::P_UP, Buff::BUFF_TYPE::P_DOWN))return;
		if (checkBuff(Buff::BUFF_TYPE::P_DOWN, Buff::BUFF_TYPE::P_UP))return;
		if (checkBuff(Buff::BUFF_TYPE::D_UP, Buff::BUFF_TYPE::D_DOWN))return;
		if (checkBuff(Buff::BUFF_TYPE::D_DOWN, Buff::BUFF_TYPE::D_UP))return;
		if (checkBuff(Buff::BUFF_TYPE::S_UP, Buff::BUFF_TYPE::S_DOWN))return;
		if (checkBuff(Buff::BUFF_TYPE::S_DOWN, Buff::BUFF_TYPE::S_UP))return;
	}

	//バフの生成
	auto buff = GetValidBuff();
	buff->CreateBuff(type);

}

Buff* UnitBase::GetValidBuff(void)
{
	//使っていないバフがいたら、再利用
	size_t size = buffs_.size();
	for (int i = 0; i < size; i++)
	{
		if (buffs_[i]->IsAlive())continue;
		//利用できるバフを返す
		return buffs_[i];
	}

	//新しいバフを生成
	Buff* buff = new Buff();
	buffs_.push_back(buff);

	return buff;
}

DrawShader::PS_TYPE UnitBase::SelectBuffShader(const Buff::BUFF_TYPE& type)
{
	PsType buffPs;

	switch (type)
	{
	case Buff::BUFF_TYPE::P_UP: 
	case Buff::BUFF_TYPE::D_UP: 
	case Buff::BUFF_TYPE::S_UP: {
		buffPs = PsType::STATUS_UP;
		break;
	}
	case Buff::BUFF_TYPE::P_DOWN: 
	case Buff::BUFF_TYPE::D_DOWN: 
	case Buff::BUFF_TYPE::S_DOWN: {
		buffPs = PsType::STATUS_DOWN;
		break;
	}
	case Buff::BUFF_TYPE::PALALYSIS: {
		buffPs = PsType::PARALYSIS;
		break;
	}
	case Buff::BUFF_TYPE::POISON: {
		buffPs = PsType::POISON;
		break;
	}
	case Buff::BUFF_TYPE::AVOIDANCE: {
		buffPs = PsType::BAYER_DITHE;
		break;
	}
	default:
		break;
	}

	//シェーダータイプを返す
	return buffPs;
}

void UnitBase::ActivBuffEf(const Buff::BUFF_TYPE& type)
{
	isPlayBuffEf_ = true;
	buffEfTime_ = 0.0f;
	//シェーダーの選択
	nowPs_ = SelectBuffShader(type);
}


void UnitBase::SetDrawingPos(int x)
{
	auto topY = UNIT_SHOW_BASE_POS_Y;
	auto spanY = UNIT_SHOW_SPAN_POS_Y;

	if (unitNum_ == 1)pos_ = { x, topY  };				//中央
	else if (unitNum_ == 2)pos_ = { x,topY - spanY };	//上
	else if (unitNum_ == 3)pos_ = { x,topY + spanY };	//下
	
	return;
}
