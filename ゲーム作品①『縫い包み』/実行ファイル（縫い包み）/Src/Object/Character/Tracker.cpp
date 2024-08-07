#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/SoundManager.h"
#include "../../Utility/Utility.h"
#include "../Common/AnimationController.h"
#include "../Common/Capsule.h"
#include "../../Sound/SoundController.h"
#include "Player.h"
#include "Tracker.h"

// �ǐՎ҂̏Փ˔���̔��a
constexpr float RADIOUS = 50.0f;
// �ǐՎ҂̖ڂ̍���
constexpr float EYE_LEVEL = 160.0f;
// �ǐՎ҂̃J�v�Z���R���C�_�[���W�i���j
constexpr VECTOR CAPSULE_LOCAL_POS_DOWN = { 0, RADIOUS,0 }; //��
constexpr VECTOR CAPSULE_LOCAL_POS_UP = { 0,EYE_LEVEL,0 };  //��
// �ړ��X�s�[�h
constexpr float SPEED_WALK = 4.35f; //����
constexpr float SPEED_RUN = 9.0f;   //����
//�����]���X�s�[�h
constexpr float TROCKER_CHANGE_DIR_SPEED = 0.05f;

//����
constexpr float VIEW_RANGE = 1000.0f;   //����̋���
constexpr float VIEW_ANGLE = 50.0f;     //����p
constexpr float VIEW_ANGLE_RAD = (VIEW_ANGLE * (DX_PI_F / 180.0f));  //����p

//�v���C���[������������
constexpr float DIS_LOSE_SIGHT_OF_PLAYER = 900.0f;

//�����]����������
constexpr float ROTATION_COMPLETED_TIME = 2.5f;
//�C�⎞��
constexpr float FAINT_TIME = 3.0f;

//�A�j���[�V�����Đ����x
constexpr float ANIM_PLAY_SPEED_BASE = 30.0f;   //��{
constexpr float ANIM_PLAY_SPEED_WALK = 33.0f;    //����
constexpr float ANIM_PLAY_SPEED_CHACH = 14.0f;  //�߂܂���

//�ǐՎ�
constexpr VECTOR TRACKER_SCL = { 1.5f,1.5f,1.5f };           //�傫��
constexpr VECTOR TRACKER_POS = { -870.0f, 360.0f, 900.0f };  //���W
constexpr VECTOR TRACKER_LOCAL_ROT = { 0.0f,180.0f * (DX_PI_F / 180.0f),0.0f };  //���[�J���p�x

//����
constexpr int FOOTSTEPS_SE_VOLUME = 200;		//����
constexpr float FOOTSTEPS_SE_HEARING_AREA = 1000.0f;	//�����͂�����

