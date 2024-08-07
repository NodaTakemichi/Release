#include <algorithm>
#include "../../Manager/ResourceManager.h"
#include "../../Manager/Resource.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/SoundManager.h"
#include "../../Manager/Camera.h"
#include "../../Utility/Utility.h"
#include "../../Common/Quaternion.h"
#include "../../Sound/SoundController.h"
#include "../../_debug/_DebugConOut.h"
#include "../../_debug/_DebugDispOut.h"
#include "../Common/Transform.h"
#include "../Common/Capsule.h"

#include "../Item/ItemBase.h"
#include "../Stage/Stage.h"
#include "../Stage/GoalGate.h"
#include "Player.h"

// �v���C���[�̔��a
constexpr float RADIOUS = 40.0f;
// �v���C���[�̖ڂ̍���
constexpr float EYE_LEVEL = 170.0f;
//�Q�[���I�[�o�[���̖ڐ��̍���
constexpr float GAME_OVER_EYE_LEVEL = 190.0f;
// �v���C���[�̃J�v�Z���R���C�_�[���W
constexpr VECTOR CAPSULE_LOCAL_POS_DOWN = { 0, RADIOUS,0 };  //��
constexpr VECTOR CAPSULE_LOCAL_POS_UP = { 0,EYE_LEVEL,0 };   //��

// �I�u�W�F�N�g�Փ˗pray���W����
constexpr float RAY_LOCAL_DIS_FRONT = 10.0f; //��O
constexpr float RAY_LOCAL_DIS_BACK = 200.0f; //��
// �ړ��X�s�[�h
constexpr float SPEED_MOVE = 4.5f;   //����
constexpr float SPEED_RUN = 7.0f;    //����
// �g�F�͈�
constexpr float BURN_RADIOUS = 50.0f;

//�v���C���[���L���W
constexpr VECTOR PLAYER_POS = { 5.0f, 330.0f, -2530.0f };

//�X�|�b�g���C�g
constexpr float LIGHT_OUT_ANGLE_DEG = 55.0f;	//�O���̊p�x
constexpr float LIGHT_IN_ANGLE_DEG = 35.0f;		//�����̊p�x
constexpr float LIGHT_RANGE = 2000.0f;			//�L������
constexpr float LIGHT_ATTEN_0 = 0.28f;			//���������p�����[�^�O
constexpr float LIGHT_ATTEN_1 = 0.002f;		    //���������p�����[�^�P
constexpr float LIGHT_ATTEN_2 = 0.0f;			//���������p�����[�^�Q

// ���������̍Đ����x�iRUN�j
constexpr int FOOTSTEP_SOUND_SPEE_RUN = 75000;
//����
constexpr int FOOTSTEPS_SE_VOLUME = 200;		        //����
constexpr int FOOTSTEPS_SE_HEARING_AREA = 300;	        //�����͂�����
//�A�C�e���擾��
constexpr int GET_ITEM_SE_VOLUME = 175;		            //����
constexpr int GET_ITEM_SE_HEARING_AREA = 300; 	        //�����͂�����
//�아�g�pSE
constexpr int USE_AMULET_SE_VOLUME = 200;		        //����
constexpr int USE_AMULET_SE_HEARING_AREA = 300;	        //�����͂�����
//�ʂ�����ݏċpSE 
constexpr int BURN_DOOL_SE_VOLUME = 225;		        //����
constexpr int BURN_DOOL_SE_HEARING_AREA = 300;	        //�����͂�����

Player::Player(void)
{
	//�X�V�֐��̃Z�b�g
	updateFunc_ = std::bind(& Player::UpdateSearch,this);
}

Player::~Player(void)
{
}


