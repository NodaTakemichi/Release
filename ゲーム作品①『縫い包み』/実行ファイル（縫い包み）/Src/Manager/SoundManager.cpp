#include "../Sound/SoundController.h"
#include "SoundManager.h"


void SoundManager::Init(void)
{
	volumeRate_ = 1.0f;
}

void SoundManager::Update(void)
{
}

void SoundManager::AddSoundController(std::shared_ptr<SoundController> sound)
{
	sounds_.emplace_back(sound);
}

void SoundManager::StopAllSound(void)
{
	for (const auto& sound : sounds_)
	{
		sound->Stop();
	}
}

void SoundManager::ClearSpounds(void)
{
	sounds_.clear();
}

void SoundManager::ChangeMasterVolume(const float& rate)
{
	for (const auto& sound : sounds_)
	{
		sound->ChangeVolumeRate(volumeRate_);
	}
}

SoundManager::SoundManager(void)
{
	volumeRate_=1.0f;
}

SoundManager::~SoundManager(void)
{
}
