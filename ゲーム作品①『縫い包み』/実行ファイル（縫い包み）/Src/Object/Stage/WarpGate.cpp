#include"../../Manager/Camera.h"
#include"../../Manager/SceneManager.h"
#include"../../Manager/ResourceManager.h"
#include "../../Graphics/Drawable/3DModel/ModelMaterial.h"
#include "../../Graphics/Drawable/3DModel/ModelRenderer.h"
#include "../Common/Transform.h"

#include "WarpGate.h"

//���[�v�Q�[�g�T�C�Y
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

	//���f���}�e���A���̍쐬
	modelMaterial_ = std::make_unique<ModelMaterial>();
	modelMaterial_->DataLoad(
		ModelMaterial::VS_TYPE::MODEL_NORMAL_1FRAME,
		ModelMaterial::PS_TYPE::DISTORTION_WALL);

}

void WarpGate::Init(void)
{
	//3D��{���̏�����
	transform_.Init(
		WARP_GATE_SCALE,
		{0.0f,0.0f,0.0f },
		Quaternion(),
		Quaternion(),
		Collider::TYPE::STAGE
	);

	//���f���}�e���A���̍쐬
	modelMaterial_->Init();
	modelRenderer_ = std::make_unique<ModelRenderer>(transform_.modelId_, *modelMaterial_);

}

void WarpGate::Update(void)
{
	//�t�H�O���:	//�萔�o�b�t�@�̍X�V
	VECTOR cameraPos = scnMng_.GetCamera().GetPos();
	float fogEnd, fogStart;
	GetFogStartEnd(&fogStart, &fogEnd);

	//�ʏ�`��}�e���A��	
	modelMaterial_->SetConstBufVS(0, FLOAT4{ cameraPos.x,cameraPos.y,cameraPos.z,fogStart });
	modelMaterial_->SetConstBufVS(1, FLOAT4{ fogEnd ,0,0,0 });

	float totalTime = scnMng_.GetTotalTime();
	modelMaterial_->SetConstBufPS(0, FLOAT4{ totalTime ,0.0f,0.0f,0.0f });

}

void WarpGate::Draw(void)
{
	//�I���W�i���V�F�[�_�[�ł̕`��
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
