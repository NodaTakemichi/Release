#include <cmath>
#include <DxLib.h>
#include "../Application.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/DataManager/EffectManager.h"
#include "../Manager/DataManager/SoundManager.h"
#include "../Manager/DeckManager.h"
#include "../Manager/DataManager/BattleDataManager.h"

#include "../Utility/Utility.h"
#include "../Utility/DrawShader.h"
#include "../Battle/BattleSystem.h"
#include "../Battle/DeathStaging.h"

#include "../Object/Unit/Status/Command.h"
#include "../Object/Unit/Status/Buff.h"
#include "../Object/Unit/UnitManager.h"
#include "../Object/Unit/UnitBase.h"
#include "../Object/Roulette.h"
#include "../Object/GameUI.h"

#include "../_debug/_DebugConOut.h"
#include "../_debug/_DebugDispOut.h"

#include "GameScene.h"

//UI�p�t�H���g
constexpr int UI_FONT_SIZE = 36;
constexpr int UI_FONT_EDGE_SIZE = 2;
constexpr int UI_FONT_COLOR = 0xdddddd;
const Vector2 UI_POS = { 300,120 };
//��ʗh��
constexpr float AMPLITUDE_VALUE = 50.0f;		//�ő�h�ꕝ
constexpr int AMPLITUDE_ADJUSTMENT = 3;			//�h�ꕝ�����l
constexpr float AMPLITUDE_DECREASE = 0.9f;		//�����l
//�t���[���\�����W
const Vector2 FRAME_SHOW_POS = { -20, 110 };
//�X�^�[�g�摜�\�����W
const Vector2 START_IMAGE_SHOW_POS = { 800,500 };
//���݂̍s�����j�b�g�̃^�[�������J���[
constexpr int PLAUER_UNIT_TURN_TEXT_COLOR = 0x00ff00;	//�����G��
constexpr int ENEMY_UNIT_TURN_TEXT_COLOR = 0xff0000;	//�G�F��
//�N���b�N�w���e�L�X�g
constexpr float OPERATION_SHOE_TIME = 3.0f;		//�\������
constexpr float OPERATION_SHOE_INTERVAL = 0.5f;	//�\���Ԋu
constexpr int OPERATION_SHOE_POS_Y = 600;	//�\���ʒuY
constexpr int OPERATION_SHOE_COLOR = 0xffa500;	//�J���[
constexpr int OPERATION_SHOE_COLOR_EDGE = 0x0a0a0a;	//�G�b�W�J���[
//�Z�s�A��������
constexpr float SEPIA_FINISH_TIME = 2.0f;	//�G�b�W�J���[

GameScene::GameScene(void)
{
	screenPsBuf_ = COLOR_F{};
	screenPos_ = Vector2{};
	topFrameImg_=0;
	mainScreen_ = 0;
	frame_=0;
	winBgm_=0;	
	loseBgm_=0;		
	resultBgm_=0;		
	turnChangeSE_=0;	
	uiFontHandle_=0;
	resultImg_=0;		
	winImg_=0;	
	loseImg_=0;	
	battleStartImg_=0;
	bStartFadeImg_=0;
	isPlayerWin_=false;
	actUnitAoutm_ = true;
	resutlTime_=0.0f;
	amplitude_=0.0f;


}

GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
{

	// ���S���o������
	DeathStaging::CreateInstance();

	//���[���b�g
	roulette_ = new Roulette();
	roulette_->Init();

	//UI
	GameUi_ = new GameUI();
	GameUi_->Init();

	//�o�g���V�X�e��
	battleSys_ = new BattleSystem();
	battleSys_->Init();

	//���j�b�g�}�l�[�W���[
	unitMng_ = new UnitManager();
	unitMng_->Init();
	//���j�b�g�̐���
	auto& bd = BattleDataManager::GetInstance().GetBattleData();
	unitMng_->CreateUnit(bd.pDeck, bd.eDeck);



	//�w�i�摜�̓o�^
	backImgs_.push_back(LoadGraph("Data/Image/bg/red.jpg"));
	backImgs_.push_back(LoadGraph("Data/Image/bg/blue.jpg"));
	backImgs_.push_back(LoadGraph("Data/Image/bg/earth.jpg"));
	backImgs_.push_back(LoadGraph("Data/Image/bg/green.jpg"));
	backImgs_.push_back(LoadGraph("Data/Image/bg/holy.jpg"));
	backImgs_.push_back(LoadGraph("Data/Image/bg/dark.jpg"));

	bgImg_ = backImgs_[bd.bgNum];
	frameImg_ = LoadGraph("./Data/Image/UI/frame_full.png");
	topFrameImg_= LoadGraph("./Data/Image/UI/Turn.png");


	//�t�F�[�Y�̕ύX
	ChangeGamePhase(GAME_PHASE::BATTLE_START);

	//�T�E���h�̓o�^
	auto& snd = SoundManager::GetInstance();
	bgmHandle_ = snd.LoadSound(SoundManager::SOUND_TYPE::BATTLE1_BGN);
	winBgm_ = snd.LoadSound(SoundManager::SOUND_TYPE::VICTORY_BGN);
	loseBgm_ = snd.LoadSound(SoundManager::SOUND_TYPE::LOSE_BGN);
	resultBgm_ = 0;
	turnChangeSE_ = snd.LoadSound(SoundManager::SOUND_TYPE::TURN_CHANGE);
	//BGM�̍Đ�
	snd.PlaySoundBGM(bgmHandle_);

	//������
	resutlTime_ = 0.0f;
	//�X�N���[���֘A
	mainScreen_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);
	psType_ = DrawShader::PS_TYPE::TEXTURE();
	screenPsBuf_ = COLOR_F{};
	
	screenPos_ = {0,0};	//�X�N���[�����W
	amplitude_=0.0f;	//�X�N���[���U��
	frame_=0;			//�t���[��

	//����
	isPlayerWin_ = true;
	//���ʉ摜
	resultImg_ = 0;
	loseImg_ = LoadGraph("Data/Image/UI/Game/Lose.png");
	winImg_ = LoadGraph("Data/Image/UI/Game/Win.png");

	//�o�g���X�^�[�g�p�摜
	battleStartImg_ = LoadGraph("Data/Image/UI/Game/BattleStart.png");
	bStartFadeImg_ = LoadGraph("Data/Image/Fade/136.png");


	//UI�t�H���g
	uiFontHandle_ = CreateFontToHandle("������", UI_FONT_SIZE, -1,
		DX_FONTTYPE_ANTIALIASING_EDGE_4X4, -1, UI_FONT_EDGE_SIZE);
} 

void GameScene::Update(void)
{
	//�f���^�^�C��
	auto delta = SceneManager::GetInstance().GetDeltaTime();
	totalTime_ += delta;


	//����
	auto& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		// �o�g���̒��f�F�V�[���J��
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::SELECT, true);
		return;
	}

	//�h�ꉉ�o���m
	if (DeathStaging::GetInstance().CheckStartStanging())
	{
		amplitude_ = AMPLITUDE_VALUE;
	}
	//���S���o�̍X�V
	DeathStaging::GetInstance().Update();


	//�X�V
	unitMng_->Update();
	GameUi_->Update();

	//�t�F�[�Y�ʍX�V
	switch (phase_)
	{
	case GameScene::GAME_PHASE::BATTLE_START:
		//���S�t�F�[�h�����������Ƃ��A�o�g���V�[���֑J�ځA
		if (START_IMAGE_FADE_TIME * 2.0f <= totalTime_) {
			//�X�^�[�g
			ChangeGamePhase(GAME_PHASE::RULLET_TIME);
		}

		break;
	case GameScene::GAME_PHASE::RULLET_TIME:
		UpdateRouTime();
		break;
	case GameScene::GAME_PHASE::AIM:
		UpdateAIM();
		break;
	case GameScene::GAME_PHASE::EFFECT:
		UpdateEffect();
		break;
	case GameScene::GAME_PHASE::BATTLE:
		UpdateBattle();
		break;
	case GameScene::GAME_PHASE::BUFF_EFFECT :
		UpdateBuffEffect();
		break;
	case GameScene::GAME_PHASE::TURN_END:
		UpdateTurnEnd();
		break;
	case GameScene::GAME_PHASE::GAME_END:
		//�����F�Q�[���V�[���I��
		UpdateGameEnd();
		break;
	default:
		break;
	}


}

