#include"../../Manager/Camera.h"
#include"../../Manager/SceneManager.h"
#include"../../Manager/ResourceManager.h"
#include "../../Graphics/Drawable/3DModel/ModelMaterial.h"
#include "../../Graphics/Drawable/3DModel/ModelRenderer.h"
#include "../Common/Transform.h"

#include "WarpGate.h"

//ワープゲートサイズ
constexpr VECTOR WARP_GATE_SCALE = { 2.0f,3.0f,0.5f };

WarpGate::WarpGate(void)
{
}

WarpGate::~WarpGate(void)
{
}

void WarpGate::LoadData(void)
{
	transform_.SetModel(
		resMng_.LoadModelDuplicate(ResourceManager::SRC::DISTORTION_WALL));

	//モデルマテリアルの作成
	modelMaterial_ = std::make_unique<ModelMaterial>();
	modelMaterial_->DataLoad(
		ModelMaterial::VS_TYPE::MODEL_NORMAL_1FRAME,
		ModelMaterial::PS_TYPE::DISTORTION_WALL);

}

void WarpGate::Init(void)
{
	//3D基本情報の初期化
	transform_.Init(
		WARP_GATE_SCALE,
		{0.0f,0.0f,0.0f },
		Quaternion(),
		Quaternion(),
		Collider::TYPE::STAGE
	);

	//モデルマテリアルの作成
	modelMaterial_->Init();
	modelRenderer_ = std::make_unique<ModelRenderer>(transform_.modelId_, *modelMaterial_);

}

void WarpGate::Update(void)
{
	//フォグ情報:	//定数バッファの更新
	VECTOR cameraPos = scnMng_.GetCamera().GetPos();
	float fogEnd, fogStart;
	GetFogStartEnd(&fogStart, &fogEnd);

	//通常描画マテリアル	
	modelMaterial_->SetConstBufVS(0, FLOAT4{ cameraPos.x,cameraPos.y,cameraPos.z,fogStart });
	modelMaterial_->SetConstBufVS(1, FLOAT4{ fogEnd ,0,0,0 });

	float totalTime = scnMng_.GetTotalTime();
	modelMaterial_->SetConstBufPS(0, FLOAT4{ totalTime ,0.0f,0.0f,0.0f });

}

void WarpGate::Draw(void)
{
	//オリジナルシェーダーでの描画
	modelRenderer_->Draw();
}

const void WarpGate::SetPos(const VECTOR& pos)
{
	transform_.pos_ = pos;
	transform_.Update();
}

const void WarpGate::SetRot(const VECTOR& rot)
{
	transform_.quaRot_ = Quaternion::Euler(rot);
	transform_.Update();
}
