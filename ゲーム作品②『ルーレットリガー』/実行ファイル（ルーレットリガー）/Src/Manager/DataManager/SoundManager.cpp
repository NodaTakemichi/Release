#include <DxLib.h>
#include "SoundManager.h"
//�x�[�X�{�����[��
constexpr int BASE_VOLUME = 120;

SoundManager* SoundManager::instance_ = nullptr;

void SoundManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new SoundManager();
	}
	instance_->Init();
}

SoundManager& SoundManager::GetInstance(void)
{
	return *instance_;
}

void SoundManager::Init(void)
{
	std::string res;
	//BGM
	std::string pathSound = "Data/Sound/BGM/";

	// �^�C�g�����BGM
	res = pathSound + "Title.mp3";
	resMap_.insert(std::make_pair(SOUND_TYPE::TITLE_BGM, res));

	//�Z���N�g���GM
	res = pathSound + "Select.mp3";
	resMap_.insert(std::make_pair(SOUND_TYPE::SELECT_BGM, res));

	// �o�g���PBGM
	res = pathSound + "Battle1.mp3";
	resMap_.insert(std::make_pair(SOUND_TYPE::BATTLE1_BGN, res));

	// �o�g���QBGM
	res = pathSound + "BAttle2.mp3";
	resMap_.insert(std::make_pair(SOUND_TYPE::BATTLE2_BGN, res));

	// �o�g���RBGM
	res = pathSound + "Battle3.mp3";
	resMap_.insert(std::make_pair(SOUND_TYPE::BATTLE3_BGN, res));

	// ����BGM
	res = pathSound + "Victory.mp3";
	resMap_.insert(std::make_pair(SOUND_TYPE::VICTORY_BGN, res));

	// �s�kBGM
	res = pathSound + "Lose.mp3";
	resMap_.insert(std::make_pair(SOUND_TYPE::LOSE_BGN, res));

	// ���U���g���BGM
	res = pathSound + "Result.mp3";
	resMap_.insert(std::make_pair(SOUND_TYPE::RESULT_BGN, res));

	// �ҏW���BGM
	res = pathSound + "Edit.mp3";
	resMap_.insert(std::make_pair(SOUND_TYPE::EDIT_BGM, res));



	//SE
	pathSound = "Data/Sound/SE/";
	//�V�[���J��SE
	res = pathSound + "SceneMove.mp3";
	resMap_.insert(std::make_pair(SOUND_TYPE::SCENE_MOVE, res));

	//�{�^������SE
	res = pathSound + "BtnClick.mp3";
	resMap_.insert(std::make_pair(SOUND_TYPE::BTN_CLICK, res));

	//�{�^����SE
	res = pathSound + "BtnOn.mp3";
	resMap_.insert(std::make_pair(SOUND_TYPE::BTN_ON, res));

	//�߂�{�^��SE
	res = pathSound + "BackBtn.mp3";
	resMap_.insert(std::make_pair(SOUND_TYPE::BTN_BACK, res));

	//�X�N���[��SE
	res = pathSound + "Scroll.mp3";
	resMap_.insert(std::make_pair(SOUND_TYPE::SCROLL, res));


	//�o�g��SE
	pathSound = "Data/Sound/SE/Battle/";
	//�_���[�WSE
	res = pathSound + "Damage.mp3";
	resMap_.insert(std::make_pair(SOUND_TYPE::DAMAGE, res));
	//��SE
	res = pathSound + "Heal.mp3";
	resMap_.insert(std::make_pair(SOUND_TYPE::HEAL, res));
	//���SSE
	res = pathSound + "Death.mp3";
	resMap_.insert(std::make_pair(SOUND_TYPE::DEATH, res));
	//�^�[�����SE
	res = pathSound + "TurnChange.mp3";
	resMap_.insert(std::make_pair(SOUND_TYPE::TURN_CHANGE, res));
	//�ΏۑI��SE
	res = pathSound + "Target_Select.mp3";
	resMap_.insert(std::make_pair(SOUND_TYPE::TARGET_SELECT, res));
	//�Ώی���SE
	res = pathSound + ".mp3";
	resMap_.insert(std::make_pair(SOUND_TYPE::TARGET_DEC, res));

	//�o�tSE
	pathSound = "Data/Sound/SE/Battle/Buff/";
	//�X�e�[�^�X�A�b�vSE
	res = pathSound + "StatusUp.mp3";
	resMap_.insert(std::make_pair(SOUND_TYPE::STATUS_UP, res));
	//�X�e�[�^�X�_�E��SE
	res = pathSound + "StatusDown.mp3";
	resMap_.insert(std::make_pair(SOUND_TYPE::STATUS_DOWN, res));
	//��SE
	res = pathSound + "Poison.mp3";
	resMap_.insert(std::make_pair(SOUND_TYPE::POISON, res));
	//���SE
	res = pathSound + "Para.mp3";
	resMap_.insert(std::make_pair(SOUND_TYPE::PARA, res));
	//���SE
	res = pathSound + "Avoi.mp3";
	resMap_.insert(std::make_pair(SOUND_TYPE::AVOI, res));

	//���[���b�gSE
	pathSound = "Data/Sound/SE/Battle/Roulette/";
	//���[���b�g��]SE
	res = pathSound + "RouletteRotate.mp3";
	resMap_.insert(std::make_pair(SOUND_TYPE::ROU_ROTATE, res));
	//���[���b�g��~��SE
	res = pathSound + "RouletteStoping.mp3";
	resMap_.insert(std::make_pair(SOUND_TYPE::ROU_STOPING, res));
	//���[���b�g��~�{�^��SE
	res = pathSound + "RouletteStopBtn.mp3";
	resMap_.insert(std::make_pair(SOUND_TYPE::ROU_STOP_BTN, res));

}

void SoundManager::Release(void)
{
	InitSoundMem();

	resMap_.clear();
	loadedMap_.clear();
}

int SoundManager::LoadSound(SOUND_TYPE sound)
{
	const auto& lPair = loadedMap_.find(sound);
	if (lPair != loadedMap_.end())
	{
		//�o�^����Ă���
		return lPair->second;
	}

	const auto& rPair = resMap_.find(sound);
	if (rPair == resMap_.end())
	{
		// ���݂��Ȃ�
		return -1;
	}

	//�o�^����Ă��Ȃ��ꍇ
	int handle = LoadSoundMem(rPair->second.c_str());
	//����
	ChangeVolumeSoundMem(BASE_VOLUME, handle);

	//�ǉ��o�^
	loadedMap_.insert(std::make_pair(sound, handle));

	return handle;
}

void SoundManager::PlaySoundNormal(const int& handle)
{
	PlaySoundMem(handle, DX_PLAYTYPE_NORMAL);
}

void SoundManager::PlaySoundBack(const int& handle)
{
	PlaySoundMem(handle, DX_PLAYTYPE_BACK);
}

void SoundManager::PlaySoundBGM(const int& handle)
{
	PlaySoundMem(handle, DX_PLAYTYPE_LOOP);
}

void SoundManager::StopSound(void)
{
	StopSoundFile();
}

SoundManager::SoundManager(void)
{
}

SoundManager::~SoundManager(void)
{
	delete instance_;
}