//�ړ��|�C���g�̏Փ˔���̔��a
constexpr float MOVE_POINT_R = 20.0f;
//�ړ��|�C���g���W(�ǐՎ҂��p�j����Ƃ��Ɏg�����W)
#pragma region �ړ��|�C���g���W
constexpr float FRONT_MOVE_POINT_Y = 330.0f;	    //��K��Y���W
constexpr float BASEMENT_MOVE_POINT_Y = -155.0f;	//�n����Y���W
//��K
constexpr VECTOR MOVE_POINT_POS_1 = { 5.0f,FRONT_MOVE_POINT_Y,-710.0f };
constexpr VECTOR MOVE_POINT_POS_2 = { 5.0f,FRONT_MOVE_POINT_Y,-1700.0f };
constexpr VECTOR MOVE_POINT_POS_3 = { 5.0f,FRONT_MOVE_POINT_Y,-2570.0f };
//�啔��
constexpr VECTOR MOVE_POINT_POS_4 = { -590.0f,FRONT_MOVE_POINT_Y,-300.0f };
constexpr VECTOR MOVE_POINT_POS_5 = { -590.0f,FRONT_MOVE_POINT_Y,-200.0f };
constexpr VECTOR MOVE_POINT_POS_6 = { -590.0f,FRONT_MOVE_POINT_Y,150.0f };
constexpr VECTOR MOVE_POINT_POS_7 = { 550.0f,FRONT_MOVE_POINT_Y,-300.0f };
constexpr VECTOR MOVE_POINT_POS_8 = { 550.0f,FRONT_MOVE_POINT_Y,5.0f };
constexpr VECTOR MOVE_POINT_POS_9 = { 550.0f,FRONT_MOVE_POINT_Y,150.0f };
//���L��
constexpr VECTOR MOVE_POINT_POS_10 = { -910.0f,FRONT_MOVE_POINT_Y,1600.0f };
constexpr VECTOR MOVE_POINT_POS_11 = { -910.0f,FRONT_MOVE_POINT_Y,900.0f };
constexpr VECTOR MOVE_POINT_POS_12 = { -910.0f,FRONT_MOVE_POINT_Y,10.0f };
constexpr VECTOR MOVE_POINT_POS_13 = { -910.0f,FRONT_MOVE_POINT_Y,-200.0f };
constexpr VECTOR MOVE_POINT_POS_14 = { -910.0f,FRONT_MOVE_POINT_Y,-710.0f };
constexpr VECTOR MOVE_POINT_POS_15 = { -910.0f,FRONT_MOVE_POINT_Y,-1200.0f };
constexpr VECTOR MOVE_POINT_POS_16 = { -910.0f,FRONT_MOVE_POINT_Y,-1700.0f };
//�E�L��
constexpr VECTOR MOVE_POINT_POS_17 = { 910.0f,FRONT_MOVE_POINT_Y,1600.0f };
constexpr VECTOR MOVE_POINT_POS_18 = { 910.0f,FRONT_MOVE_POINT_Y,900.0f };
constexpr VECTOR MOVE_POINT_POS_19 = { 910.0f,FRONT_MOVE_POINT_Y,5.0f };
constexpr VECTOR MOVE_POINT_POS_20 = { 910.0f,FRONT_MOVE_POINT_Y,-200.0f };
constexpr VECTOR MOVE_POINT_POS_21 = { 910.0f,FRONT_MOVE_POINT_Y,-710.0f };
constexpr VECTOR MOVE_POINT_POS_22 = { 910.0f,FRONT_MOVE_POINT_Y,-1700.0f };
//������
constexpr VECTOR MOVE_POINT_POS_23 = { -500.0f,FRONT_MOVE_POINT_Y,-1200.0f };		//��O�̏�����
constexpr VECTOR MOVE_POINT_POS_24 = { -500.0f,FRONT_MOVE_POINT_Y,1600.0f };		//���̏������i�E�j

//�n��
constexpr VECTOR MOVE_POINT_POS_25 = { -40.0f,BASEMENT_MOVE_POINT_Y,1000.0f };
constexpr VECTOR MOVE_POINT_POS_26 = { 270.0f,BASEMENT_MOVE_POINT_Y,1000.0f };

constexpr VECTOR MOVE_POINT_POS_27 = { -40.0f,BASEMENT_MOVE_POINT_Y,200.0f };
constexpr VECTOR MOVE_POINT_POS_28 = { 270.0f,BASEMENT_MOVE_POINT_Y,200.0f };
constexpr VECTOR MOVE_POINT_POS_29 = { -300.0f,BASEMENT_MOVE_POINT_Y,30.0f };
constexpr VECTOR MOVE_POINT_POS_30 = { 270.0f,BASEMENT_MOVE_POINT_Y,-200.0f };

constexpr VECTOR MOVE_POINT_POS_31 = { -40.0f,BASEMENT_MOVE_POINT_Y,-700.0f };
constexpr VECTOR MOVE_POINT_POS_32 = { -245.0f,BASEMENT_MOVE_POINT_Y,-700.0f };