void GameScene::Draw(void)
{
	int sx = Application::SCREEN_SIZE_X;
	int sy = Application::SCREEN_SIZE_Y;

	auto& ds = DrawShader::GetInstance();

	//�X�N���[���Z�b�g
	SetDrawScreen(mainScreen_);
	ClearDrawScreen();


	//�w�i
	ds.DrawExtendGraphToShader({ 0,0 }, {sx,sy}, bgImg_);
	//�t���[��
	DrawGraph(FRAME_SHOW_POS.x, FRAME_SHOW_POS.y, frameImg_, true);
	DrawGraph(0, 0, topFrameImg_, true);

	//�I�u�W�F�N�g
	unitMng_->Draw();
	roulette_->Draw();

	//UI�֘A
	GameUi_->Draw();


	//�X�N���[���Z�b�g
	SetDrawScreen(DX_SCREEN_BACK);

	//�h��v�Z
	CalcScreenShake();
	//�X�N���[���`��
	ds.DrawGraphToShader(
		screenPos_, mainScreen_, psType_, screenPsBuf_);

	//�t�F�[�Y�ʕ`��
	switch (phase_)
	{
	case GameScene::GAME_PHASE::BATTLE_START:
		DrawBattleStart();
		break;
	case GameScene::GAME_PHASE::RULLET_TIME:
		break;
	case GameScene::GAME_PHASE::AIM:
		GameUi_->DrawActivSkill();
		break;
	case GameScene::GAME_PHASE::EFFECT:
		GameUi_->DrawActivSkill();
		break;
	case GameScene::GAME_PHASE::BATTLE:
		break;
	case GameScene::GAME_PHASE::BUFF_EFFECT:
		break;
	case GameScene::GAME_PHASE::TURN_END:
		break;
	case GameScene::GAME_PHASE::GAME_END:
		//���ʕ`��
		DrawResult();
		break;
	}

	//���S���o
	DeathStaging::GetInstance().PlayDeathStaging();

#ifdef DEBUG
	DrawPosition();
#endif // _DEBUG


}

void GameScene::Release(void)
{
	//���S���o�N���X�̊J��
	DeathStaging::GetInstance().Release();

	//����BGM�̒�~
	StopSoundMem(resultBgm_);
	StopSoundMem(bgmHandle_);

	//�j��
	unitMng_->Release();
	delete unitMng_;
	roulette_->Release();
	delete roulette_;
	GameUi_->Release();
	delete GameUi_;
	battleSys_->Release();
	delete battleSys_;

	//�摜���
	DeleteGraph(bgImg_);
	DeleteGraph(frameImg_);
	DeleteGraph(topFrameImg_);

	DeleteGraph(loseImg_);
	DeleteGraph(winImg_);
	DeleteGraph(resultImg_);

	DeleteGraph(battleStartImg_);
	DeleteGraph(bStartFadeImg_);


}

void GameScene::UpdateRouTime(void)
{
	//���[���b�g�̍X�V
	roulette_->Update();

	//���[���b�g�̒�~����i���j�b�g�ɂ���Ď������蓮�����߂�j
	roulette_->StopRoulette(actUnitAoutm_);

	//�o�t���f
	//�s�����j�b�g
	if (unitMng_->GetActivUnit()->CheckOwnBuff(Buff::BUFF_TYPE::PALALYSIS))
	{
		printf(unitMng_->GetActivUnit()->GetUnitName().c_str());
		printf("�̓}�q���Ă���\n\n");

		//��჏�Ԃ̏ꍇ�A�o�t�G�t�F�N�g�Đ���^�[���G���h����
		nextPhase_ = GAME_PHASE::TURN_END;
		ChangeGamePhase(GAME_PHASE::BUFF_EFFECT);
		return;
	}

	//���[���b�g����~������A�t�F�[�Y�ړ�
	bool next = roulette_->GetRouStop();
	if(next)ChangeGamePhase(GAME_PHASE::AIM);
	
}

