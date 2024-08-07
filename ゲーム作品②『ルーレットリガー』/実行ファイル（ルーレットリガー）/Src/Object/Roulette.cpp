#include<DxLib.h>
#include<math.h>
#include"../Application.h"
#include"../Utility/Utility.h"
#include"../Manager/SceneManager.h"
#include"../Manager/InputManager.h"
#include"../Manager/DataManager/SoundManager.h"
#include"./Unit/Status/Command.h"
#include"../_debug/_DebugConOut.h"
#include"../_debug/_DebugDispOut.h"
#include "Roulette.h"

//���[���b�g�̉�]
constexpr float ROT_SPEED_MAX = -10.0f;		//���x
constexpr float ROT_BRAKE = 120.0f;			//�u���[�L��
constexpr float ROT_STOP_SPEED = 0.1f;			//��]��~���f���x
//�I�[�g�ł̃��[���b�g��~����܂ł̑ҋ@����
constexpr float STOP_WAIT_TIME_ROU = 0.7f;
//���[���b�g�p�t�H���g
constexpr int ROULETTE_FONT_SIZE = 20;
constexpr int ROULETTE_FONT_THICK = 10;
constexpr int ROULETTE_FONT_COLOR = 0xe6e6e6;
//���ʒu
const Vector2 ARROW_SHOW_POS = Vector2{ -50,150 };
//���[���b�g�ʒu
const Vector2 ROULETTE_SHOW_POS = Vector2{ 0,530 };
//�R�}���h���\��
const Vector2 ROTATE_LOCAL_CENTER_POS = Vector2{190,10};
constexpr int CMD_NAME_COLOR = 0x220022;
//���
constexpr float ONE_LAP = 360.0f;


Roulette::Roulette()
{
	rouletteImg_= 0;	
	rouFrame_= 0;	
	center_= 0;		
	arrowImg_= 0;
	fontHandle_= 0;
	rotateSE_= 0;
	stopingSE_= 0;
	stopBtnSE_= 0;
	angle_=0.0;
	rotPower_=0.0;
	isRouSpin_=true;
	isStop_=false;
	totalTime_=0.0f;
}

Roulette::~Roulette()
{
}

void Roulette::Init(void)
{
	//������
	angle_ = 0.0;
	rotPower_ = 0.0;
	isStop_ = false;
	isRouSpin_ = true;
	totalTime_ = 0.0f;

	//�摜�o�^
	arrowImg_ = LoadGraph("./Data/Image/UI/arrow.png");
	rouletteImg_ = LoadGraph("./Data/Image/UI/roulette.png");
	rouFrame_ = LoadGraph("./Data/Image/UI/RouFrame.png");
	center_ = LoadGraph("./Data/Image/UI/center.png");

	//�t�H���g�̕ύX
	fontHandle_=CreateFontToHandle("@�l�r ����", ROULETTE_FONT_SIZE, ROULETTE_FONT_THICK);

	//SE�̓o�^
	auto& snd = SoundManager::GetInstance();
	rotateSE_ = snd.LoadSound(SoundManager::SOUND_TYPE::ROU_ROTATE);
	stopingSE_ = snd.LoadSound(SoundManager::SOUND_TYPE::ROU_STOPING);
	stopBtnSE_ = snd.LoadSound(SoundManager::SOUND_TYPE::ROU_STOP_BTN);

}

void Roulette::Update(void)
{
	//���[���b�g�̉�]����
	RotateProcess();
}

