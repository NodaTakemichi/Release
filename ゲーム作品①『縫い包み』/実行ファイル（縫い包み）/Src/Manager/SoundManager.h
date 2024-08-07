#pragma once
#include<DxLib.h>
#include<memory>
#include<vector>
#include "../Utility/StaticSingleton.h"
class SoundController;

class SoundManager:
	public StaticSingleton<SoundManager>
{
public:

	void Init(void);
	void Update(void);

	void AddSoundController(std::shared_ptr<SoundController> sound);

	//全音源の再生ストップ
	void StopAllSound(void);

	//音声データをクリアする
	void ClearSpounds(void);

	//マスターボリュームの変更
	void ChangeMasterVolume(const float& rate);


private:
	//シングルトン
	THIS_CLASS_IS_STATIC_SINGLETON(SoundManager);

	//サウンド管理
	std::vector<std::shared_ptr<SoundController>> sounds_;
	
	//音量倍率
	float volumeRate_;

	SoundManager(void);
	~SoundManager(void);


};

