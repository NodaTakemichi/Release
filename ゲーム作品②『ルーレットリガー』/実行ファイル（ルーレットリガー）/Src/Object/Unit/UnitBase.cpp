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
//�o�t�G�t�F�N�g��������
constexpr float BAUU_EFFECT_COMP_TIME = 1.5f;
//�ł̃_���[�W�p�[�Z���g
constexpr int POISON_DAMAGE_PARCENT = 8;
//���j�b�g���W
constexpr int ACTION_UNIT_SHOW_OFFSET_X = 70;	//�s�����j�b�g�̃I�t�Z�b�gX
constexpr int UNIT_SHOW_BASE_POS_Y = 320;	//�x�[�X���W�x
constexpr int UNIT_SHOW_SPAN_POS_Y = 180;	//�x���W�Ԋu
//�_���[�W�����l
constexpr int DAMAGE_RANDOM_VALUE = 90;	
//���j�b�g�̉摜�h��
constexpr float SHALE_VALUE = 50.0f;	//�U����
constexpr float SHAKE_MOVE_POW = 20.0f;	//�h��̈ړ���
constexpr float SHAKE_STOP_VALUE = 0.6f;	//�h���~�l
constexpr float SHAKE_DEC_VALUE = 0.8f;	//�h�ꌸ���l


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
	//���j�b�g�f�[�^�̓o�^
	LoadData();

	//������ԁA�s����ԁA���ݍs����Ԃ��Z�b�g����
	SetAlive(true);
	SetActed(false);
	SetAct(false);
	SetTargeted(false);


	//���݂̃s�N�Z���V�F�[�_�[(���]�\�V�F�[�_�[)
	nowPs_ = PsType::REVERSE_TEXTURE;;

	//���v����
	totalTime_ = 0.0f;

	//�h�ꕝ
	shakeX_ = 0.0f;
	shakeValue_ = 0.0f;
	movePow_ = 0.0f;

	//���炵���W
	shakePosX_ = 0;
}

void UnitBase::Draw(void)
{
	//�_���Ă��郆�j�b�g
	if (IsTargeted())unitUi_->DrawRockOn();
}

void UnitBase::Release(void)
{
	//���
	unitUi_->Release();

}