void Player::LoadData(void)
{

	transform_.SetModel(-1);

	//�����T�E���h�̍쐬
	footstepsSound_ = std::make_shared<SoundController>();
	footstepsSound_->DataLoad(ResourceManager::SRC::FOOTSTEPS_PLAYER_SND);
	//�擾�T�E���h�̍쐬
	getItemSound_ = std::make_shared<SoundController>();
	getItemSound_->DataLoad(ResourceManager::SRC::GET_ITEM_SND);
	//�아�g�p�T�E���h�̍쐬
	useAmuletSound_ = std::make_shared<SoundController>();
	useAmuletSound_->DataLoad(ResourceManager::SRC::USE_AMULET_SND);
	//�아�ċp�T�E���h�̍쐬
	burnAmuletSound_ = std::make_shared<SoundController>();
	burnAmuletSound_->DataLoad(ResourceManager::SRC::BURN_AMULET_SND);
}

void Player::Init(void)
{
	//3D��{���̏�����
	transform_.Init(
		Utility::VECTOR_ONE,
		PLAYER_POS,
		Quaternion(),
		Quaternion(),
		Collider::TYPE::NONE
	);

	//�J�v�Z���R���C�_�[
	capsule_ = std::make_unique<Capsule>(transform_);
	capsule_->SetRadius(RADIOUS);
	capsule_->SetLocalPosDown(CAPSULE_LOCAL_POS_DOWN);
	capsule_->SetLocalPosTop(CAPSULE_LOCAL_POS_UP);

	//�Փ˃R���C�_�[�̃N���A
	ClearStageCollider();
	ClearItemInfos();

	//�e�L�X�g
	SetActionText(L"");
	SetCommentText(L"");

	//�아�̃N�[���^�C��
	amuletCoolTime_ = AMULET_USE_COOL_TIME;

	//�X�|�b�g���C�g�̍쐬
	auto& camera = scnMng_.GetCamera();
	lightHandle_ = CreateSpotLightHandle(
		camera.GetPos(), camera.GetForward(),
		Utility::Deg2RadF(LIGHT_OUT_ANGLE_DEG), Utility::Deg2RadF(LIGHT_IN_ANGLE_DEG),
		LIGHT_RANGE,
		LIGHT_ATTEN_0, LIGHT_ATTEN_1, LIGHT_ATTEN_2);

	//�E�o����
	isEscape_ = false;

	//�T�E���h�̐ݒ�
	//������
	footstepsSound_->Init(FOOTSTEPS_SE_VOLUME, FOOTSTEPS_SE_HEARING_AREA,
		transform_.pos_, SoundController::REVERB_TYPE::PADDEDCELL);
	//�A�C�e���擾��
	getItemSound_->Init(
		GET_ITEM_SE_VOLUME, GET_ITEM_SE_HEARING_AREA,
		transform_.pos_, SoundController::REVERB_TYPE::PADDEDCELL);
	//�아�g�pSE
	useAmuletSound_->Init(
		USE_AMULET_SE_VOLUME, USE_AMULET_SE_HEARING_AREA,
		transform_.pos_, SoundController::REVERB_TYPE::PADDEDCELL);
	//�아�ċpSE
	burnAmuletSound_->Init(
		BURN_DOOL_SE_VOLUME, BURN_DOOL_SE_HEARING_AREA,
		transform_.pos_, SoundController::REVERB_TYPE::PADDEDCELL);

}

void Player::Update(void)
{
	//���C�g�X�V
	auto& camera = scnMng_.GetCamera();
	SetLightDirectionHandle(lightHandle_, camera.GetForward());
	SetLightPositionHandle(lightHandle_, camera.GetPos());

	//�e�L�X�g���펞null�ɂ���
	SetActionText(L"");
	SetCommentText(L"");

	//�T�E���h�̍��W�X�V
	footstepsSound_->SetSoundPos(transform_.pos_);
	useAmuletSound_->SetSoundPos(transform_.pos_);
	burnAmuletSound_->SetSoundPos(transform_.pos_);


	//���݂̍X�V�֐��̌Ăяo��
	updateFunc_();
	//3D���̍X�V
	transform_.Update();
	return;
}

