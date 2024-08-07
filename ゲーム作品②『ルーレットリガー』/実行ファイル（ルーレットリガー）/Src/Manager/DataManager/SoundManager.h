#pragma once
#include <unordered_map>
#include <string>

class SoundManager
{
public:
	// サウンド名
	enum class SOUND_TYPE
	{
		TITLE_BGM,
		SELECT_BGM,
		BATTLE1_BGN,
		BATTLE2_BGN,
		BATTLE3_BGN,
		VICTORY_BGN,
		LOSE_BGN,
		RESULT_BGN,
		EDIT_BGM,


		SCENE_MOVE,
		BTN_CLICK,
		BTN_ON,
		BTN_BACK,
		SCROLL,

		DAMAGE,
		DEATH,
		HEAL,
		TURN_CHANGE,
		TARGET_SELECT,
		TARGET_DEC,

		STATUS_UP,
		STATUS_DOWN,
		POISON,
		PARA,
		AVOI,

		ROU_ROTATE,
		ROU_STOPING,
		ROU_STOP_BTN,


	};

	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static SoundManager& GetInstance(void);

	// 初期化
	void Init(void);

	// 解放
	void Release(void);

	// 音声のロード
	int LoadSound(SOUND_TYPE sound);

	//音声の再生（ノーマル）
	void PlaySoundNormal(const int& sound);
	//音声の再生（バックグランド）
	void PlaySoundBack(const int& sound);
	//音声の再生（BGM）
	void PlaySoundBGM(const int& sound);

	//音ファイルの再生を停止
	void StopSound(void);

private:

	// 静的インスタンス
	static SoundManager* instance_;

	// リソース管理の対象
	std::unordered_map<SOUND_TYPE, std::string> resMap_;

	// 読み込み済みリソース
	std::unordered_map<SOUND_TYPE, int> loadedMap_;

	// 外部から生成できない様にする
	SoundManager(void);
	// デストラクタも同様
	~SoundManager(void);

	//マスターボリューム
	int masterVolume_;
	//BGMボリューム
	int bgmVolume_;
	//SEボリューム
	int seVolume_;


};