bool UnitBase::DecHpProcess(void)
{
	//HP�ω������鎞�A����HP���O�ȏ�̎��̂�
	if (changeHp_ != hp_ && changeHp_ >= 0)
	{
		auto changeTime = 1.0f;		//��������

		//�o�ߎ���
		totalTime_ += SceneManager::GetInstance().GetDeltaTime();
		//�o�߁@���i�������鎞�ԁ[�o�ߎ��ԁj/�������鎞��
		auto progress = 1.0f - (changeTime - totalTime_) / changeTime;

		//�_���[�W��ƃ_���[�W�O�̐��`��Ԃ��s��
		changeHp_ = Utility::Lerp(beforHp_, hp_, progress);

		//��_���̎��A�摜��h�炷
		if (hp_ < changeHp_)UnitImgShake(progress);


		//���߂��Ă���A�������͕ω���HP������HP�ɒǂ�������
		if (progress >= 1.0f || changeHp_ == hp_ || changeHp_ <= 0)
		{
			//�_���[�W�\�L�A�񕜒l�\�L���\������
			unitUi_->SetDmgValue(false, 0);
			unitUi_->SetHealValue(false, 0);
			//�o�ߎ��Ԃ̃��Z�b�g
			totalTime_ = 0.0f;

			if (!IsAlive()) {
				changeHp_ = 0;
				hp_ = 0;

				//���S���o�̔���
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

			//HP�����F�I��
			return true;
		}
		//HP�����F���I��
		return false;
	}

	//HP�ϓ��F����
	return true;
}

void UnitBase::TurnEndProcess(void)
{
	for (auto& buff : buffs_)
	{
		if (!buff->IsAlive())continue;

		//�Ń_���[�W�̏���
		if (buff->CheckOwnBuff(Buff::BUFF_TYPE::POISON))
		{
			TRACE("�Ń_���[�W\n");

			//�ő�HP�� 1/8 �̃_���[�W
			int dmg = maxHp_ / POISON_DAMAGE_PARCENT;
			Damage(dmg);
		}

		//�o�t�̃^�[������������
		if (buff->DecBuffTurn())
		{
			//�����^�[�����I��������A�o�t�𖳌���Ԃɂ���
			buff->SetAlive(false);
		}
	}
}

const int& UnitBase::GetSpeed(void)
{
	//�o�t���݂̃X�s�[�h�v�Z
	int nowSpeed = CalcBuffStatus(
		speed_, Buff::BUFF_TYPE::S_UP, Buff::BUFF_TYPE::S_DOWN);

	return nowSpeed;
}

const int& UnitBase::GetAttack(void)
{
	//�o�t���݂̍U���͌v�Z
	int nowAttack = CalcBuffStatus(
		attack_, Buff::BUFF_TYPE::P_UP, Buff::BUFF_TYPE::P_DOWN);

	return nowAttack;
}

int UnitBase::CalcBuffStatus(const int& status, 
	const Buff::BUFF_TYPE& up, const Buff::BUFF_TYPE& down)
{
	float value = static_cast<float>(status);

	//�o�t�̌v�Z
	for (auto buff : buffs_)
	{
		if (!buff->IsAlive())continue;

		//�X�e�[�^�X�A�b�v
		if (buff->CheckOwnBuff(up))value *= 1.2;
		//�X�e�[�^�X�_�E��
		if (buff->CheckOwnBuff(down))value *= 0.8;
	}

	return static_cast<int>(floor(value));
}

void UnitBase::SetAct(bool act)
{
	//�s�����j�b�g�͏���X���W��O�ɉ����o��
	shakePosX_ = act ? ACTION_UNIT_SHOW_OFFSET_X : 0;
	//���炵�l
	shakePosX_ *= UNIT_TYPE::PLAYER == type_ ? 1 : -1;
	//�s�����
	isAct_ = act;
}

void UnitBase::Damage(const int& dmg)
{
	TRACE(name_.c_str());
	TRACE("\n�U���l�F%d ||", dmg);

	//�o�t���݂̃_���[�W�v�Z
	auto calcDmg = CalcBuffStatus(
		dmg, Buff::BUFF_TYPE::D_DOWN, Buff::BUFF_TYPE::D_UP);

	//�_���[�W�̗����v�Z
	float randNum = static_cast<float>(
		SceneManager::GetInstance().GetRand(DAMAGE_RANDOM_VALUE, 100)) / 100.0f;
	calcDmg = Utility::Round(static_cast<float>(calcDmg) * randNum);
	TRACE("��_���l�F%d\n", calcDmg);

	//���OHP�̕ێ�
	beforHp_ = hp_;
	//�_���[�W�v�Z
	hp_ -= calcDmg;
	//�c��̗�
	TRACE("�c��̗�:%d\n\n",hp_);

	//�_���[�W�\�L��\������
	unitUi_->SetDmgValue(true, calcDmg);
	//�摜�̗h�ꕝ�̌���
	shakeValue_ = SHALE_VALUE;
	movePow_ = SHAKE_MOVE_POW;

	//���S����
	if (CheckDead())
	{
		TRACE("���S���܂���\n");
		//�V�F�[�_�[�̕ύX(���m�g�[��)
		nowPs_ = PsType::MONOTONE;

		//���S��Ԃɂ���
		SetAlive(false);
	}
}

void UnitBase::Heal(const int& heal)
{
	//���OHP�̋L��
	beforHp_ = hp_;
	//HP�v�Z
	hp_ += heal;

	//�񕜒l�\�L��\������
	if (hp_ - maxHp_ < heal)unitUi_->SetHealValue(true, heal);
	
	//HP�̏���v�Z
	hp_ = (std::min)(hp_, maxHp_);
}

void UnitBase::GiveBuff(const Buff::BUFF_TYPE& type)
{
	//�o�t�̐���
	CreateBuff(type);

	//UI�Ƀo�t�̓o�^
	unitUi_->SetBuff(buffs_);

	//�o�t�G�t�F�N�g�̔���
	ActivBuffEf(type);
}

bool UnitBase::CheckDead(void)
{
	//���S����
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

		//�w��o�t�����L���Ă���
		if (buff->CheckOwnBuff(type))
		{
			return true;
		}
	}
	return false;
}

bool UnitBase::PlayBuffEffect(void)
{
	//�o�t�G�t�F�N�g�̍Đ��I�����f
	float wait = DX_PI_F / BAUU_EFFECT_COMP_TIME;
	if (Utility::OverTime(buffEfTime_, wait))
	{
		isPlayBuffEf_ = false;

		//���j�b�g�V�F�[�_�[��߂�
		nowPs_ = PsType::REVERSE_TEXTURE;

		return true;
	}
	return false;
}