void GameScene::UpdateAIM(void)
{	
	//�Ώۂ�I��������A�G�t�F�N�g�Đ��ɐi��
	bool next = battleSys_->SelectUnit(actUnitAoutm_);
	if(next)ChangeGamePhase(GAME_PHASE::EFFECT);
}

void GameScene::UpdateEffect(void)
{
	//�G�t�F�N�g�Đ����I���������ǂ������f
	bool next = EffectManager::GetInstance().FinishEffect();

	//�R�}���h�^�C�v�ɂ���āA�t�F�[�Y���ύX
	if (next) {
		//�G�t�F�N�g�A�j���[�V�������I��������
		if (roulette_->GetCmd()->GetCmdType() == Command::CMD_TYPE::BUFF)
		{
			//�o�t�G�t�F�N�g�t�F�[�Y�ɐi��
			nextPhase_ = GAME_PHASE::NONE;
			ChangeGamePhase(GAME_PHASE::BUFF_EFFECT);
		}
		else
		{
			//�o�g���t�F�[�Y�ɐi��
			ChangeGamePhase(GAME_PHASE::BATTLE);
		}
	}
}

void GameScene::UpdateBattle(void)
{
	//�_���[�W�������I���������ǂ������f
	bool next = battleSys_->FinishedDecHP();

	//���S���o���̏ꍇ�A�i�s���Ȃ�
	if(DeathStaging::GetInstance().PlayingStaging())return;

	//�o�g���i�_���[�W�����j���I��������A�^�[���I���ɐi��
	if (next)ChangeGamePhase(GAME_PHASE::TURN_END);
}

void GameScene::UpdateBuffEffect(void)
{
	//(�t�^��)�o�t�G�t�F�N�g����
	bool next = battleSys_->FinishedBuffEffect();

	//��Ԉُ�V�F�[�_�[�ϑ��n�_
	nextPhase_ = GAME_PHASE::TURN_END;

	//�o�t�G�t�F�N�g�I�����A���̃t�F�[�Y�ɐi��
	if(next)ChangeGamePhase(nextPhase_);
}

void GameScene::UpdateTurnEnd(void)
{
	//�_���[�W�������I���������ǂ������f�i�Ń_���[�W�j
	bool next = unitMng_->GetActivUnit()->DecHpProcess();


	//�S�łȂ�΃Q�[���I���A
	//�����łȂ����A�K�v�����I����Ȃ烋�[���b�g�t�F�[�Y�ɐi�ށB
	if (next) 
	{
		if (unitMng_->IsAnniUnit())ChangeGamePhase(GAME_PHASE::GAME_END);
		else 
		{
			//�^�[���`�F���W
			auto& snd = SoundManager::GetInstance();
			snd.PlaySoundBack(turnChangeSE_);

			ChangeGamePhase(GAME_PHASE::RULLET_TIME);
		}
	}
}

void GameScene::UpdateGameEnd(void)
{
	//���ʉ�ʁF�o�ߎ���
	resutlTime_ += SceneManager::GetInstance().GetDeltaTime();

	//����
	auto& ins = InputManager::GetInstance();
	// �V�[���J��
	if (ins.IsClickMouseLeft())
	{
		if (isPlayerWin_)
		{
			//���U���g�V�[���֑J��
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::RESULT, true);
		}
		else
		{
			//�Z���N�g�V�[���֑J��
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::SELECT, true);
		}
	}

}