//�K�i�F��
constexpr VECTOR MOVE_POINT_POS_33 = { -1300.0f,BASEMENT_MOVE_POINT_Y,30.0f };
constexpr VECTOR MOVE_POINT_POS_34 = { -1300.0f,175.0f,-690.0f };
constexpr VECTOR MOVE_POINT_POS_35 = { -1500.0f,155.0f,-690.0f };
constexpr VECTOR MOVE_POINT_POS_36 = { -1500.0f,FRONT_MOVE_POINT_Y + 20.0f,10.0f };
//�K�i�F�E
constexpr VECTOR MOVE_POINT_POS_37 = { 1300.0f,BASEMENT_MOVE_POINT_Y,-200.0f };
constexpr VECTOR MOVE_POINT_POS_38 = { 1300.0f,175.0f,500.0f };
constexpr VECTOR MOVE_POINT_POS_39 = { 1500.0f,155.0f,500.0f };
constexpr VECTOR MOVE_POINT_POS_40 = { 1500.0f,FRONT_MOVE_POINT_Y + 20.0f,-200.0f };

//���[�v�|�C���g
constexpr VECTOR MOVE_POINT_POS_41 = { 910.0f,FRONT_MOVE_POINT_Y,2600.0f };
constexpr VECTOR MOVE_POINT_POS_42 = { -910.0f,FRONT_MOVE_POINT_Y,2600.0f };
constexpr VECTOR MOVE_POINT_POS_43 = { 910.0f,FRONT_MOVE_POINT_Y,-2700.0f };
constexpr VECTOR MOVE_POINT_POS_44 = { -910.0f,FRONT_MOVE_POINT_Y,-2700.0f };
constexpr VECTOR MOVE_POINT_POS_45 = { -910.0f,BASEMENT_MOVE_POINT_Y,1000.0f };
constexpr VECTOR MOVE_POINT_POS_46 = { 940.0f,BASEMENT_MOVE_POINT_Y,200.0f };


#pragma endregion


Tracker::Tracker(const Player& player):player_(player)
{
	//�X�V����֐��̃Z�b�g
	updateFunc_ = std::bind(& Tracker::UpdateNormal,this);
}

Tracker::~Tracker(void)
{
}

void Tracker::LoadData(void)
{
	//���f���o�^
	transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::TRACKER_MODEL));
	//�A�j���o�^
	std::string path = "Data/Model/Tracker/";
	animController_ = std::make_unique<AnimationController>(transform_.modelId_);
	animController_->Add((int)ANIM_TYPE::RUN, path + "Tracker_Anim_Walk.mv1", ANIM_PLAY_SPEED_WALK);
	animController_->Add((int)ANIM_TYPE::FAST_RUN, path + "Tracker_Anim_Tracking.mv1", ANIM_PLAY_SPEED_BASE);
	animController_->Add((int)ANIM_TYPE::STUN, path + "Tracker_Anim_Stun.mv1", ANIM_PLAY_SPEED_BASE);
	animController_->Add((int)ANIM_TYPE::CHACH, path + "Tracker_Anim_Attack.mv1", ANIM_PLAY_SPEED_CHACH);
	animController_->Add((int)ANIM_TYPE::LOST, path + "Tracker_Anim_Looking.mv1", ANIM_PLAY_SPEED_BASE);

	//�����T�E���h�̍쐬
	footstepsSound_ = std::make_shared<SoundController>();
	footstepsSound_->DataLoad(ResourceManager::SRC::FOOTSTEPS_TRACKER_SND);
}

void Tracker::Init(void)
{
	//3D��{���̏�����
	transform_.Init(
		TRACKER_SCL,
		TRACKER_POS,
		Quaternion(),
		Quaternion::Euler(TRACKER_LOCAL_ROT),
		Collider::TYPE::ENEMY
	);

	//�J�v�Z���R���C�_�[�̍쐬
	capsule_ = std::make_unique<Capsule>(transform_);
	capsule_->SetRadius(RADIOUS);
	capsule_->SetLocalPosDown(CAPSULE_LOCAL_POS_DOWN);
	capsule_->SetLocalPosTop(CAPSULE_LOCAL_POS_UP);

	//�Փ˃R���C�_�[�̃N���A
	ClearStageCollider();

	//�ړ��\�|�C���g�̏�����
	InitMovePoint();

	//�����A�j���[�V����
	animController_->Play((int)ANIM_TYPE::RUN);

	//�C����
	isFainting_ = false;

	//�����̏�����
	footstepsSound_->Init(
		FOOTSTEPS_SE_VOLUME, FOOTSTEPS_SE_HEARING_AREA,
		transform_.pos_, SoundController::REVERB_TYPE::PADDEDCELL);
}

