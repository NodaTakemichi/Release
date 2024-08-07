#include <DxLib.h>
#include "SoundManager.h"
//ベースボリューム
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

	// タイトル画面BGM
	res = pathSound + "Title.mp3";
	resMap_.insert(std::make_pair(SOUND_TYPE::TITLE_BGM, res));

	//セレクト画面GM
	res = pathSound + "Select.mp3";
	resMap_.insert(std::make_pair(SOUND_TYPE::SELECT_BGM, res));

	// バトル１BGM
	res = pathSound + "Battle1.mp3";
	resMap_.insert(std::make_pair(SOUND_TYPE::BATTLE1_BGN, res));

	// バトル２BGM
	res = pathSound + "BAttle2.mp3";
	resMap_.insert(std::make_pair(SOUND_TYPE::BATTLE2_BGN, res));

	// バトル３BGM
	res = pathSound + "Battle3.mp3";
	resMap_.insert(std::make_pair(SOUND_TYPE::BATTLE3_BGN, res));

	// 勝利BGM
	res = pathSound + "Victory.mp3";
	resMap_.insert(std::make_pair(SOUND_TYPE::VICTORY_BGN, res));

	// 敗北BGM
	res = pathSound + "Lose.mp3";
	resMap_.insert(std::make_pair(SOUND_TYPE::LOSE_BGN, res));

	// リザルト画面BGM
	res = pathSound + "Result.mp3";
	resMap_.insert(std::make_pair(SOUND_TYPE::RESULT_BGN, res));

	// 編集画面BGM
	res = pathSound + "Edit.mp3";
	resMap_.insert(std::make_pair(SOUND_TYPE::EDIT_BGM, res));



	//SE
	pathSound = "Data/Sound/SE/";
	//シーン遷移SE
	res = pathSound + "SceneMove.mp3";
	resMap_.insert(std::make_pair(SOUND_TYPE::SCENE_MOVE, res));

	//ボタン決定SE
	res = pathSound + "BtnClick.mp3";
	resMap_.insert(std::make_pair(SOUND_TYPE::BTN_CLICK, res));

	//ボタン上SE
	res = pathSound + "BtnOn.mp3";
	resMap_.insert(std::make_pair(SOUND_TYPE::BTN_ON, res));

	//戻るボタンSE
	res = pathSound + "BackBtn.mp3";
	resMap_.insert(std::make_pair(SOUND_TYPE::BTN_BACK, res));

	//スクロールSE
	res = pathSound + "Scroll.mp3";
	resMap_.insert(std::make_pair(SOUND_TYPE::SCROLL, res));


	//バトルSE
	pathSound = "Data/Sound/SE/Battle/";
	//ダメージSE
	res = pathSound + "Damage.mp3";
	resMap_.insert(std::make_pair(SOUND_TYPE::DAMAGE, res));
	//回復SE
	res = pathSound + "Heal.mp3";
	resMap_.insert(std::make_pair(SOUND_TYPE::HEAL, res));
	//死亡SE
	res = pathSound + "Death.mp3";
	resMap_.insert(std::make_pair(SOUND_TYPE::DEATH, res));
	//ターン交代SE
	res = pathSound + "TurnChange.mp3";
	resMap_.insert(std::make_pair(SOUND_TYPE::TURN_CHANGE, res));
	//対象選択SE
	res = pathSound + "Target_Select.mp3";
	resMap_.insert(std::make_pair(SOUND_TYPE::TARGET_SELECT, res));
	//対象決定SE
	res = pathSound + ".mp3";
	resMap_.insert(std::make_pair(SOUND_TYPE::TARGET_DEC, res));

	//バフSE
	pathSound = "Data/Sound/SE/Battle/Buff/";
	//ステータスアップSE
	res = pathSound + "StatusUp.mp3";
	resMap_.insert(std::make_pair(SOUND_TYPE::STATUS_UP, res));
	//ステータスダウンSE
	res = pathSound + "StatusDown.mp3";
	resMap_.insert(std::make_pair(SOUND_TYPE::STATUS_DOWN, res));
	//毒SE
	res = pathSound + "Poison.mp3";
	resMap_.insert(std::make_pair(SOUND_TYPE::POISON, res));
	//麻痺SE
	res = pathSound + "Para.mp3";
	resMap_.insert(std::make_pair(SOUND_TYPE::PARA, res));
	//回避SE
	res = pathSound + "Avoi.mp3";
	resMap_.insert(std::make_pair(SOUND_TYPE::AVOI, res));

	//ルーレットSE
	pathSound = "Data/Sound/SE/Battle/Roulette/";
	//ルーレット回転SE
	res = pathSound + "RouletteRotate.mp3";
	resMap_.insert(std::make_pair(SOUND_TYPE::ROU_ROTATE, res));
	//ルーレット停止中SE
	res = pathSound + "RouletteStoping.mp3";
	resMap_.insert(std::make_pair(SOUND_TYPE::ROU_STOPING, res));
	//ルーレット停止ボタンSE
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
		//登録されている
		return lPair->second;
	}

	const auto& rPair = resMap_.find(sound);
	if (rPair == resMap_.end())
	{
		// 存在しない
		return -1;
	}

	//登録されていない場合
	int handle = LoadSoundMem(rPair->second.c_str());
	//音量
	ChangeVolumeSoundMem(BASE_VOLUME, handle);

	//追加登録
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

