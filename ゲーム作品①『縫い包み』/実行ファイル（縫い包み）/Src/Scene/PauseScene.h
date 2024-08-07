#pragma once
#include "SceneBase.h"


class Button;
class PauseScene :
    public SceneBase
{
public:
	PauseScene(SceneManager& manager);
	~PauseScene(void);

	virtual void LoadData(void)override;	
	virtual void Init(void) override;
	virtual void Update(InputManager& input) override;
	virtual void Draw(void) override;


private:


	void DefaultUpdate(InputManager& input);	//何もしない
	void BeginUpdate(InputManager& input);	//ポーズ表示までの更新
	void NormalUpdate(InputManager& input);	//通常更新
	void EndUpdate(InputManager& input);	//ポーズ解除までの更新

	void BeginDraw(void);	//ポーズ表示までの描画
	void NormalDraw(void);	//通常描画
	void EndDraw(void);		//ポーズ解除までの描画

	float frameTime_ = 0.0f;

	//ポーズ背景画像
	int pauseBackImg_;

	//選択ボタン
	std::vector<Button>buttons_;

	//各ボタン処理
	void RuleBtnProcess(void);
	void TitleBtnProcess(void);
	void ExitBtnProcess(void);
	void CloseBtnProcess(void);

};

