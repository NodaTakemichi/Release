#include <EffekseerForDXLib.h>
#include "../../Manager/ResourceManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/SoundManager.h"
#include "../../Manager/Camera.h"
#include "../../Utility/Utility.h"
#include "../../Graphics/Drawable/3DModel/ModelMaterial.h"
#include "../../Graphics/Drawable/3DModel/ModelRenderer.h"
#include "../../Sound/SoundController.h"
#include "../Common/Transform.h"
#include "../Character/Player.h"
#include "Stage.h"

//暖炉のポイントライト
constexpr float FIREPLACE_LIGHT_RANGE = 400.0f;			//有効距離
constexpr float FIREPLACE_LIGHT_ATTEN_0 = 0.0f;			//距離減衰パラメータ０
constexpr float FIREPLACE_LIGHT_ATTEN_1 = 0.005f;		//距離減衰パラメータ１
constexpr float FIREPLACE_LIGHT_ATTEN_2 = 0.0f;			//距離減衰パラメータ２
//暖炉エフェクトのサイズ
constexpr float FIREPLACE_EFFECT_SIZE = 13.0f;
//暖炉サウンド
constexpr int FIREPLACE_SE_VOLUME = 75;	//音量
constexpr float FIREPLACE_SE_HEARING_AREA = 800.0f;	//音が届く距離

//発行シェーダー:PS
constexpr int LUMI_MODEL_PS_CONST_BUF_NUM = 2;		//PSの定数バッファ数
constexpr float LUMI_DIS_TIME = 1500.0f;	//発行距離時間
constexpr float LUMI_DIS_AREA = 100.0f;	//発行距離範囲

Stage::Stage(const Player& player):player_(player)
{
	fireHandle_=0;
	fireLightHandle_ = 0;
	lumiProgressTime_ = 0.0f;
	isLumiDrawing_ = false;
	amuletActionPos_=VECTOR();

}

Stage::~Stage(void)
{
}

void Stage::LoadData(void)
{
	auto& res = ResourceManager::GetInstance();

	transform_.SetModel(resMng_.Load(ResourceManager::SRC::STAGE_MODEL).handleId_);
	collTransform_.SetModel(resMng_.Load(ResourceManager::SRC::STAGE_COLL_MODEL).handleId_);
	metaTransform_.SetModel(resMng_.Load(ResourceManager::SRC::METALLIC_MODEL).handleId_);

	//通常モデルマテリアルの作成
	normalModelMaterial_ = std::make_unique<ModelMaterial>();
	normalModelMaterial_->DataLoad(
		ModelMaterial::VS_TYPE::MODEL_NORMAL_1FRAME,
		ModelMaterial::PS_TYPE::MODEL_NORMAL);

	//発光モデルマテリアル
	lumiModelMaterial_ = std::make_unique<ModelMaterial>();
	lumiModelMaterial_->DataLoad(
		ModelMaterial::VS_TYPE::MODEL_NORMAL_1FRAME,
		ModelMaterial::PS_TYPE::LUMINESCENCE);

	//スフィアマップマテリアル
	sphereMaterial_ = std::make_unique<ModelMaterial>();
	sphereMaterial_->DataLoad(
		ModelMaterial::VS_TYPE::MODEL_NORMAL_1FRAME,
		ModelMaterial::PS_TYPE::SPHERE_MAP);
	//スフィアアップのテクスチャ貼り付け
	int rough = LoadGraph(L"Data/Model/pbrmodel/steel/roughness.png");
	int metallic = LoadGraph(L"Data/Model/pbrmodel/steel/metallic.png");
	int sphmap = LoadGraph(L"Data/Model/pbrmodel/sph.png");
	//テクスチャは3枚目から貼り付ける
	sphereMaterial_->SetTextures(2, rough);
	sphereMaterial_->SetTextures(3, metallic);
	sphereMaterial_->SetTextures(4, sphmap);


	//サウンド
	fireSound_ = std::make_shared<SoundController>();
	fireSound_->DataLoad(ResourceManager::SRC::FIRE_SND);

}

