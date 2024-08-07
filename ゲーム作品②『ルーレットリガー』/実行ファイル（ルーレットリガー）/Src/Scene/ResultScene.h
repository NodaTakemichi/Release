#pragma once
#include<vector>
#include "SceneBase.h"
class Vector2;
class UnitButton;

class ResultScene :
    public SceneBase
{
public:
	// コンストラクタ
	ResultScene(void);

	// デストラクタ
	~ResultScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	//フォントハンドル
	int fontHandle_;

	//使い魔画像、座標
	int devilImg_;
	Vector2 devilPos_;
	//コマンドボックス画像
	int cmtBoxImg_;

	//加入ユニットボタン
	std::vector<UnitButton*>unitBtns_;

	//ユニットカード背面
	int unitBackImg_;

	//加入ユニットのボタン生成
	void CreateUnitBtn(void);

	//加入ユニットを描画する
	void DrawGetUnits(void);

};

