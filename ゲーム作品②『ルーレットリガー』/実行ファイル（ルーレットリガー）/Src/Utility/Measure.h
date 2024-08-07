#pragma once
#include <chrono>
#include <string>
#include <map>
#include <vector>

class Measure
{

public:

	// 無効か
	static constexpr bool IS_DISABLE = true;

	// 平均を取る直近フレーム数
	static constexpr int AVG_FRAME = 180;

	// インスタンスを明示的に生成
	static void CreateInstance(void);

	// インスタンスの取得
	static Measure& GetInstance(void);

	void Init(void);

	// 計測開始
	void Start(void);

	// 計測
	void Watch(std::string name);

	// 描画
	void Draw(void);

	// リセット
	void Reset(void);

private:

	// シングルトン用インスタンス
	static Measure* instance_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	Measure(void);
	~Measure(void);

	// 計測開始
	std::chrono::system_clock::time_point startTime_;

	// １フレーム前の時間
	std::chrono::system_clock::time_point preFrameTime_;

	// 各計測箇所の計測時間
	std::map<std::string, double> watchTimes_;

	// インスタンス生成時からの経過時間
	std::chrono::system_clock::time_point initTime_;
	double totalTime_;

	// フレームカウント
	int cntTotalFrame_;

	// 直近180フレーム
	std::vector<double> average_;

};

