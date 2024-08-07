#pragma once
#include<vector>
#include<array>
#include"../../Common/Deck.h"


//バトルデータ
struct BattleData {
	Deck pDeck;	//プレイヤーデッキ
	Deck eDeck;	//エネミーデッキ
	int bgNum;					//背景画像ナンバー
	int bgmNum;					//BGM番号
};

class BattleDataManager
{
public:
	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static BattleDataManager& GetInstance(void);

	// 初期化
	void Init(void);

	// 解放
	void Release(void);

	//バトルデータのゲット、セット
	const  BattleData& GetBattleData(void) { return bData_; }
	void SetBattleData(const BattleData& data);

private:
	// 静的インスタンス
	static BattleDataManager* instance_;

	//バトルデータ
	BattleData bData_;


	// 外部から生成できない様にする
	BattleDataManager(void);
	// デストラクタも同様
	~BattleDataManager(void);


};