void UnitBase::LoadData(void)
{
	//���j�b�g�f�[�^�[�̎擾
	auto& data = UnitDataManager::GetInstance().GetUnitData(unitDataNum_);

	//�p�����[�^�Z�b�g
	name_ = data.name;
	unitImg_ = data.imgHandle;
	hp_ = data.hp;
	attack_ = data.attack;
	speed_ = data.speed;
	//�R�}���h�Z�b�g
	for (auto& cmd : data.cmdNum)
	{
		//�R�}���h�̍쐬
		CreateCommand(cmd);
	}

	//HP�֘A�̏�����
	maxHp_ = beforHp_= changeHp_ = hp_;

}

void UnitBase::DrawUnitShader(const float& revers)
{
	auto time = SceneManager::GetInstance().GetTotalTime();
	auto& ds = DrawShader::GetInstance();

	//���ݍ��W�@�{�@�h�ꕝ�@�{�@���炵�l
	Vector2 pos = { pos_.x + static_cast<int>(shakeX_) + shakePosX_,pos_.y };
	//�萔�o�b�t�@
	COLOR_F buf = {
		revers,					//���]���邩�ǂ���
		buffEfTime_,			//�o�ߎ���
		BAUU_EFFECT_COMP_TIME	//�o�t�G�t�F�N�g��������
	};

	//�`��
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

	//�摜���W�̐U���l���Z
	shakeX_ += movePow_;

	// �l���ő�l�܂��͍ŏ��l�𒴂����ꍇ�A���]������
	if (shakeValue_ <= shakeX_ || (shakeValue_ * -1) >= shakeX_)
	{
		//���]
		movePow_ *= -SHAKE_DEC_VALUE;

		//�U���l������������
		shakeValue_ *= (1.0f - leap);

		auto i = 1;
		if ((shakeValue_ * -1) >= shakeX_)i = -1;
		shakeX_=shakeValue_* i + movePow_;
	}
}

void UnitBase::CreateCommand(const int& num)
{
	//�R�}���h�̐���
	auto cmd = new Command(num);
	cmd->Init();
	commands_.push_back(cmd);
}

void UnitBase::CreateBuff(const Buff::BUFF_TYPE& type)
{
	for (auto& buff : buffs_)
	{
		//����ł�����A���������Ȃ�
		if (!buff->IsAlive())continue;

		//�d�����Ă����ꍇ�A�ǉ����Ȃ�
		if (buff->GetBuff() == type)return;

		//�����������o�t�̏ꍇ�A�Ώ��ł�����
		auto checkBuff = [&](Buff::BUFF_TYPE get, Buff::BUFF_TYPE give) {
			if (buff->GetBuff() == get)
			{
				if (type == give)
				{
					//����
					buff->SetAlive(false);

					//���������ɁA�������I��
					return true;
				}
			}
			return false;
		};

		//�����������o�t
		if (checkBuff(Buff::BUFF_TYPE::P_UP, Buff::BUFF_TYPE::P_DOWN))return;
		if (checkBuff(Buff::BUFF_TYPE::P_DOWN, Buff::BUFF_TYPE::P_UP))return;
		if (checkBuff(Buff::BUFF_TYPE::D_UP, Buff::BUFF_TYPE::D_DOWN))return;
		if (checkBuff(Buff::BUFF_TYPE::D_DOWN, Buff::BUFF_TYPE::D_UP))return;
		if (checkBuff(Buff::BUFF_TYPE::S_UP, Buff::BUFF_TYPE::S_DOWN))return;
		if (checkBuff(Buff::BUFF_TYPE::S_DOWN, Buff::BUFF_TYPE::S_UP))return;
	}

	//�o�t�̐���
	auto buff = GetValidBuff();
	buff->CreateBuff(type);

}

Buff* UnitBase::GetValidBuff(void)
{
	//�g���Ă��Ȃ��o�t��������A�ė��p
	size_t size = buffs_.size();
	for (int i = 0; i < size; i++)
	{
		if (buffs_[i]->IsAlive())continue;
		//���p�ł���o�t��Ԃ�
		return buffs_[i];
	}

	//�V�����o�t�𐶐�
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

	//�V�F�[�_�[�^�C�v��Ԃ�
	return buffPs;
}

void UnitBase::ActivBuffEf(const Buff::BUFF_TYPE& type)
{
	isPlayBuffEf_ = true;
	buffEfTime_ = 0.0f;
	//�V�F�[�_�[�̑I��
	nowPs_ = SelectBuffShader(type);
}


void UnitBase::SetDrawingPos(int x)
{
	auto topY = UNIT_SHOW_BASE_POS_Y;
	auto spanY = UNIT_SHOW_SPAN_POS_Y;

	if (unitNum_ == 1)pos_ = { x, topY  };				//����
	else if (unitNum_ == 2)pos_ = { x,topY - spanY };	//��
	else if (unitNum_ == 3)pos_ = { x,topY + spanY };	//��
	
	return;
}