void Tracker::Update(void)
{
	//���݂̍X�V�֐��̌Ăяo��
	updateFunc_();

	//�����ʒu�̐ݒ�
	footstepsSound_->SetSoundPos(transform_.pos_);

	//3D���̍X�V
	transform_.Update();
	// �A�j���[�V�����Đ�
	animController_->Update();
}

void Tracker::Draw(void)
{
	//���f���̕`��
	MV1DrawModel(transform_.modelId_);


#ifdef _DEBUG
	//�f�o�b�N�p
	capsule_->Draw();

		//���W
		DrawFormatString(900, 10, 0xffffff,
			L"�ǐՎҁF���W�i%.2f,%.2f,%.2f)",
			transform_.pos_.x,
			transform_.pos_.y,
			transform_.pos_.z);

		//�ړ��|�C���g
		int sphereNum = 0;
		int count = 0;
		for (const auto& point : movePoints_)
		{
			count++;
			//DrawFormatString(10, 50+count*16, 0xffffff,
			//	L"�S�[���񐔁F%d", point.goledNum);

			int color = 0xff0000;
			if (point.warpPoint)color = 0xccaa00;
			if (currentMovePointNum_ == sphereNum)color = 0x00aaff;
			DrawSphere3D(point.pos, MOVE_POINT_R, 10, color, color, false);
			sphereNum++;
		}
		DrawFormatString(900,26,0xffffff,
			L"�ړ��\�|�C���g�̐��F%d", moveablePointNums_.size());
		DrawFormatString(900, 42, 0xffffff,
			L"�ړ��|�C���g�i%.2f,%.2f,%.2f)",
			movePoints_[currentMovePointNum_].pos.x,
			movePoints_[currentMovePointNum_].pos.y,
			movePoints_[currentMovePointNum_].pos.z);

		//�����G���A
		DrawDiscoveryArea();

#endif // _DEBUG
}

void Tracker::SetGameOverInfo(const VECTOR& opponentPos)
{
	//�S�[�������̃N�H�[�^�j�I�����Z�b�g
	goalQuaRot_ = CalcQuaRad(opponentPos, transform_.pos_);
	//��]���Ԃ̏�����
	rotateTime_ = 0.0f;

	//�A�j���[�V�����̕ύX
	animController_->Play((int)ANIM_TYPE::CHACH, true, 0.0f, -1.0f, false, false, 0.0f);

	//�Q�[���I�[�o�[��ԂɕύX
	updateFunc_ = std::bind(&Tracker::UpdateGameOver, this);

}

