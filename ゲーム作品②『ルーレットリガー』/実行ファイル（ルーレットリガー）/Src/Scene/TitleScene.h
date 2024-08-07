#pragma once
#include <vector>
#include "SceneBase.h"
class TitleScene :
    public SceneBase
{
public:

	// コンストラクタ
	TitleScene(void);

	// デストラクタ
	~TitleScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;


private:


	//タイトル画像
	int titleImg_;
	int titleImgNor_;
	//タイトルバック画像
	int titleBackImg_;

	//モンスター画像
	std::vector<int> unitsImg_;

	//フォントハンドル
	int fontHandle_;
	int fontColor_;

	//描画画面
	int uiScreen_;

	//UI描画
	void DrawUI(void);

};

