#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../Application.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/SoundManager.h"
#include "../Manager/Camera.h"
#include "../Utility/Utility.h"

#include "../Object/Stage/Stage.h"
#include "../Object/Stage/GoalGate.h"
#include "../Object/Stage/GoalDoor.h"
#include "../Object/Stage/WarpGate.h"
#include "../Object/Character/Player.h"
#include "../Object/Character/Tracker.h"
#include "../Object/Item/ItemBase.h"
#include "../Object/Item/Dool.h"
#include "../Object/Item/Amulet.h"
#include "../UI/PlayerUI.h"
#include "../UI/OperationUI.h"

#include "../Graphics/Drawable/PostEffect/PostEffectMaterial.h"
#include "../Graphics/Drawable/PostEffect/PostEffectRenderer.h"
#include "../Sound/SoundController.h"

#include "PauseScene.h"
#include "GameOverScene.h"
#include "GameScene.h"

//�S�[��SE����
constexpr int GOAL_SE_VOLUME = 150;
constexpr int GOAL_SE_VOLUMsE = 150;

//���[�v�S�[�����W
constexpr int WARP_GATE_CREATE_NUM = 6;
constexpr VECTOR WARP_GATE_POS_FLOOR_LEFT_BOTTOM	= { -910.0f,400.0f,-2330.0f };	//��K�@����
constexpr VECTOR WARP_GATE_POS_FLOOR_RIGHT_BOTTOM	= { 910.0f,400.0f,-2330.0f };	//��K�@�E��
constexpr VECTOR WARP_GATE_POS_FLOOR_LEFT_TOP		= { -910.0f,400.0f, 2330.0f };	//��K�@����
constexpr VECTOR WARP_GATE_POS_FLOOR_RIGHT_TOP		= { 910.0f,400.0f, 2330.0f };	//��K�@�E��
constexpr VECTOR WARP_GATE_POS_BASEMENT_RIGHT		= { 570.0f,-110.0f, 200.0f };	//�n���@�E
constexpr VECTOR WARP_GATE_POS_BASEMENT_LEFT		= { -370.0f,-110.0f,1000.0f };	//�n���@��

//�ʂ������
constexpr float DOOL_MODEL_SCALE = 0.25f;	//�傫��
//�T��
constexpr VECTOR MONKEY_POS = { -1059.0f,435.0f,-599.0f };	//���W
constexpr VECTOR MONKEY_ROT = { 0.0f,-90.0f,0.0f };			//�p�x
//�E�T�M
constexpr VECTOR RABBIT_POS = { 448.0f,-60.0f,560.0f };		//���W
constexpr VECTOR RABBIT_ROT = { 0.0f,60.0f,0.0f };			//�p�x
//�q�c�W
constexpr VECTOR SHEEP_POS  = { 514.0f, 430.0f,1430.0f };	//���W
constexpr VECTOR SHEEP_ROT  = { 0.0f,150.0f,0.0f };			//�p�x