void Tracker::InitMovePoint(void)
{
	//���݂̈ړ��|�C���g
	currentMovePointNum_ = 0;

	//�ړ��|�C���g���W�̃Z�b�g
	currentMovePointNum_ = 0;
	auto makePoint = [&](VECTOR pos, bool warp = false)
	{
		movePoints_.push_back(MovePoint{ 0,pos ,warp });
	};

	//��K
	makePoint(MOVE_POINT_POS_1);
	makePoint(MOVE_POINT_POS_2);
	makePoint(MOVE_POINT_POS_3);
	//�啔��
	makePoint(MOVE_POINT_POS_4);
	makePoint(MOVE_POINT_POS_5);
	makePoint(MOVE_POINT_POS_6);
	makePoint(MOVE_POINT_POS_7);
	makePoint(MOVE_POINT_POS_8);
	makePoint(MOVE_POINT_POS_9);
	//���L��
	makePoint(MOVE_POINT_POS_10);
	makePoint(MOVE_POINT_POS_11);
	makePoint(MOVE_POINT_POS_1);
	makePoint(MOVE_POINT_POS_13);
	makePoint(MOVE_POINT_POS_14);
	makePoint(MOVE_POINT_POS_15);
	makePoint(MOVE_POINT_POS_16);
	//�E�L��
	makePoint(MOVE_POINT_POS_17);
	makePoint(MOVE_POINT_POS_18);
	makePoint(MOVE_POINT_POS_19);
	makePoint(MOVE_POINT_POS_20);
	makePoint(MOVE_POINT_POS_21);
	makePoint(MOVE_POINT_POS_22);
	//������
	makePoint(MOVE_POINT_POS_23);
	makePoint(MOVE_POINT_POS_24);

	//�n��
	makePoint(MOVE_POINT_POS_25);
	makePoint(MOVE_POINT_POS_26);

	makePoint(MOVE_POINT_POS_27);
	makePoint(MOVE_POINT_POS_28);
	makePoint(MOVE_POINT_POS_29);
	makePoint(MOVE_POINT_POS_30);

	makePoint(MOVE_POINT_POS_31);
	makePoint(MOVE_POINT_POS_32);

	//�K�i�F��
	makePoint(MOVE_POINT_POS_33);
	makePoint(MOVE_POINT_POS_34);
	makePoint(MOVE_POINT_POS_35);
	makePoint(MOVE_POINT_POS_36);
	//�K�i�F�E
	makePoint(MOVE_POINT_POS_37);
	makePoint(MOVE_POINT_POS_38);
	makePoint(MOVE_POINT_POS_39);
	makePoint(MOVE_POINT_POS_40);

	//���[�v�|�C���g
	makePoint(MOVE_POINT_POS_41, true);
	makePoint(MOVE_POINT_POS_42, true);
	makePoint(MOVE_POINT_POS_43, true);
	makePoint(MOVE_POINT_POS_44, true);
	makePoint(MOVE_POINT_POS_45, true);
	makePoint(MOVE_POINT_POS_46, true);

}

void Tracker::SearchMoveablePoint(void)
{
	moveablePointNums_.clear();

	VECTOR trackerEyePos = transform_.pos_;
	trackerEyePos.y += EYE_LEVEL;				//�ǐՎ҂̖ڐ����W

	int count = -1;	//�ړ��|�C���g�ԍ�
	for (const auto& point : movePoints_)
	{
		count++;
		//���݂̈ړ��|�C���g�͏��O
		if (currentMovePointNum_ == count)continue;

		//�X�e�[�W��Lien���Փ˂��Ă��Ȃ��ꍇ�A�ړ��\�|�C���g�ɒǉ�����
		if (!CollCheckLine_AllStage(trackerEyePos, point.pos))
		{
			moveablePointNums_.emplace_back(count);
		}
	}
	assert(moveablePointNums_.size() > 0);		//�ړ��\�|�C���g�����݂��Ȃ�

	//�S�[���|�C���g�̌���
	DecisionGoalPoint();

	//�S�[�������̃N�H�[�^�j�I�����Z�b�g
	goalQuaRot_ = CalcQuaRad(goalPos_, transform_.pos_);
	//��]���Ԃ̏�����
	rotateTime_ = 0.0f;
}

void Tracker::DecisionGoalPoint(void)
{
	int golaNum = 0;

	// �e�ϐ��̒l���t���i�������قǑ傫�Ȓl�ɂȂ�j���d�݂Ƃ��Ĉ���
	std::vector<double> weights;
	for (int value : moveablePointNums_) {
		weights.push_back(1.0 / (static_cast<double>(movePoints_[value].goledNum) + 1));
	}

	// �d�݂̍��v���v�Z
	double totalWeight = 0.0;
	for (double weight : weights) {
		totalWeight += weight;
	}

	// 0.0����totalWeight�܂ł͈̔͂̃����_���Ȓl�𐶐�
	double randomValue = (std::rand() / static_cast<double>(RAND_MAX)) * totalWeight;

	// �d�݂̗ݐς��v�Z���A�����_���Ȓl�𒴂���ŏ��̃C���f�b�N�X��I��
	double cumulativeWeight = 0.0;
	for (int i = 0; i < moveablePointNums_.size(); ++i) {
		cumulativeWeight += weights[i];
		if (randomValue < cumulativeWeight) {
			golaNum = i;
			break;
		}
	}


	//�ړ��|�C���g�ԍ�������
	currentMovePointNum_ = moveablePointNums_[golaNum];

	//�S�[�����W�̃Z�b�g
	goalPos_ = movePoints_[currentMovePointNum_].pos;
	//�S�[���񐔂̉��Z
	movePoints_[currentMovePointNum_].goledNum++;
}