void Stage::Init(void)
{	
	//見た目用ステージの3D基本情報の初期化
	transform_.Init(
		Utility::VECTOR_ONE,
		Utility::VECTOR_ZERO,
		Quaternion(),
		Quaternion::Euler({ 0.0f, Utility::ANGLE_270_RAD, 0.0f }),
		Collider::TYPE::NONE
	);

	//衝突用ステージの3D基本情報の初期化
	collTransform_.Init(
		transform_.scl_,
		transform_.pos_,
		transform_.quaRot_,
		transform_.quaRotLocal_,
		Collider::TYPE::STAGE
	);

	//メタリックオブジェクトの3D基本情報の初期化
	metaTransform_.Init(
		transform_.scl_,
		transform_.pos_,
		transform_.quaRot_,
		transform_.quaRotLocal_,
		Collider::TYPE::NONE
	);

	//通常モデルマテリアルの作成
	normalModelMaterial_->Init();
	normalModelRenderer_ = std::make_unique<ModelRenderer>(transform_.modelId_, *normalModelMaterial_);

	//発光モデルマテリアルの作成
	lumiModelMaterial_->Init();
	lumiModelRenderer_ = std::make_unique<ModelRenderer>(transform_.modelId_, *lumiModelMaterial_);

	//スフィアマップマテリアルの作成
	sphereMaterial_->Init();
	sphereRenderer_ = std::make_unique<ModelRenderer>(metaTransform_.modelId_, *sphereMaterial_);

	//発光描画無効
	isLumiDrawing_ = false; 


	//暖炉のエフェクトの再生	
	fireHandle_ = resMng_.Load(ResourceManager::SRC::FIRE_EFF).handleId_;
	float size = FIREPLACE_EFFECT_SIZE;
	int playHandle = PlayEffekseer3DEffect(fireHandle_);
	SetScalePlayingEffekseer3DEffect(playHandle, size, size, size);
	SetPosPlayingEffekseer3DEffect(playHandle, FIREPLACE_POS.x, FIREPLACE_POS.y, FIREPLACE_POS.z);

	//炎のライト作成
	fireLightHandle_ = CreatePointLightHandle(
		FIREPLACE_LIGHT_POS,
		FIREPLACE_LIGHT_RANGE,
		FIREPLACE_LIGHT_ATTEN_0, FIREPLACE_LIGHT_ATTEN_1, FIREPLACE_LIGHT_ATTEN_2
	);

	//炎サウンド
	fireSound_->Init(
		FIREPLACE_SE_VOLUME, FIREPLACE_SE_HEARING_AREA,
		FIREPLACE_LIGHT_POS, SoundController::REVERB_TYPE::PADDEDCELL);
	fireSound_->PlayLoop();

}

void Stage::Update(void)
{
	//護符使用判定
	if (player_.IsUseAmulet())
	{
		//発光マテリアルに変更
		isLumiDrawing_ = true;
		//護符発動場所
		amuletActionPos_ = player_.GetTransform().GetPos();
		//発光タイムのリセット
		lumiProgressTime_ = 0.0f;
	}
	//時間経過で通常描画に戻す
	if (lumiProgressTime_ >= Player::AMULET_USE_COOL_TIME)
	{
		isLumiDrawing_ = false;
	}

	//フォグ情報
	VECTOR cameraPos = scnMng_.GetCamera().GetPos();
	float fogEnd, fogStart;
	GetFogStartEnd(&fogStart, &fogEnd);
#pragma region 定数バッファの更新
	//通常描画マテリアル	
	normalModelMaterial_->SetConstBufVS(0, FLOAT4{ cameraPos.x,cameraPos.y,cameraPos.z,fogStart });
	normalModelMaterial_->SetConstBufVS(1, FLOAT4{ fogEnd ,0,0,0 });

	//発光描画マテリアル
	if (isLumiDrawing_)
	{
		//頂点シェーダー
		lumiModelMaterial_->SetConstBufVS(0, FLOAT4{ cameraPos.x,cameraPos.y,cameraPos.z,fogStart });
		lumiModelMaterial_->SetConstBufVS(1, FLOAT4{ fogEnd ,0,0,0 });

		//ピクセルシェーダー
		lumiProgressTime_ += scnMng_.GetDeltaTime();
		lumiModelMaterial_->SetConstBufPS(0,
			FLOAT4{ amuletActionPos_.x,amuletActionPos_.y,amuletActionPos_.z,lumiProgressTime_ });
		lumiModelMaterial_->SetConstBufPS(1,
			FLOAT4{ LUMI_DIS_TIME,LUMI_DIS_AREA });	//発行距離時間、発行距離範囲
	}


	//スフィアマップ	
	//頂点シェーダー
	sphereMaterial_->SetConstBufVS(0, FLOAT4{ cameraPos.x,cameraPos.y,cameraPos.z,fogStart });
	sphereMaterial_->SetConstBufVS(1, FLOAT4{ fogEnd ,0,0,0 });
	//ピクセルシェーダー
	sphereMaterial_->SetConstBufPS(0, FLOAT4{ cameraPos.x,cameraPos.y,cameraPos.z,0 });
	auto vmat = GetCameraViewMatrix();
	int idx = 1;
	for (int j = 0; j < 4; ++j) {
		FLOAT4 mat = { vmat.m[0][j],vmat.m[1][j],vmat.m[2][j],vmat.m[3][j] };
		sphereMaterial_->SetConstBufPS(idx, mat);
		idx++;
	}

#pragma endregion
	return;
}

void Stage::Draw(void)
{
	 //モデルの描画
	if (!isLumiDrawing_)
	{
		normalModelRenderer_->Draw();	//通常描画
	}
	else
	{
		lumiModelRenderer_->Draw();		//発光描画

	}

	//メタリックオブジェクトの描画
	sphereRenderer_->Draw();


}

const Transform& Stage::GetCollStageTransform(void) const
{
	return collTransform_;
}