void Player::Draw(void)
{
#ifdef _DEBUG
	//�f�o�b�N�p
	//capsule_->Draw();

	//�v���C���[���W���S
	DrawSphere3D(transform_.pos_, 3, 5, 0xff0000, 0xff0000, true);

	//�v���C���[���W
	DrawFormatString(0, 30, 0xffffff,
		L"�v���C���[���W�F(%0.2f, %0.2f, %0.2f)", 
		transform_.pos_.x, transform_.pos_.y, transform_.pos_.z);

	//�n�ʔ���p���C��
	DrawLine3D(gravHitPosUp_, gravHitPosDown_, 0xffff00);
#endif // _DEBUG
}

const VECTOR Player::GetDirection(void) const
{
	return VNorm(scnMng_.GetCamera().GetAngles());
}

void Player::AddItemInfo(const std::weak_ptr<ItemBase> coll)
{
	itemInfos_.push_back(coll);
}

void Player::ClearItemInfos(void)
{
	itemInfos_.clear();
}

void Player::SetGameOverInfo(const VECTOR& opponentPos)
{
	//�G�ɕ߂܂������A�G�����փJ��������]������
	VECTOR rotateAngle = opponentPos;
	rotateAngle.y += GAME_OVER_EYE_LEVEL;			//�ڐ��i�J���������j���グ��
	scnMng_.GetCamera().SetGameOverRot(rotateAngle);
	//�J�������[�h�̃`�F���W
	scnMng_.GetCamera().ChangeMode(Camera::MODE::GAME_OVER_AUTO);

	//�Q�[���I�[�o�[��ԂɕύX
	updateFunc_ = std::bind(&Player::UpdateGameOver, this);
}

void Player::ProcessMove(void)
{
	//����
	auto& ins = InputManager::GetInstance();
	
	// X����]���������A�d�͕����ɐ����ȃJ�����p�x(XZ����)���擾
	Quaternion cameraRot = 
		SceneManager::GetInstance().GetCamera().GetQuaRotOutX();

	//�ړ�����
	VECTOR dir = Utility::VECTOR_ZERO;

	//�ړ�����
	//�O����
	if (ins.IsNew(KEY_INPUT_W))				
	{
		dir = VAdd(dir,cameraRot.GetForward());
	}
	//�����
	if (ins.IsNew(KEY_INPUT_S))		
	{
		dir = VAdd(dir, cameraRot.GetBack());
	}
	//������
	if (ins.IsNew(KEY_INPUT_A))		
	{
		dir = VAdd(dir, cameraRot.GetLeft());
	}
	//�E����
	if (ins.IsNew(KEY_INPUT_D))		
	{
		dir = VAdd(dir, cameraRot.GetRight());
	}

	if (!Utility::EqualsVZero(dir)) {
		//�ړ������̐��K��
		dir = VNorm(dir);

		// �ړ�����
		speed_ = SPEED_MOVE;
		if (ins.IsNew(KEY_INPUT_LSHIFT))
		{
			speed_ = SPEED_RUN;
			//���������̍Đ����x�ύX
			footstepsSound_->SetSoundFrequency(FOOTSTEP_SOUND_SPEE_RUN);

		}
		moveDir_ = dir;
		movePow_ = VScale(moveDir_, speed_);

		//�����T�E���h���Đ����Ă��Ȃ�΍Đ�
		footstepsSound_->PlayNonPlayingLoop();
	}
	else 
	{
		//�����T�E���h�̒�~
		if (footstepsSound_->CheckPlaySound())
		{
			footstepsSound_->Stop();
		}
	}
}

void Player::ProcessObtainItem(void)
{
	//�A�C�e���Ƃ�Ray�Փ˔���
	CollisionItem();
}

void Player::ProcessUseItem(void)
{
	//�아�̎g�p
	//��Ɍ아�͖��g�p��ԁifalse�j�ɂ���
	SetUseAmulet(false);

	//�N�[���^�C��
	if (!Utility::TimeOver(amuletCoolTime_, AMULET_USE_COOL_TIME))return;

	//����
	auto& ins = InputManager::GetInstance();
	//�{�^������������
	if (ins.IsClickMouseRight())
	{
		//�아���P���ȏ㏊�����Ă��鎞
		if (haveAmuletNum_ >= 1)
		{
			//�@�아�̖��������炷
			haveAmuletNum_--;

			//�A�아�̎g�p���f
			SetUseAmulet(true);

			//�B�N�[���^�C���̐ݒ�
			amuletCoolTime_ = 0.0f;

			//�CSE�Đ�
			useAmuletSound_->Play();
			burnAmuletSound_->Play();
		}
		else
		{
			SetCommentText(L"�아�������ĂȂ�");
		}
	}
}