void GameScene::ChangeGamePhase(GAME_PHASE phase)
{
	//�t�F�[�Y
	phase_ = phase;

	switch (phase_)
	{
	case GameScene::GAME_PHASE::BATTLE_START: {
		break;
	}
	case GameScene::GAME_PHASE::RULLET_TIME: {
		//�I�����j�b�g�̃��Z�b�g
		battleSys_->ResetSelectUnits();

		//�s�����j�b�g�̐���
		unitMng_->ChangeActivUnit();

		//�s�����j�b�g
		//���[���b�g�̒�~���蓮�����������f
		actUnitAoutm_ = (unitMng_->GetActivUnit()->GetUnitType() == UnitBase::UNIT_TYPE::ENEMY);

		//���[���b�g�ɃR�}���h�Z���Z�b�g
		roulette_->SetCommand(unitMng_->GetCommand());
		//���[���b�g�󋵂̃��Z�b�g
		roulette_->ResetRouSpin();


		//���݂̍s�����j�b�g�̃^�[������ 
		bool turn = unitMng_->GetActivUnit()->GetUnitType() == UnitBase::UNIT_TYPE::PLAYER;
		int color = turn ? PLAUER_UNIT_TURN_TEXT_COLOR : ENEMY_UNIT_TURN_TEXT_COLOR;
		auto& name = unitMng_->GetActivUnit()->GetUnitName();
		GameUi_->SetTurnString(name, color);

		break;
	}
	case GameScene::GAME_PHASE::AIM: {
		//���肵���R�}���h���Q�[��UI�Ɏ󂯓n��
		std::string cmd = roulette_->GetCmd()->GetName();
		GameUi_->SetCmdName(cmd);

		//�o�g���f�[�^�Ɋe����n��
		battleSys_->SetBattleData(unitMng_->GetActivUnit(),
			roulette_->GetCmd(), unitMng_->GetUnits());

		//�z�C�[���̃��Z�b�g
		GetMouseWheelRotVol();

		//�R�}���h�̑I��Ώۂ𔻒f����
		battleSys_->CheckSelectTarget();

		//���삪�����̏ꍇ�A���j�b�g�������_���Ɍ��߂�
		if (actUnitAoutm_)battleSys_->SetRandUnit();

		break;
	}
	case GameScene::GAME_PHASE::EFFECT: {

		//�R�}���h����G�t�F�N�g�ԍ��̎擾
		int num = roulette_->GetCmd()->GetEffectNum();

		//�^�[�Q�b�g�̍��W�擾(�^�[�Q�b�g�����݂��Ȃ��ꍇ�A�������W)
		Vector2 pos = unitMng_->GetActivUnit()->GetUnitPos();
		auto& target = battleSys_->GetTargetUnit();

		if (target.size() <= 0)
		{
			//�G�t�F�N�g�Đ�(�^�[�Q�b�g�����݂��Ȃ��ꍇ)
			EffectManager::GetInstance().PlayEffect(num, pos);
			return;
		}

		for (auto& unit : target)
		{
			//���j�b�g�̍��W���擾
			pos=unit->GetUnitPos();
			//�G�t�F�N�g�Đ�
			EffectManager::GetInstance().PlayEffect(num, pos);
		}

		break;
	}
	case GameScene::GAME_PHASE::BATTLE: {
		//�R�}���h����
		battleSys_->CmdProcess();

		break;
	}
	case GameScene::GAME_PHASE::BUFF_EFFECT: {
		//�R�}���h���������Ȃ�
		if (nextPhase_ == GameScene::GAME_PHASE::TURN_END)return;

		//�R�}���h����
		battleSys_->CmdProcess();
		break;
	}
	case GameScene::GAME_PHASE::TURN_END: {
		//�s�����j�b�g�̃G���h����
		unitMng_->GetActivUnit()->TurnEndProcess();

		break;
	}
	case GameScene::GAME_PHASE::GAME_END: {
		//���Ҕ���
		isPlayerWin_ = CheckVictory();


		//����摜�̃Z�b�g
		SettingScreenShader();


		//���y�̍Đ�
		StopSoundMem(bgmHandle_);
		resultBgm_ = isPlayerWin_ ? winBgm_ : loseBgm_;
		auto& snd = SoundManager::GetInstance();
		snd.PlaySoundBGM(resultBgm_);


		break;
	}
	}
}

