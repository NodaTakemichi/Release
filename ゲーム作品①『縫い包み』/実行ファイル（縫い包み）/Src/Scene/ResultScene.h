#pragma once
#include "SceneBase.h"
class Button;
class ModelMaterial;
class ModelRenderer;

class ResultScene :
    public SceneBase
{
public:

	ResultScene(SceneManager& manager);
	~ResultScene(void);


	virtual void LoadData(void)override;
	virtual void Init(void) override;
	virtual void Update(InputManager& input) override;
	virtual void Draw(void) override;

private:

	//各ボタン処理
	void TitleButtnProcess(void);	//タイトル遷移ボタン
	void ExitButtnProcess(void);	//終了ボタン

	//Update関数
	void LoadingUpdate(InputManager& input);
	void NormalUpdate(InputManager& input);
	//描画関数
	void LoadingDraw(void);
	void NormalDraw(void);

	//選択ボタン
	std::vector<Button>buttons_;

	//リザルト画像
	int resultImg_;

	//BGM
	std::shared_ptr<SoundController>bgm_;

	//ライトハンドル
	int lightHandle_;
	//ステージ用スクリーン
	int stageScreen_;

	//ステージモデル
	int stageModelHandle_;
	std::unique_ptr<ModelMaterial>stageModelMaterial_;
	//モデルレンダラー
	std::vector<std::unique_ptr<ModelRenderer>>modelRenderers_;
	//モデル作成
	void SettingObject(void);


};

