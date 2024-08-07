#pragma once
#include<vector>
#include<array>
#include"../Common/Deck.h"

class DeckManager
{
public:
	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static DeckManager& GetInstance(void);

	// 初期化
	void Init(void);

	// 解放
	void Release(void);

	//デッキ情報の取得
	const Deck& GetDeck(void) { return deck_; }
	//ユニットプール情報の取得
	const std::vector<int>& GetUnitPool(void) { return unitPool_; }

	//デッキ情報のセット
	void SetDeck(const Deck& deck);

	//ユニットの追加
	void AddUnit(const int& unitNum);

private:
	// 静的インスタンス
	static DeckManager* instance_;

	//デッキカード
	Deck deck_;

	//ユニットプール
	std::vector<int>unitPool_;


	// 外部から生成できない様にする
	DeckManager(void);
	// デストラクタも同様
	~DeckManager(void);

};