void GameScene::CalcScreenShake(void)
{
	//�X�N���[���̍��W
	if (amplitude_ > 0.0f) {
		//�X�N���[���h�炵
		screenPos_.x = ((frame_ % AMPLITUDE_ADJUSTMENT) - 1) * amplitude_;
		screenPos_.y = (((frame_ + rand()) % AMPLITUDE_ADJUSTMENT) - 1) * amplitude_;
	}

	//�h������X�ɏ��������Ă���
	amplitude_ *= AMPLITUDE_DECREASE;
	if (amplitude_ < 1.0f) {
		amplitude_ = 0.0f;
	}
	++frame_;
}

bool GameScene::CheckVictory(void)
{
	bool win = false;
	for (auto& unit : unitMng_->GetUnits())
	{
		if (!unit->IsAlive())continue;
		if (unit->GetUnitType() == UnitBase::UNIT_TYPE::PLAYER)
		{
			win = true;
			break;
		}
	}

	return win;
}

void GameScene::DrawBattleStart(void)
{
	//��ʃT�C�Y
	auto sx = Application::SCREEN_SIZE_X / 2;
	auto sy = Application::SCREEN_SIZE_Y / 2;

	auto& ds = DrawShader::GetInstance();

	Vector2 size = START_IMAGE_SHOW_POS;
	Vector2 pos = { sx - size.x / 2,sy - size.y / 2 };

	//�p�[�Z���g
	float time = START_IMAGE_FADE_TIME;
	float par =  totalTime_/ time;
	if (par >= 1.0f)
	{
		par -= totalTime_ - time;
	}
	COLOR_F buf = COLOR_F{ par };

	ds.DrawGraphAndSubToShader(pos, size, battleStartImg_,bStartFadeImg_,
		DrawShader::PS_TYPE::FADE_TEXTURE,buf);
}

void GameScene::DrawResult(void)
{
	//��ʃT�C�Y
	auto sx = Application::SCREEN_SIZE_X / 2;
	auto sy = Application::SCREEN_SIZE_Y / 2;

	auto& ds = DrawShader::GetInstance();

	//�萔�o�b�t�@
	if (!isPlayerWin_)
	{
		//��ʂ̃Z�s�A��
		screenPsBuf_ = COLOR_F{ resutlTime_ ,resutlTime_ / SEPIA_FINISH_TIME };
	}

	//���s�����`��
	int rx, ry;
	GetGraphSize(resultImg_, &rx, &ry);
	Vector2 pos = { sx - rx / 2, sy - ry / 2 };
	COLOR_F buf = COLOR_F{ totalTime_ };
	ds.DrawGraphToShader(
		pos, resultImg_, 
		DrawShader::PS_TYPE::DISTORTION , buf);


	//�u�N���b�N���Ăˁv
	std::string inst = "�N���b�N���Ă�";
	auto len = Utility::CalcStringLength(inst, uiFontHandle_) / 2;
	if (fmodf(totalTime_, OPERATION_SHOE_TIME) >= OPERATION_SHOE_INTERVAL)
	{
		DrawStringToHandle(
			sx - len, OPERATION_SHOE_POS_Y, inst.c_str(),
			OPERATION_SHOE_COLOR,
			uiFontHandle_,
			OPERATION_SHOE_COLOR_EDGE
		);
	}
}

void GameScene::SettingScreenShader(void)
{
	//�X�N���[����ʃV�F�[�_�[�̐ݒ�
	if (!isPlayerWin_)
	{
		//�������ꍇ�A��ʂ����m�N���ɂ���
		psType_ = DrawShader::PS_TYPE::SIMPLE_SEPIA;
	}

	//�摜�I��
	resultImg_ = isPlayerWin_ ? winImg_ : loseImg_;
}

