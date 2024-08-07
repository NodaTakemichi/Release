#pragma once
#include<DxLib.h>
#include "../Manager/ResourceManager.h"

class SoundController
{
public:
	enum class SOUND_MODE
	{
		SOUND_2D,
		SOUND_3D
	};

	enum class REVERB_TYPE
	{
		DEFAULT,	//デフォルト
		GENERIC,	//一般的な空間
		PADDEDCELL,	//精神病者室
		ROOM,		//部屋
		BATHROOM,	//バスルーム
		LIVINGROOM,	//リビングルーム
		SMALLROOM,	//小部屋
		MEDIUMROOM,	//中部屋
		LARGEROOM,	//大部屋
		PLATE		//板
	};

	SoundController(void);
	~SoundController(void);

	//データロード
	void DataLoad(const ResourceManager::SRC& soundSrouce, bool is3DSound = true);

	/// <summary>
	/// サウンドの初期化（2D）
	/// </summary>
	/// <param name="volume">音量</param>
	void Init(const int& volume);

	/// <summary>
	/// サウンドの初期化（3D）
	/// </summary>
	/// <param name="volume">音量</param>
	/// <param name="hearingArea">音が届く距離</param>
	/// <param name="pos">音源位置</param>
	/// <param name="type">リバーブエフェクトのタイプ</param>
	void Init(const int& volume ,const float& hearingArea,
		const VECTOR& pos, REVERB_TYPE type);
	void Update(void);
	void Draw(void);

	/// <summary>
	/// 音声再生（一回きり）
	/// </summary>
	void Play(void);

	/// <summary>
	/// 音声再生（ループ）
	/// </summary>
	void PlayLoop(void);

	/// <summary>
	/// 再生中でなければ、音声を再生する
	/// </summary>
	void PlayNonPlaying(void);

	/// <summary>
	/// 再生中でなければ、音声をループ再生する
	/// </summary>
	void PlayNonPlayingLoop(void);

	/// <summary>
	/// 音声停止
	/// </summary>
	void Stop(void);

	/// <summary>
	/// 再生中か判断
	/// </summary>
	/// <returns>true:再生中　false:再生中ではない</returns>
	bool CheckPlaySound(void);

	/// <summary>
	/// 音源位置の設定
	/// </summary>
	/// <param name="soundPos">音源座標</param>
	void SetSoundPos(const VECTOR& soundPos);	

	/// <summary>
	/// 音量の設定
	/// </summary>
	/// <param name="volume">音量</param>
	void SetVolume(const int& volume);

	/// <summary>
	/// 音が聞こえる範囲の設定
	/// </summary>
	/// <param name="area">音が聞こえる半径</param>
	void SetHearingArea(const float& area);

	/// <summary>
	/// 再生周波数の設定
	/// </summary>
	/// <param name="frequencyPal">設定する再生周波数</param>
	void SetSoundFrequency(const int& frequencyPal);

	/// <summary>
	/// 音量倍率の変更
	/// </summary>
	/// <param name="frequencyPal">音量倍率</param>
	void ChangeVolumeRate(const float& rate);


private:
	int soundHandle_;

	//音が聞こえる距離
	float hearingArea_;
	//音量
	int volume_;
	//座標
	VECTOR pos_;

	//リバーブエフェクトの選択
	const int SelectReverbType(const REVERB_TYPE& type);
	
};