bool Tracker::IsWarping(void)
{
	return movePoints_[currentMovePointNum_].warpPoint;
}

void Tracker::ProcessWarp(void)
{
	//���[�v��̌���
	moveablePointNums_.clear();

	int count = -1;	//�ړ��|�C���g�ԍ�
	for (const auto& point : movePoints_)
	{
		count++;

		//���[�v�|�C���g�ȊO�͏��O
		if (!point.warpPoint)continue;

		//���݂̈ړ��|�C���g�͏��O
		if (currentMovePointNum_ == count)continue;

		//���[�v����ړ��\�|�C���g�ɒǉ�����
		moveablePointNums_.emplace_back(count);
	}
	assert(moveablePointNums_.size() > 0);	//�ړ��\�|�C���g�����݂��Ȃ�

	//�S�[���|�C���g�̌���
	DecisionGoalPoint();
	

	//�S�[�����W�Ƀ��[�v������
	transform_.pos_ = movePoints_[currentMovePointNum_].pos;

	printf("���[�v����\n");

}

VECTOR Tracker::GetMoveDir(const VECTOR& goalPos) 
{
	auto pos = VSub(goalPos, transform_.pos_);
	pos.y = 0;
	return VNorm(pos);
}  

bool Tracker::IsGoal(void)
{
	bool result = false;

	// ���f���ƈړ��|�C���g���̂̏Փ˔���
	auto hit = MV1CollCheck_Sphere(
		transform_.modelId_, -1, goalPos_, MOVE_POINT_R);

	if (hit.HitNum >= 1) {
		printf("�S�[��\n");
		result = true;
	}
	// ���o�����|���S�����̌�n��
	MV1CollResultPolyDimTerminate(hit);
	
	return result;
	
}

bool Tracker::IsDiscoveryPlayer(void)
{
	//�v���C���[�̍��W�擾
	auto playerPos = player_.GetTransform().pos_;
	auto trackerPos = transform_.pos_;
	//����
	float diff = (playerPos.z - trackerPos.z) * (playerPos.z - trackerPos.z) + 
		(playerPos.x - trackerPos.x) * (playerPos.x - trackerPos.x);	//2��̒l

	//�@����͈́i�����j�ɑ���L�����N�^�[�������Ă��Ȃ��ꍇ,false
	if (diff > VIEW_RANGE * VIEW_RANGE)return false;

	//�A�v���C���[�Ƃ̊Ԃ��X�e�[�W������ł���ꍇ�Afalse
	playerPos.y += EYE_LEVEL;
	trackerPos.y += EYE_LEVEL;
	if(CollCheckLine_AllStage(playerPos, trackerPos))return false;
	

	//Enemy���猩���v���C���[�̊p�x
	float playerPosRad = atan2f(playerPos.x - trackerPos.x, playerPos.z - trackerPos.z);
	float viewDeg = transform_.rot_.y;
	//�B����͈́i����p�j�ɑ���L�����N�^�[�������Ă��Ȃ��ꍇ�Afalse
	if (!((viewDeg - VIEW_ANGLE_RAD <= playerPosRad) &&
		playerPosRad <= (viewDeg + VIEW_ANGLE_RAD)))return false;

	//�v���C���[�𔭌�
	return true;
}

