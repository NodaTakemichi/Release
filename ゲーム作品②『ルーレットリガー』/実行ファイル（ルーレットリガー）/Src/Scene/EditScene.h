#pragma once
#include<vector>
#include<array>
#include<map>
#include<unordered_map>
#include "SceneBase.h"
#include "../Manager/DataManager/UnitDataManager.h"
#include "../Common/Vector2.h"

class CircleButton;
class RectButton;
class UnitButton;
class CmdButton;

class EditScene :
	public SceneBase
{
public:


	// コンストラクタ
	EditScene(void);

	// デストラクタ
	~EditScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;


private:
	//マイデッキボタン
	std::unordered_map<int, UnitButton*> myDeckBtn_;

	//所持ユニットカードボタン
	std::unordered_map<int, UnitButton*> unitCards_;
	//所持ユニット番号
	std::vector<int> unitCardNums_;

	//ピックアップユニットデータ
	UnitData pickUpUnitData_;

	//コマンドボタン
	std::vector<CmdButton*> cmdBtns_;


	//バックボタン
	CircleButton* backBtn_;
	//ボタン画像
	int backImg_;

	//ページ更新ボタン　（０：前ページへ、１：次ページへ）
	std::array<RectButton*, 2> pageBtn_;
	//次：三角画像
	int nextPB_;
	//前：三角画像
	int backPB_;


	//ユニットカードの背面画像
	int unitBackImg_;

	//HP、攻撃力、スピードアイコン
	int hpIcon_;
	int attackIcon_;
	int speedIcon_;

	//ピックアップユニット
	int pickUpUnit_;
	//ステータスフォントハンドル
	int sFontHandle_;
	int sFontColor_;

	//ページ数
	int page_;

	//デッキユニット、所持ユニットの初期化
	void InitMyUnit(void);

	//ステータス画面のコマンド生成
	void CreateCmd(void);

	//カードステータスの描画
	void DrawUnitStatus(void);

	//デッキ編集
	void DeckEditProcess(void);

	//ピックアップユニットの選択
	void SelectPickUpUnit(void);

	//デッキの確定
	void DeckDecision(void);

	//ページボタン更新処理
	void PageBtnProcess(void);

	//デッキプールの表示ページの変更
	void ChangeDeckPoolPage(void);

};