//�아
constexpr float AMULET_MODEL_SCALE = 1.0f;	//�傫��
//�S�[���O
constexpr VECTOR AMULET_POS_GOAL_FRONT		 = { -105.0f,500.0f,-1518.0f };
constexpr VECTOR AMULET_ROT_GOAL_FRONT		 = { 0,0,10 };
//��O�ʘH
constexpr VECTOR AMULET_POS_PASSAGEWAY_FRONT = { 780.0f,500.0f,1316.0f };
constexpr VECTOR AMULET_ROT_PASSAGEWAY_FRONT = { 0,180,-23 };
//�T���̉�
constexpr VECTOR AMULET_POS_BESIDE_MONKEY	 = { -1082.0f,500.0f,-480.0f };
constexpr VECTOR AMULET_ROT_BESIDE_MONKEY 	 = { 0,-90,0 };
//�E���̏�����	
constexpr VECTOR AMULET_POS_SMALL_ROOM_RIGHT = { 370.0f,500.0f,-510.0f };
constexpr VECTOR AMULET_ROT_SMALL_ROOM_RIGHT = { 0,0,-38 };
//�����̏�����
constexpr VECTOR AMULET_POS_SMALL_ROOM_LEFT	 = { -600.0f,500.0f,1900.0f };
constexpr VECTOR AMULET_ROT_SMALL_ROOM_LEFT	 = { 0,0,25 };
//�啔��
constexpr VECTOR AMULET_POS_LARG_ROOM		 = { -500.0f,500.0f,295.0f };
constexpr VECTOR AMULET_ROT_LARG_ROOM		 = { 0,0,25 };
//�n���̉E
constexpr VECTOR AMULET_POS_BASEMENT_RIGHT	 = { 245.0f,-2.0f,1155.0f };
constexpr VECTOR AMULET_ROT_BASEMENT_RIGHT	 = { 0,30,0 };
//�n���̍�
constexpr VECTOR AMULET_POS_BASEMENT_LEFT	 = { -295.0f,-2.0f,1155.0f };
constexpr VECTOR AMULET_ROT_BASEMENT_LEFT	 = { 0,-90,30 };

//���S���o����
constexpr float DETH_SHOW_TIME = 2.25f;

GameScene::GameScene(SceneManager& manager):SceneBase(manager)
{
	//�X�V�֐��̃Z�b�g
	updataFunc_ = [&](InputManager& input) {
		LoadingUpdate(input);
	};
	//�`��֐��̃Z�b�g
	drawFunc_ = std::bind(&GameScene::LoadingDraw, this);
}

GameScene::~GameScene(void)
{
}

void GameScene::LoadData(void)
{
	auto& res = ResourceManager::GetInstance();
	loadingTime_ = 0.0f;

	//�񓯊��ǂݍ��݂�L���ɂ���
	SetUseASyncLoadFlag(true);


	//�v���C���[
	player_ = std::make_unique<Player>();
	player_->LoadData();
	//�X�e�[�W
	stage_ = std::make_unique<Stage>(*player_);
	stage_->LoadData();
	//�S�[���Q�[�g
	goalGate_ = std::make_unique<GoalGate>();
	goalGate_->LoadData();
	//�ǐՎ�
	tracker_ = std::make_unique<Tracker>(*player_);
	tracker_->LoadData();
	//�S�[���h�A
	goalDoor_ = std::make_unique<GoalDoor>();
	goalDoor_->LoadData();
	//�A�C�e���̐���
	LoadItem();
	//���[�v�Q�[�g�̐���
	for (int i = 0; i < WARP_GATE_CREATE_NUM; i++)
	{
		warpGates_.push_back(std::make_unique<WarpGate>());
		warpGates_[i]->LoadData();
	}

	//�v���C���[UI
	playerUI_ = std::make_unique<PlayerUI>(*player_);
	playerUI_->LoadData();
	//����UI
	operationUI_ = std::make_unique<OperationUI>();
	operationUI_->LoadData();

	//�S�[��SE
	goalSE_ = std::make_shared<SoundController>();
	goalSE_->DataLoad(ResourceManager::SRC::GOAL_SND, false);

	//�t�H���g�̓o�^
	res.Load(ResourceManager::SRC::SELECT_CMD_FONT);
	buttnFontHandle_ = CreateFontToHandle(L"EbiharaNoKuseji", FONT_SIZE, 0);

}