bool Tracker::IsLoseSightOfPlayer(void)
{
	//�v���C���[�ƕǂɑj�܂ꂽ��
	auto playerPos = player_.GetTransform().pos_;
	auto trackerPos = transform_.pos_;
	playerPos.y += EYE_LEVEL;
	trackerPos.y += EYE_LEVEL;
	if (CollCheckLine_AllStage(playerPos, trackerPos)&& 
		(Utility::Distance(playerPos, trackerPos)>=DIS_LOSE_SIGHT_OF_PLAYER))
	{
		return true;
	}
	return false;

}

void Tracker::DrawDiscoveryArea(void)
{
#ifdef DEBUG
	//�f�o�b�N�p

	//���ꂼ��̍��W
	VECTOR pos0 = transform_.pos_;
	VECTOR pos1 = VGet(0.0f, 0.0f, 0.0f);
	VECTOR pos2 = VGet(0.0f, 0.0f, 0.0f);
	VECTOR pos3 = VGet(0.0f, 0.0f, 0.0f);

	//�G�l�~�[�̎���ʒu�̍��W���擾
	auto calcPos = [&](float angleY, VECTOR mPos) {

		//�����Ă���������擾
		float dirX = sinf(angleY);
		float dirZ = cosf(angleY);
		//�P�ʃx�N�g�����擾
		VECTOR dir = VNorm({ dirX, 0.0f, dirZ });
		//�ړ��������擾
		VECTOR movePos = VScale(dir, VIEW_RANGE);
		//�ړ����������Z
		VECTOR posVal = VAdd(mPos, movePos);
		posVal.y += 2;		//�e�X�g�p�F������悤�ɂ��邽�߁A�n�ʂ��班��������

		return posVal;
	};

	pos1 = calcPos(transform_.rot_.y - VIEW_ANGLE_RAD, transform_.pos_);
	pos2 = calcPos(transform_.rot_.y, transform_.pos_);
	pos3 = calcPos(transform_.rot_.y + VIEW_ANGLE_RAD, transform_.pos_);

	pos0.y += 2;
	DrawLine3D(pos0, pos1, 0xff00ff);	//������30�x�̐��j
	DrawLine3D(pos0, pos2, 0xff00ff);	//�܂�������
	DrawLine3D(pos0, pos3, 0xff00ff);	//�E����30�x�̐��j
	DrawLine3D(pos1, pos2, 0xff00ff);	
	DrawLine3D(pos2, pos3, 0xff00ff);	

	//���m���f
	if (IsDiscoveryPlayer())
	{
		//�O�p�`�̕`��(���W�͎��v�������Ɉ�������͂���)
		DrawTriangle3D(pos0, pos1, pos2, 0xffff00, true);
		DrawTriangle3D(pos0, pos2, pos3, 0xffff00, true);
	}

#endif // DEBUG


}

void Tracker::ProcessMove(void)
{
	//�ړ������i�ړ��|�C���g���W�j
	moveDir_ = GetMoveDir(goalPos_);
	speed_ = SPEED_WALK;
	movePow_ = VAdd(movePow_, VScale(moveDir_, speed_));
}

void Tracker::ProcessTrack(void)
{
	//�ړ��ꏊ
	goalPos_ = player_.GetTransform().pos_;

	moveDir_ = GetMoveDir(goalPos_);	//�ړ�����(�v���C���[���W)
	speed_ = SPEED_RUN;					//�ړ��X�s�[�h
	movePow_ = VAdd(movePow_, VScale(moveDir_, speed_));	//�ړ��ʉ��Z
}

void Tracker::ProcessRotatePortal(void)
{
	//�ړI�����̃N�H�[�^�j�I�����Z�b�g
	actorRotY_ = CalcQuaRad(goalPos_, transform_.pos_);
}

void Tracker::ProcessRotateTrack(void)
{
	//��]�̋��ʕ⊮
	rotateTime_ += SceneManager::GetInstance().GetDeltaTime();
	float par = rotateTime_ / ROTATION_COMPLETED_TIME;
	actorRotY_ = Quaternion::Slerp(actorRotY_, goalQuaRot_, par);
}