void Player::CollisionItem(void)
{
	//���͏��
	auto& ins = InputManager::GetInstance();
	// �J����
	const auto& camera = scnMng_.GetCamera();

	//ray�̍��W
	objHitRayPosFront_ = camera.GetPos();					//�J�������W
	objHitRayPosBack_ = VAdd(objHitRayPosFront_, 
		VScale(camera.GetForward(), RAY_LOCAL_DIS_BACK));	//�J����������

	for (const auto& item : itemInfos_)
	{
		if(item.lock()->GetHasItem())continue;

		// �A�C�e���Ƃ̏Փ�
		auto hit = MV1CollCheck_Line(
			item.lock()->GetTransform().modelId_, -1, objHitRayPosFront_, objHitRayPosBack_);

		//�Փ˂��Ă��Ȃ��ꍇ
		if (hit.HitFlag <= 0)continue;

		//�@�A�C�e���ƏՓ˂��Ă���
		SetActionText(L"���N���b�N�F��Ɏ��");

		//�A�Փ˂��Ă���Ƃ��ɁA�{�^���������Ɓh����h����
		if (ins.IsTrgMouseLeft())
		{
			//�ϐ��ɃA�C�e���̊l���������適��ޕʂɕʏ������s��
			switch (item.lock()->GetItemType()) {
			case ItemBase::ITEM_TYPE::AMULET:
				//�아�̖��������Z
				TRACE("�아�̎擾\n");
				haveAmuletNum_++;
				break;
			default:
				//�l�`�̊l��
				if (!haveTheDoll_)
				{
					TRACE("�ʂ�����݂̎擾\n");
					haveTheDoll_ = true;
					haveDoll_ = item;
				}
				else
				{
					SetCommentText(L"��������Ă����ɂȂ�");
					continue;
				}
				break;
			}

			//�A�C�e�����擾��Ԃɂ���
			item.lock()->SetHasItem(true);

			//�A�C�e���擾���Đ�
			getItemSound_->SetSoundPos(transform_.pos_);
			getItemSound_->Play();
		}
	}
}

void Player::CollisionGoalWall(void)
{
	//�������W�Ƙc�ݕǂ̏Փ˔���
	if (CollCheckLine_Stage(objHitRayPosFront_, objHitRayPosBack_, Collider::TYPE::DIS_WALL))
	{
		SetActionText(L"���ׂ�");

		//���N���b�N�����������
		auto& input = InputManager::GetInstance();
		if (input.IsTrgMouseLeft())
		{
			//�ʂ�����݂̑S�Ŕ���
			if (CheckDeadAllDoll())
			{
				//�c�ݕǂ̃f�B�]���u�J�n
				goalGate_.lock()->SetOpenGate();

				//�c�ݕǍ폜
				std::vector<std::shared_ptr<Collider>>stageColls;
				for (const auto& coll : stageColliders_)
				{
					if (coll->type_ != Collider::TYPE::DIS_WALL)
					{
						stageColls.push_back(coll);
					}
				}
				stageColliders_ = stageColls;
			}
			else
			{
				SetCommentText(L"�܂��ʂ�Ȃ�������");
			}
		}
	}
}

void Player::CollisionGoalDoor(void)
{
	//�������W�Ƙc�ݕǂ̏Փ˔���
	if (CollCheckLine_Stage(objHitRayPosFront_, objHitRayPosBack_, Collider::TYPE::GOAL_DOOR))
	{
		SetActionText(L"�E�o����");

		//���N���b�N�����������
		auto& input = InputManager::GetInstance();
		if (input.IsTrgMouseLeft())
		{
			//�E�o����
			isEscape_ = true;
		}
	}
}

