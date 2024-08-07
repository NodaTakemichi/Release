#include "../../Manager/ResourceManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/SoundManager.h"
#include "../../Manager/Camera.h"
#include "../../Utility/Utility.h"
#include "../../Graphics/Drawable/3DModel/ModelMaterial.h"
#include "../../Graphics/Drawable/3DModel/ModelRenderer.h"
#include "../../Sound/SoundController.h"
#include "../Common/Transform.h"
#include "GoalGate.h"

//ゴールゲート
constexpr VECTOR GOAL_GATE_SCL = { 3.5f,2.0f,0.5f };			//大きさ
constexpr VECTOR GOAL_GATE_POS = { -7.0f, 450.0f, -2800.0f };	//座標

//歪みSE
constexpr int DISTORTION_SE_VOLUME = 80;		//音量
constexpr float DISTORTION_SE_HEARING_AREA = 400.0f;	//音が届く距離
//消失SE
constexpr int LOST_SE_VOLUME = 150;				//音量
constexpr float LOST_SE_HEARING_AREA = 400.0f;	//音が届く距離

GoalGate::GoalGate(void)
{
	dissolveTime_=0.0f;
	openGate_ = false;
}

GoalGate::~GoalGate(void)
{
}

void GoalGate::LoadData(void)
{
	transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::DISTORTION_WALL));

	//モデルマテリアルの作成
	modelMaterial_ = std::make_unique<ModelMaterial>();
	modelMaterial_->DataLoad(
		ModelMaterial::VS_TYPE::MODEL_NORMAL_1FRAME,
		ModelMaterial::PS_TYPE::DISTORTION_WALL);
	//テクスチャの追加
	int heightMap = resMng_.Load(ResourceManager::SRC::HEIGHT_MAP_IMG).handleId_;
	modelMaterial_->SetTextures(2, heightMap);


	//歪みSE
	disSound_ = std::make_shared<SoundController>();
	disSound_->DataLoad(ResourceManager::SRC::DIS_WALL_SND);
	//消失SE
	loastWallSound_ = std::make_shared<SoundController>();
	loastWallSound_->DataLoad(ResourceManager::SRC::DIS_WALL_LOST_SND);
}

void GoalGate::Init(void)
{
	//3D基本情報の初期化
	transform_.Init(
		GOAL_GATE_SCL,
		GOAL_GATE_POS,
		Quaternion(),
		Quaternion(),
		Collider::TYPE::DIS_WALL
	);


	//モデルマテリアルの作成
	modelMaterial_->Init();
	modelRenderer_ = std::make_unique<ModelRenderer>(transform_.modelId_, *modelMaterial_);

	//サウンドの設定
	disSound_->Init(
		DISTORTION_SE_VOLUME, DISTORTION_SE_HEARING_AREA, 
		transform_.pos_, SoundController::REVERB_TYPE::PADDEDCELL);
	loastWallSound_->Init(
		LOST_SE_VOLUME, LOST_SE_HEARING_AREA,
		transform_.pos_, SoundController::REVERB_TYPE::PADDEDCELL);

	//歪みSEの再生
	disSound_->PlayLoop();

	
	dissolveTime_ = 0.0f;

}

void GoalGate::Update(void)
{
	if (openGate_)
	{
		//ディゾルブ開始
		dissolveTime_ += scnMng_.GetDeltaTime();
	}

	//マテリアルの定数バッファの更新
	//フォグ情報
	VECTOR cameraPos = scnMng_.GetCamera().GetPos();
	float fogEnd, fogStart;
	GetFogStartEnd(&fogStart, &fogEnd);

	modelMaterial_->SetConstBufVS(0, FLOAT4{ cameraPos.x,cameraPos.y,cameraPos.z,fogStart });
	modelMaterial_->SetConstBufVS(1, FLOAT4{ fogEnd ,0,0,0 });

	float totalTime = scnMng_.GetTotalTime();
	//定数バッファの更新
	modelMaterial_->SetConstBufPS(0, FLOAT4{ totalTime ,dissolveTime_,0,0 });
	return;

}

void GoalGate::Draw(void)
{
	//モデルの描画
	modelRenderer_->Draw();
}

void GoalGate::SetOpenGate(void)
{
	openGate_ = true;

	//歪みSEの停止
	disSound_->Stop();
	//消失SEの再生
	loastWallSound_->Play();
}
