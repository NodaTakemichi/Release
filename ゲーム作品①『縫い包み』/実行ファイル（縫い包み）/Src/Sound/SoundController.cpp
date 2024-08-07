#include "../Manager/SoundManager.h"
#include "SoundController.h"

SoundController::SoundController(void)
{
	soundHandle_ = 0;
	hearingArea_ = 0.0f;
	volume_ = 0;
	pos_= VECTOR();

}

SoundController::~SoundController(void)
{
}

void SoundController::DataLoad(const ResourceManager::SRC& soundSrouce, bool is3DSound)
{
	auto& res = ResourceManager::GetInstance();

	SetCreate3DSoundFlag(is3DSound);
	soundHandle_ = res.Load(soundSrouce).handleId_;
	SetCreate3DSoundFlag(false);

}

void SoundController::Init(const int& volume)
{
	volume_ = volume;
	//音量設定
	ChangeVolumeSoundMem(volume_, soundHandle_);

	//マネージャーに追加
	auto& sndMng = SoundManager::GetInstance();
	sndMng.AddSoundController(std::make_shared<SoundController>(*this));

}

void SoundController::Init(const int& volume, const float& hearingArea, const VECTOR& pos, REVERB_TYPE type)
{
	pos_ = pos;
	hearingArea_ = hearingArea;


	// 音を鳴らす位置を設定する
	Set3DPositionSoundMem(pos_, soundHandle_);
	//音が聞こえる距離を設定する
	Set3DRadiusSoundMem(hearingArea_, soundHandle_);
	//音データのリバーブエフェクトの設定
	Set3DPresetReverbParamSoundMem(
		SelectReverbType(type), soundHandle_);

	Init(volume);
}


void SoundController::Play(void)
{
	//音声の再生
	PlaySoundMem(soundHandle_, DX_PLAYTYPE_BACK);
}

void SoundController::PlayLoop(void)
{
	//音声の再生
	PlaySoundMem(soundHandle_, DX_PLAYTYPE_LOOP);
}

void SoundController::PlayNonPlaying(void)
{
	if (!CheckPlaySound())
	{
		Play();
	}
}

void SoundController::PlayNonPlayingLoop(void)
{
	if (!CheckPlaySound())
	{
		PlayLoop();
	}
}

void SoundController::Stop(void)
{
	StopSoundMem(soundHandle_);
}

bool SoundController::CheckPlaySound(void)
{
	return CheckSoundMem(soundHandle_);
}

void SoundController::Update(void)
{

}

void SoundController::Draw(void)
{
#ifdef _DEBUG
	DrawSphere3D(pos_, hearingArea_, 5, 0xaaaaff, 0xaaaaff, true);
#endif // DEBUG

}

void SoundController::SetSoundPos(const VECTOR& soundPos)
{
	//音源位置更新
 	pos_ = soundPos;
	Set3DPositionSoundMem(pos_, soundHandle_);
}

void SoundController::SetVolume(const int& volume)
{
	volume_ = volume;
	ChangeVolumeSoundMem(volume_, soundHandle_);
}

void SoundController::SetHearingArea(const float& area)
{
	hearingArea_ = area;
	Set3DRadiusSoundMem(hearingArea_, soundHandle_);
}

void SoundController::SetSoundFrequency(const int& frequencyPal)
{
	SetFrequencySoundMem(frequencyPal, soundHandle_);
}

void SoundController::ChangeVolumeRate(const float& rate)
{
	ChangeVolumeSoundMem(static_cast<int>(volume_ * rate), soundHandle_);
}

const int SoundController::SelectReverbType(const REVERB_TYPE& type)
{
	int presetNo = DX_REVERB_PRESET_DEFAULT;

	switch (type)
	{
	case REVERB_TYPE::DEFAULT:
		presetNo = DX_REVERB_PRESET_DEFAULT;
		break;
	case REVERB_TYPE::GENERIC:
		presetNo = DX_REVERB_PRESET_GENERIC;
		break;
	case REVERB_TYPE::PADDEDCELL:
		presetNo = DX_REVERB_PRESET_PADDEDCELL;
		break;
	case REVERB_TYPE::ROOM:
		presetNo = DX_REVERB_PRESET_ROOM;
		break;
	case REVERB_TYPE::BATHROOM:
		presetNo = DX_REVERB_PRESET_BATHROOM;
		break;
	case REVERB_TYPE::LIVINGROOM:
		presetNo = DX_REVERB_PRESET_LIVINGROOM;
		break;
	case REVERB_TYPE::SMALLROOM:
		presetNo = DX_REVERB_PRESET_SMALLROOM;
		break;
	case REVERB_TYPE::MEDIUMROOM:
		presetNo = DX_REVERB_PRESET_MEDIUMROOM;
		break;
	case REVERB_TYPE::LARGEROOM:
		presetNo = DX_REVERB_PRESET_LARGEROOM;
		break;
	case REVERB_TYPE::PLATE:
		presetNo = DX_REVERB_PRESET_PLATE;
		break;
	}

	return presetNo;
}

