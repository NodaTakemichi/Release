#pragma once
#include<vector>
#include<array>
#include<unordered_map>
#include "../Common/Vector2.h"
#include "../Common/Deck.h"
#include "SceneBase.h"

class RectButton;
class CircleButton;

struct ModeData {
	int modeImg;		//モード選択画像
	RectButton* btn;	//モードボタン
};

struct StageData {
	int backImg;				//背景画像
	std::vector<Deck>enemys;	//エネミーズ
};

class SelectScene :
    public SceneBase
{
public:

	enum class SELECT_MODE
	{
		BATTLE	  = 0,
		DECK_EDIT = 1,
		RULE_BOOK = 2,
		CREDIT	  = 3,
		TITLE	  = 4,
		MAX		  
	};

	enum class SELECT_STAGE
	{
		VOLCANO = 0,
		OCEAN	= 1,
		WILDRNESS	= 2,
		FOREST		= 3,
		SANCTUARY	= 4,
		ABYSS		= 5,
		MAX
	};

	enum class SELECT_PAGE
	{
		MODE = 0,
		STAGE	= 1,
		ENEMY	= 2,
		MAX
	};

	// コンストラクタ
	SelectScene(void);

	// デストラクタ
	~SelectScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:
	//バックボタン
	CircleButton* backBtn_;
	//ボタン画像
	int backImg_;

	//決定ボタン
	RectButton* deciBtn_;
	//ボタン画像
	int deciBtnImg_;

	//選択画面の進行
	SELECT_PAGE page_;

	//バトル相手情報
	std::vector<Deck>enemys_;
	//現エネミーズ番号
	int nowEnemysNum_;
	//ステージ
	std::unordered_map<SELECT_STAGE, StageData>stages_;
	//現在ステージ番号
	int nowStageNum_;


	//選択ボックス画像
	int selectedBoxImg_;
	//選択ボックスのマスク画像
	int selectedBoxMaskImg_;
	//選択用マスクスクリーン
	int selectMaskScreen_;
	//選択画像座標
	Vector2 selectImgPos_;

	//スクロール経過時間
	float scrollTime_;
	//ホイール動作判断（スクロール中は反応しない）
	bool isMoveWheel_;
	//ホイールのスクロール方向
	int wheelSign_;



	//現在モード
	SELECT_MODE mode_;
	//モードボタン
	std::map<SELECT_MODE, ModeData>modeBtn_;
	//モードボタン画像
	int modeBackBtnImg_;

	//モードボックス画像
	Vector2 modePos_;
	std::string nowMode_;
	int modeBox_;
	int modeFontHandle_;

	//コメントボックス
	Vector2 cmtPos_;
	std::string devilCmt_;
	int cmtBox_;
	int cmtFontHandle_;
	//マスコット画像
	int devilImg_;
	Vector2 devilPos_;
	int shakeY_;

	//音声関連
	int scrollSE_;		//スクロールSE


	//モードボックス描画
	void DrawModeBox(void);
	//コメントボックス描画
	void DrawCmtBox(void);


	//ボタン処理
	void BtnProcess();
	//各ボタン処理
	void BattleBtnProcess(void);
	void EditBtnProcess(void);
	void TitleBtnProcess(void);

	//モードボタン生成
	void CreateModeBtn(void);
	//モードの変更
	void ChangeSelectMode(const SELECT_MODE& mode);

	//クリックしたボタンの処理
	void SelectBtnProcess(void);

	//マスコットのコメントセット
	void SetDevilCmt(const std::string& cmt);

	//ステージ情報の登録
	void StageDataInit();

	//選択スクロール操作
	void SelectSceollProcess(void);
	//選択画像スライド移動
	void ScrollSelectImg(int& num, const int& size);

	//選択ステージの描画（選択ボックス）
	void DrawSelectBoxStage(void);
	//選択エネミーの描画（選択ボックス）
	void DrawSelectBoxEnemy(void);

};

