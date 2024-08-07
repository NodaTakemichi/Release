#pragma once
#include"StageBase.h"
class ModelMaterial;
class ModelRenderer;
class Player;
class SoundController;

class Stage: public StageBase
{
public:
	// 暖炉の座標
	static constexpr VECTOR FIREPLACE_POS = { 260.0f,-140.0f,-688.0f };
	// 暖炉のライト座標
	static constexpr VECTOR FIREPLACE_LIGHT_POS = { FIREPLACE_POS.x - 40.0f,FIREPLACE_POS.y,FIREPLACE_POS.z };
	// 暖炉の座標(プレイヤー側での衝突判定用のため static)
	static constexpr VECTOR FIREPLACE_HIT_POS = { FIREPLACE_POS.x - 50.0f,FIREPLACE_POS.y + 50.0f,FIREPLACE_POS.z };

	Stage(const Player& player);
	virtual ~Stage(void);

	void LoadData(void)override;
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;

	//ステージコリジョンの取得
	const Transform& GetCollStageTransform(void) const;


private:
	//プレイヤー情報
	const Player& player_;

	//衝突判定用モデル
	Transform collTransform_;

	//メタリックオブジェクトモデル
	Transform metaTransform_;

	//暖炉の炎エフェクトハンドル
	int fireHandle_;
	//暖炉の炎サウンド
	std::shared_ptr< SoundController>fireSound_;
	//ライトハンドル
	int fireLightHandle_;


	//通常描画マテリアル
	std::unique_ptr<ModelMaterial>normalModelMaterial_;
	std::unique_ptr<ModelRenderer>normalModelRenderer_;
	//発光描画マテリアル
	std::unique_ptr<ModelMaterial>lumiModelMaterial_;
	std::unique_ptr<ModelRenderer>lumiModelRenderer_;

	//スフィアマップマテリアル
	std::unique_ptr<ModelMaterial>sphereMaterial_;
	std::unique_ptr<ModelRenderer>sphereRenderer_;


	//発光用経過時間
	float lumiProgressTime_;
	//発光描画中か判断
	bool isLumiDrawing_;
	//護符発動座標
	VECTOR amuletActionPos_;

};