bool Player::CheckBurnTheDoll(void)
{
	//�@Ray�̉����W�ƒg�F���W�̍����������͈͓�
	auto diff = Utility::Distance(Stage::FIREPLACE_HIT_POS, objHitRayPosBack_);
	if (diff <= BURN_RADIOUS)
	{
		if (haveTheDoll_)SetActionText(L"�ʂ�����݂�R�₷");
		else SetActionText(L"���ׂ�");

		//�A���N���b�N�����������
		auto& input = InputManager::GetInstance();
		if (input.IsTrgMouseLeft())
		{
			//�B�ʂ�����݂��������Ă���
			if (haveTheDoll_)
			{
				//���������̒�~
				footstepsSound_->Stop();

				TRACE("�ʂ�����݂̏ċp�����̊J�n\n");

				//�ʂ�����݂̏Ď����o�̊J�n
				haveDoll_.lock()->StartBurnDirection();

				//�ʂ�����݂𖢏�����Ԃɂ���
				haveTheDoll_ = false;

				//�ċp�����̊J�n
				return true;
			}
			else
			{
				TRACE("�ʂ�����݂��������Ă��܂���\n");
				SetCommentText(L"�����R�₹������");
			}
		}
	}

	return false;
}

void Player::SetActionText(std::wstring text)
{
	if (text == L"")actionText_.clear();
	actionText_ = text; 
}

void Player::SetCommentText(std::wstring text)
{
	if (text == L"")commentText_.clear();
	commentText_ = text;
}

bool Player::CheckDeadAllDoll(void)
{
	for (const auto& item : itemInfos_)
	{
		if (item.lock()->GetItemType() == ItemBase::ITEM_TYPE::AMULET)continue;
		if (item.lock()->IsAlive())
		{
			return false;
		}
	}

	//�S��
	return true;
}

void Player::UpdateSearch(void)
{
	beforePos_ = transform_.pos_;		// �ړ��O���W
	movePow_ = Utility::VECTOR_ZERO;	// �ړ��ʂ��[��
	ProcessMove();						// �ړ�����
	CalcGravityPow();					// �d�͂ɂ��ړ���
	Collision();						// �Փ˔���

	ProcessObtainItem();				//�A�C�e���̓��葀��
	CollisionGoalWall();				//�S�[���ǂƂ̏Փˑ���
	CollisionGoalDoor();				//�S�[���Ƃ̏Փˑ���
	ProcessUseItem();					//�A�C�e���̎g�p
	transform_.quaRot_ = actorRotY_;	// ��]������


	//�ʂ�����݂̏ċp���f
	if (CheckBurnTheDoll())
	{
		//�ċp���ɂ���
		updateFunc_ = std::bind(&Player::UpdateBurning, this);
		return;
	}
}

void Player::UpdateBurning(void)
{	
	movePow_ = Utility::VECTOR_ZERO;	// �ړ��ʂ��[��
	CalcGravityPow();					// �d�͂ɂ��ړ���
	Collision();						// �Փ˔���
	ProcessUseItem();					// �A�C�e���̎g�p
	transform_.quaRot_ = actorRotY_;	// ��]������


	//�ċp���Ԃ̏I��
	if (Utility::TimeOver(burnTime_, BURN_END_TIME))
	{
		//�S�Ă�����
		if (CheckDeadAllDoll())
		{
			SetCommentText(L"�ʂ�����݂�S�ĔR�₵���悤��");
		}


		//�ʂ�����݂𖢏�����Ԃɂ���
		haveTheDoll_ = false;
		//�ċp���Ԃ̃��Z�b�g
		burnTime_ = 0.0f;

		//��ԑJ��
		updateFunc_ = std::bind(&Player::UpdateSearch, this);
		return;
	}
}

void Player::UpdateGameOver(void)
{
	//��]�̋��ʕ⊮
	rotateTime_ += SceneManager::GetInstance().GetDeltaTime();
	float par = rotateTime_ / 0.075f;
	actorRotY_ = Quaternion::Slerp(actorRotY_, goalQuaRot_, par);
	transform_.quaRot_ = actorRotY_;	// ��]������

}