void GameScene::Init(void)
{
	//�X�e�[�W
	stage_->Init();
	//�S�[���Q�[�g
	goalGate_->Init();
	//�S�[���h�A
	goalDoor_->Init();
	//�v���C���[
	player_->Init();
	//�ǐՎ�
	tracker_->Init();
	//�A�C�e���̐���
	CreateItems();
	//���[�v�Q�[�g�̐���
	for (const auto& warpGate: warpGates_)
	{
		warpGate->Init();
	}
	//���[�v�Q�[�g�̍��W�A�p�x�̐ݒ�
	int count = 0;
	auto createWarpGate = [&](VECTOR pos, VECTOR rot)
	{
		if (count >= warpGates_.size())return;
		warpGates_[count]->SetPos(pos);
		warpGates_[count]->SetRot(rot);
		count++;
	};
	createWarpGate(WARP_GATE_POS_FLOOR_LEFT_BOTTOM, {});
	createWarpGate(WARP_GATE_POS_FLOOR_RIGHT_BOTTOM, {});
	createWarpGate(WARP_GATE_POS_FLOOR_LEFT_TOP, {});
	createWarpGate(WARP_GATE_POS_FLOOR_RIGHT_TOP, {});
	createWarpGate(WARP_GATE_POS_BASEMENT_RIGHT, { 0.0f,Utility::ANGLE_90_RAD,0.0f });
	createWarpGate(WARP_GATE_POS_BASEMENT_LEFT, { 0.0f,Utility::ANGLE_90_RAD,0.0f });


	//�v���C���[UI
	playerUI_->Init();
	//����UI
	operationUI_->Init();

	//�X�e�[�W�R���C�_�[���̎󂯓n��
	player_->AddStageCollider(*(stage_->GetCollStageTransform().collider_));
	player_->AddStageCollider(*(goalGate_->GetTransform().collider_));
	player_->AddStageCollider(*(goalDoor_->GetTransform().collider_));
	for (const auto& warpGate : warpGates_)
	{
		player_->AddStageCollider(*(warpGate->GetTransform().collider_));
	}
	tracker_->AddStageCollider(*(stage_->GetCollStageTransform().collider_));
	//�S�[���Q�[�g���̎󂯓n��
	player_->SetGoalGate(goalGate_);

	//�|�X�g�G�t�F�N�g
	InitPE();

	//�J�������[�h�̃Z�b�g�i��l�̃J�����j
	sceneManager_.GetCamera().ChangeMode(Camera::MODE::FPS);
	//�J�����Ǐ]�Ώۂ̐ݒ�
	sceneManager_.GetCamera().SetFollow(&player_->GetTransform());


	//�S�[��SE
	goalSE_->Init(GOAL_SE_VOLUME);
}

void GameScene::Update(InputManager& input)
{
	updataFunc_(input);
	return;
}

void GameScene::Draw(void)
{
	drawFunc_();
	return;
}

void GameScene::InitPE(void)
{
	auto& res = ResourceManager::GetInstance();

	//���C���X�N���[��
	int mainScreen = sceneManager_.GetMainScreen();
	//�m�C�Y�摜
	int noiseImg = res.Load(ResourceManager::SRC::NOISE_IMG).handleId_;

}

