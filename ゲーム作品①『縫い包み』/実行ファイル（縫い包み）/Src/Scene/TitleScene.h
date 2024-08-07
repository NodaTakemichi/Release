#pragma once
#include <string>
#include "SceneBase.h"
class ModelMaterial;
class ModelRenderer;
class PostEffectMaterial;
class PostEffectRenderer;
class Button;

//修正
#include"../Graphics/Drawable/3DModel/ModelMaterial.h"
#include"../Graphics/Drawable/3DModel/ModelRenderer.h"
#include"../Graphics/Drawable/PostEffect/PostEffectMaterial.h"
#include"../Graphics/Drawable/PostEffect/PostEffectRenderer.h"



class TitleScene : public SceneBase
{
public:

	TitleScene(SceneManager& manager);
	~TitleScene(void);

	
	virtual void LoadData(void)override;
	virtual void Init(void) override;
	virtual void Update(InputManager& input) override;
	virtual void Draw(void) override;

private:

	//タイトル画像
	int titleImg_;

	//ライトハンドル
	int lightHandle_;
	//ステージ用スクリーン
	int stageScreen_;

	//モデル作成
	void SettingObject(void);

	//各ボタン処理
	void StartButtnProcess(void);	//スタートボタン
	void InstructButtnProcess(void);//説明書ボタン
	void ExitButtnProcess(void);	//終了ボタン

	//Update関数
	void LoadingUpdate(InputManager& input);
	void NormalUpdate(InputManager& input);
	//描画関数
	void LoadingDraw(void);
	void NormalDraw(void);

	//選択ボタン
	std::vector<Button>buttons_;

	//ステージモデル
	int stageModelHandle_;
	std::unique_ptr<ModelMaterial>stageModelMaterial_;
	//ぬいぐるみモデル
	int doolModelHandle_;
	std::unique_ptr<ModelMaterial>doolModelMaterial_;
	//ぬいぐるみ座標
	VECTOR doolPos_;
	//モデルレンダラー
	std::vector<std::unique_ptr<ModelRenderer>>modelRenderers_;


	//ポストエフェクト（マテリアル）
	std::unique_ptr<PostEffectMaterial>glitchMaterial_;		//グリッチシェーダー
	//ポストエフェクトレンダラー
	std::vector<std::unique_ptr<PostEffectRenderer>> postEffectRenderer_;

	//BGM
	std::shared_ptr<SoundController>bgm_;

};