void Roulette::Draw(void)
{
	int cx = Application::SCREEN_SIZE_X;
	int cy = Application::SCREEN_SIZE_Y;


	//���`��
	DrawGraph(cx / 2+ ARROW_SHOW_POS.x, ARROW_SHOW_POS.y, arrowImg_, true);
	//���[���b�g���W
	int rouPosX = cx / 2+ ROULETTE_SHOW_POS.x;
	int rouPosY = ROULETTE_SHOW_POS.y;

	//���[���b�g�g�`��
	int x, y;
	GetGraphSize(rouFrame_, &x, &y);
	DrawGraph(rouPosX - x / 2, rouPosY - y / 2, rouFrame_, true);

	//�o�C���j�A��ԃ��[�h
	SetDrawMode(DX_DRAWMODE_BILINEAR);

	//���[���b�g�`��
	DrawRotaGraph(rouPosX, rouPosY,
		1.0f, Utility::Deg2RadF(angle_), rouletteImg_, true, false);

	//�e�R�}���h�Z�̉�]���A���Ή�]
	auto rotSpan = Utility::Deg2RadF(
		ONE_LAP / static_cast<float>(cmdNames_.size()));
	auto rotReltive = rotSpan / 2;

	//�R�}���h�Z�̉�]�`��
	int i = 0;
	for (auto& cmdName : cmdNames_)
	{
		auto name = cmdName->GetName().c_str();

		//�R�}���h���`��
		DrawRotaStringToHandle(
			rouPosX, rouPosY,		//���W
			1.0, 1.0,				//�g�嗦
			ROTATE_LOCAL_CENTER_POS.x, ROTATE_LOCAL_CENTER_POS.y,	//��]�̒��S���W�i���΍��W�j
			Utility::Deg2RadF(angle_) + (i * rotSpan) + rotReltive,	//�p�x
			CMD_NAME_COLOR, fontHandle_, 0x0,	//�t�H���g
			true, name);

		//�p�x�����炷
		i++;
	}


	//�l�A���X�g�l�C�o�[�@
	SetDrawMode(DX_DRAWMODE_NEAREST);

	//���[���b�g����
	GetGraphSize(center_, &x, &y);
	DrawGraph(rouPosX - x / 2, rouPosY - y / 2, center_, true);

}

void Roulette::Release(void)
{
	//�����̊J��
	StopSoundMem(rotateSE_);

	//�摜�̊J��
	DeleteGraph(arrowImg_);
	DeleteGraph(rouletteImg_);
	DeleteGraph(rouFrame_);
	DeleteGraph(center_);

	cmdNames_.clear();
}

void Roulette::StopRoulette(const bool& autom)
{
	if (!isRouSpin_)return;		//���Ƀu���[�L��ԂȂ�A���������Ȃ�

	bool stop = false;
	//�蓮�����������f����
	if (autom)
	{
		//���Ԃ�������u���[�L��Ԃɂ���
		stop = Utility::OverTime(totalTime_, STOP_WAIT_TIME_ROU);
	}
	else
	{
		//���N���b�N���m���A�u���[�L��Ԃɂ���
		auto& ins = InputManager::GetInstance();
		stop = ins.IsTrgMouseLeft();
	}

	if (stop)
	{
		//���[���b�g��~
		isRouSpin_ = false;

		//SE�Đ�
		auto& snd = SoundManager::GetInstance();
		snd.PlaySoundBack(stopingSE_);
		snd.PlaySoundBack(stopBtnSE_);
		//��]��SE�̒�~
		StopSoundMem(rotateSE_);
	}
}

void Roulette::SetCommand(std::vector<Command*> cmd)
{
	cmdNames_ = cmd;
}

void Roulette::ResetRouSpin(void)
{
	//angle = 0.0;		//���[���b�g�̉�]��Ԃ�߂��Ȃ�
	rotPower_ = 0.0;
	isStop_ = false;
	isRouSpin_ = true;
	totalTime_ = 0.0f;

	//��]SE�̍Đ�
	auto& snd = SoundManager::GetInstance();
	snd.PlaySoundBGM(rotateSE_);

}

Command* Roulette::GetCmd(void)
{
	//��]��
	auto rotAmount = fmod(angle_, ONE_LAP) * -1;
	//�R�}���h�̐�
	int num = static_cast<double>(cmdNames_.size());
	//���肳�ꂽ�R�}���h�𔻕ʂ���
	double angle = 0.0;
	double onwAngle = ONE_LAP / num;	//����̊p�x
	//�p�x����R�}���h���擾����
	for (size_t i = 0; i <= num; i++)
	{
		//�p�x�̈ȉ��ȏ�
		if (angle < rotAmount && rotAmount <= angle + onwAngle)
		{
			//�R�}���h��Ԃ�
			return cmdNames_[i];
		}

		//�p�x�����Z
		angle += onwAngle;
	}
}

void Roulette::RotateProcess(void)
{
	//��~�܂ł̃u���[�L��
	const float stopSpeed = -ROT_SPEED_MAX / ROT_BRAKE;

	//��]�ʂ̌v�Z�i��]���u���[�L�ő��x�ύX�j
	//��]��ԁF�ō����x�A�u���[�L��ԁF���������l
	rotPower_ = isRouSpin_ ? ROT_SPEED_MAX : rotPower_ + stopSpeed;

	//��~��Ԃ����f
	if (rotPower_ < ROT_STOP_SPEED)
	{
		//��]�ʂ�������
		angle_ += rotPower_;
	}
	else
	{
		//���[���b�g��~��Ԃɂ���
		isStop_ = true;
	}
}