void GameScene::CreateItems(void)
{
	std::shared_ptr<ItemBase> item;

	auto createItem = [&](
		ItemBase::ITEM_TYPE type,VECTOR scl, VECTOR pos, VECTOR rot) 
	{
		item->SetModelInfo(type,scl,pos,rot);

		//�A�C�e���R���C�_�[���̎󂯓n��
		player_->AddItemInfo(item);
		items_.emplace_back(std::move(item));
	};

	//�ʂ�����݂̑傫��
	const float doolScl = DOOL_MODEL_SCALE;

	//�ʂ�����ݐ����i�T���j
	item = std::make_shared<Dool>();
	createItem(
		ItemBase::ITEM_TYPE::MONKEY,
		{ doolScl,doolScl,doolScl },
		MONKEY_POS,	MONKEY_ROT
	);
	//�ʂ�����ݐ����i�E�T�M�j
	item = std::make_shared<Dool>();
	createItem(
		ItemBase::ITEM_TYPE::RABBIT,
		{ doolScl,doolScl ,doolScl },
		RABBIT_POS,	RABBIT_ROT
	);
	////�ʂ�����ݐ����i�q�c�W�j
	//item = std::make_shared<Dool>();
	//createItem(
	//	ItemBase::ITEM_TYPE::SHEEP,
	//	{ doolScl,doolScl ,doolScl },
	//	SHEEP_POS,SHEEP_ROT
	//);


	//�아�A�C�e���̐���
	//�아�̑傫��
	const float amuletScl = AMULET_MODEL_SCALE;

	item = std::make_shared<Amulet>();		//�S�[���O
	createItem(
		ItemBase::ITEM_TYPE::AMULET,
		{ amuletScl,amuletScl,amuletScl },
		AMULET_POS_GOAL_FRONT, AMULET_ROT_GOAL_FRONT
	);
	item = std::make_shared<Amulet>();		//��O�ʘH
	createItem(
		ItemBase::ITEM_TYPE::AMULET,
		{ amuletScl ,amuletScl ,amuletScl },
		AMULET_POS_PASSAGEWAY_FRONT, AMULET_ROT_PASSAGEWAY_FRONT
	);
	item = std::make_shared<Amulet>();		//�T���̖D�����
	createItem(
		ItemBase::ITEM_TYPE::AMULET,
		{ amuletScl ,amuletScl ,amuletScl },
		AMULET_POS_BESIDE_MONKEY, AMULET_ROT_BESIDE_MONKEY
	);
	item = std::make_shared<Amulet>();		//�E���̏�����
	createItem(
		ItemBase::ITEM_TYPE::AMULET,
		{ amuletScl ,amuletScl ,amuletScl },
		AMULET_POS_SMALL_ROOM_RIGHT, AMULET_ROT_SMALL_ROOM_RIGHT
	);
	item = std::make_shared<Amulet>();		//�����̏�����
	createItem(
		ItemBase::ITEM_TYPE::AMULET,
		{ amuletScl ,amuletScl ,amuletScl },
		AMULET_POS_SMALL_ROOM_LEFT, AMULET_ROT_SMALL_ROOM_LEFT
	);
	item = std::make_shared<Amulet>();		//�啔��
	createItem(
		ItemBase::ITEM_TYPE::AMULET,
		{ amuletScl ,amuletScl ,amuletScl },
		AMULET_POS_LARG_ROOM, AMULET_ROT_LARG_ROOM
	);
	//�n��
	item = std::make_shared<Amulet>();
	createItem(
		ItemBase::ITEM_TYPE::AMULET,
		{ amuletScl ,amuletScl ,amuletScl },
		AMULET_POS_BASEMENT_LEFT, AMULET_ROT_BASEMENT_LEFT
	);
	item = std::make_shared<Amulet>();
	createItem(
		ItemBase::ITEM_TYPE::AMULET,
		{ amuletScl ,amuletScl ,amuletScl },
		AMULET_POS_BASEMENT_RIGHT, AMULET_ROT_BASEMENT_RIGHT
	);


}

void GameScene::LoadItem(void)
{
	auto& res = ResourceManager::GetInstance();

	res.Load(ResourceManager::SRC::AMULET_MODEL);
	res.Load(ResourceManager::SRC::DOOL_MONKEY);
	res.Load(ResourceManager::SRC::DOOL_PIG);
	res.Load(ResourceManager::SRC::DOOL_RABBIT);
	res.Load(ResourceManager::SRC::DOOL_SHEEP);
}

void GameScene::LoadingUpdate(InputManager& input)
{
	bool loadTimeOver = Utility::TimeOver(loadingTime_, LOADING_TIME);

	//���[�h���������������f
	if (GetASyncLoadNum() == 0 && loadTimeOver)
	{
		//�񓯊������𖳌��ɂ���
		SetUseASyncLoadFlag(false);

		//�J�[�\�����[�h�̕ύX
		input.ChangeCurrsolMode(false);


		//����������
		Init();
		//�t�F�[�h�C���J�n
		sceneManager_.StartFadeIn();

		//�X�V�֐��̃Z�b�g
		updataFunc_ = [&](InputManager& input) {NormalUpdate(input);};
		//�`��֐��̃Z�b�g
		drawFunc_ = std::bind(&GameScene::NormalDraw, this);
	}
}

