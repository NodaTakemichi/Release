#pragma once
class DeathStaging
{
public:
	// インスタンスの生成
	static void CreateInstance(void);

	// インスタンスの取得
	static DeathStaging& GetInstance(void);


	void Init(void);
	void Update(void);
	void Release(void);

	//死亡演出の再生
	void PlayDeathStaging(void);

	//死亡ユニットのセット
	void SetDeathUnit(const int& img,const float& r);

	//死亡演出の終了
	bool PlayingStaging(void);

	//死亡演出か始まったか判断
	bool CheckStartStanging(void);

private:
	// 静的インスタンス
	static DeathStaging* instance_;

	//死亡ユニット画像
	int unitImg_;
	//再生中か判断
	bool isPlaying_;
	bool isPlayingBefor_;

	//合計時間
	float totalTime_;
	//マスク画像の大きさ
	int mSizeX_;

	//マスク画像
	int maskImg_;
	//死亡フレーム画像
	int frameImg_;
	//死亡背景
	int backImg_;

	//死亡SE
	int deathSE_;

	//マスクスクリーン
	int maskScreen_;
	//死亡演出描画スクリーン
	int deathStagingScreen_;

	//画面反転
	float revers_;


	DeathStaging();
	~DeathStaging();


};