void Tracker::UpdateNormal(void)
{
	SearchMoveablePoint();
	updateFunc_ = std::bind(&Tracker::UpdatePatrol,this);
}

void Tracker::UpdatePatrol(void)
{
	beforePos_ = transform_.pos_;		//�ړ��O���W
	movePow_ = Utility::VECTOR_ZERO;	// �ړ��ʂ��[��
	ProcessMove();
	CalcGravityPow();
	Collision();
	ProcessRotateTrack();
	// ��]������
	transform_.quaRot_ = actorRotY_;

	//�����T�E���h���Đ����Ă��Ȃ�΍Đ�
	footstepsSound_->PlayNonPlayingLoop();
	footstepsSound_->SetSoundFrequency(45000);


	//�아���g�p���ꂽ�ꍇ
	if (player_.IsUseAmulet())
	{
		//�C�⏈��
		isFainting_ = true;
		animController_->Play((int)ANIM_TYPE::STUN);
		updateFunc_ = std::bind(&Tracker::UpdateFainting, this);
		return;
	}

	//�v���C���[��������
	if (IsDiscoveryPlayer())
	{
		animController_->Play((int)ANIM_TYPE::FAST_RUN);
		updateFunc_ = std::bind(&Tracker::UpdateTracking, this);
		return;
	}

	//�S�[������
	if (IsGoal())
	{
		//�S�[���n�_�����[�v�|�C���g�����f
		if (IsWarping())
		{
			ProcessWarp();
		}

		SearchMoveablePoint();
		return;
	}
}

void Tracker::UpdateTracking(void)
{
	beforePos_ = transform_.pos_;		//�ړ��O���W
	movePow_ = Utility::VECTOR_ZERO;	// �ړ��ʂ��[��
	ProcessTrack();				// �ǐՏ���
	CalcGravityPow();			// �d�͂ɂ��ړ���
	Collision();				// �Փ˔���
	ProcessRotatePortal();		// ��]����
	transform_.quaRot_ = actorRotY_;	// ��]������

	//�����T�E���h���Đ����Ă��Ȃ�΍Đ�
	footstepsSound_->PlayNonPlayingLoop();
	footstepsSound_->SetSoundFrequency(88000);


	//�v���C���[�������������ǂ������f
	if (IsLoseSightOfPlayer())
	{
		//���X�g���
		animController_->Play((int)ANIM_TYPE::LOST,false);
		updateFunc_ = std::bind(&Tracker::UpdateLostPlayer, this);
		return;
	}

	//�아���g�p���ꂽ�ꍇ
	if (player_.IsUseAmulet())
	{
		//�C�⏈��
		isFainting_ = true;
		animController_->Play((int)ANIM_TYPE::STUN);
		updateFunc_ = std::bind(&Tracker::UpdateFainting, this);
		return;
	}
}

void Tracker::UpdateFainting(void)
{
	footstepsSound_->Stop();

	//�C����
	auto timeOver = Utility::TimeOver(faintingTime_, FAINT_TIME);
	if (timeOver)
	{
		faintingTime_ = 0.0f;
		isFainting_ = false;

		//���X�g���
		animController_->Play((int)ANIM_TYPE::LOST, false);
		updateFunc_ = std::bind(&Tracker::UpdateLostPlayer, this);
		return;
	}
}

void Tracker::UpdateLostPlayer(void)
{
	footstepsSound_->Stop();

	if (animController_->IsEnd())
	{
		SearchMoveablePoint();
		animController_->Play((int)ANIM_TYPE::RUN);
		updateFunc_ = std::bind(&Tracker::UpdatePatrol, this);
		return;
	}
}

void Tracker::UpdateGameOver(void)
{

	//��]�̋��ʕ⊮
	rotateTime_ += SceneManager::GetInstance().GetDeltaTime();
	float par = rotateTime_ / TROCKER_CHANGE_DIR_SPEED;	//�����]���X�s�[�h����
	actorRotY_ = Quaternion::Slerp(actorRotY_, goalQuaRot_, par);
	transform_.quaRot_ = actorRotY_;	// ��]������

}

