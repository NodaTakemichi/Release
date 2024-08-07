#pragma once
#include<vector>
#include<utility>
#include<string>
#include<unordered_map>
#include"../../Common/Vector2.h"


struct EffectData
{
	//int num;			//エフェクトナンバー
	std::string path;	//エフェクトパス
	float scale;		//大きさ
	Vector2 offset;		//オフセット座標
	int target;			//0:全体エフェクト　,1:単体エフェクト
	int handle;			//エフェクトハンドル
};

class EffectManager
{
public:

	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static EffectManager& GetInstance(void);


	void Init(void);
	void Release(void);

	//登録エフェクトの解放
	void LoadedEffectRelease(void);

	//エフェクト情報のロード
	std::string EffectDataLoad(std::string fileName);

	//使用エフェクトのロード
	void EffectLoad(const int& num);

	//エフェクトの生成
	void PlayEffect(const int& num,const Vector2& pos);

	//エフェクトが終了しているかどうか
	bool FinishEffect(void);

private:
	// 静的インスタンス
	static EffectManager* instance_;


	//リソース管理マップ
	std::unordered_map<int, EffectData>resMap_;
	//ロード済みマップ
	std::unordered_map<int, EffectData>loadedMap_;


	//エフェクト再生ハンドル（bool ：使用未使用）
	std::vector<std::pair<bool,int>> effectPlays_;

	//再生ハンドルの追加
	void CreatePlayHandle(void);

	//全体攻撃エフェクトの再生
	bool AllTargetEffectPlay(const EffectData& ef, const Vector2& pos);

	EffectManager();
	~EffectManager();

};

