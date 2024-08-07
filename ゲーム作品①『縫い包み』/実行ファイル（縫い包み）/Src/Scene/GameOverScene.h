#pragma once
#include "SceneBase.h"
class PostEffectMaterial;
class PostEffectRenderer;
class Button;




class GameOverScene :
    public SceneBase
{
public:

	GameOverScene(SceneManager& manager);
	~GameOverScene(void);

	virtual void LoadData(void)override;
	virtual void Init(void) override;
	virtual void Update(InputManager& input) override;
	virtual void Draw(void) override;

private:
	void BeginUpdate(InputManager& input);	//GameOver表示までの更新
	void NormalUpdate(InputManager& input);	//通常更新
	void EndUpdate(InputManager& input);	//GameOver解除までの更新

	void BeginDraw(void);	//GameOver表示までの描画
	void NormalDraw(void);	//通常描画
	void EndDraw(void);		//GameOver解除までの描画

	float frameTime_ = 0.0f;

	//背景画像
	int bgImg_;

	//ノイズ
	std::shared_ptr<SoundController>noiseSE_;


	// ポストエフェクト
	std::unique_ptr<PostEffectMaterial> postEffectMaterial_;
	std::unique_ptr<PostEffectRenderer> postEffectRenderer_;

	//選択ボタン
	std::vector<Button>buttons_;

	//各ボタン処理
	void RestartBtnProcess(void);
	void ExitBtnProcess(void);
	void TitleBtnProcess(void);
};