void GameScene::NormalUpdate(InputManager& input)
{
	//�|�[�Y�V�[���֑J��
	if (input.IsTrgDown(KEY_INPUT_ESCAPE))
	{
		sceneManager_.PushScene(std::make_shared<PauseScene>(sceneManager_));
		return;
	}

	//�Q�[���I�[�o�[����
	if (CheckGameOver()) 
	{
		//�Q�[���I�[�o�[������
		if (!isGameOver_)
		{
			isGameOver_ = true;
			//���ꂼ��̍��W���擾
			auto playerPos = player_->GetTransform().GetPos();
			auto trackerPos = tracker_->GetTransform().GetPos();

			//���ꂼ��ɑ�����W��ݒ�A��]������
			player_->SetGameOverInfo(trackerPos);
			tracker_->SetGameOverInfo(playerPos);

			//�����̑S��~
			SoundManager::GetInstance().StopAllSound();
		}

		//���o�I������
		gameOverTime_ += sceneManager_.GetDeltaTime();
		if (Utility::TimeOver(gameOverTime_, DETH_SHOW_TIME))
		{
			//�J�[�\�����[�h�̕ύX
			input.ChangeCurrsolMode(true);

			//�Q�[���I�[�o�[�V�[���֑J��
			sceneManager_.PushScene(std::make_shared<GameOverScene>(sceneManager_));
			return;
		}
	}

	//�Q�[���N���A����
	if (player_->IsEscape())
	{
		//�N���A����
		goalSE_->PlayNonPlaying();

		//�J�[�\�����[�h�̕ύX
		input.ChangeCurrsolMode(true);

		//�Q�[���N���A�V�[���֑J��
		sceneManager_.ChangeScene(SceneManager::SCENE_ID::RESULT);
		return;
	}

	//�T�E���h���X�i�[���̍X�V
	auto& soundMng = SoundManager::GetInstance();
	VECTOR listenerPos = player_->GetTransform().pos_;
	VECTOR listenerDir = player_->GetDirection();
	Set3DSoundListenerPosAndFrontPos_UpVecY(listenerPos, VAdd(listenerPos, listenerDir));


	//�e�I�u�W�F�N�g�X�V
	stage_->Update();
	goalGate_->Update();
	player_->Update();	
	tracker_->Update();
	for (const auto& item : items_) 
	{
		item->Update();
	}
	for (const auto& warpGate : warpGates_) 
	{
		warpGate->Update();
	}
	playerUI_->Update();

	// Effekseer�ɂ��Đ����̃G�t�F�N�g���X�V����B
	UpdateEffekseer3D();
}

void GameScene::LoadingDraw(void)
{
	//�unow loading......�v�̕`��
	DrawNowLoading();
}

void GameScene::NormalDraw(void)
{
	//�e�I�u�W�F�N�g�̕`��
	stage_->Draw();
	goalGate_->Draw();
	goalDoor_->Draw();
	player_->Draw();
	tracker_->Draw();
	operationUI_->Draw();
	for (const auto& item : items_) {
		item->Draw();
	}
	for (const auto& warpGates : warpGates_) {
		warpGates->Draw();
	}

	// Effekseer�ɂ��Đ����̃G�t�F�N�g��`�悷��B
	DrawEffekseer3D();

	//�v���C���[UI
	playerUI_->Draw();

}

bool GameScene::CheckGameOver(void)
{
	//�Փ˂��Ă���ꍇ�Atrue
	float dis = static_cast<float>(Utility::Distance(player_->GetTransform().pos_, 
		tracker_->GetTransform().pos_));
	float radious = player_->GetRadious() + tracker_->GetRadious();
	bool isHit = (dis <= radious);

	//�ǐՎ҂��C�⒆�łȂ��ꍇ�Atrue
	bool isNotFainting = !tracker_->GetFainting();


	return isHit && isNotFainting;
}

