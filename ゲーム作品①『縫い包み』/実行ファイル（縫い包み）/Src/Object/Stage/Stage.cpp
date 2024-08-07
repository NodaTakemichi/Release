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

//�g�F�̃|�C���g���C�g
constexpr float FIREPLACE_LIGHT_RANGE = 400.0f;			//�L������
constexpr float FIREPLACE_LIGHT_ATTEN_0 = 0.0f;			//���������p�����[�^�O
constexpr float FIREPLACE_LIGHT_ATTEN_1 = 0.005f;		//���������p�����[�^�P
constexpr float FIREPLACE_LIGHT_ATTEN_2 = 0.0f;			//���������p�����[�^�Q
//�g�F�G�t�F�N�g�̃T�C�Y
constexpr float FIREPLACE_EFFECT_SIZE = 13.0f;
//�g�F�T�E���h
constexpr int FIREPLACE_SE_VOLUME = 75;	//����
constexpr float FIREPLACE_SE_HEARING_AREA = 800.0f;	//�����͂�����

//���s�V�F�[�_�[:PS
constexpr int LUMI_MODEL_PS_CONST_BUF_NUM = 2;		//PS�̒萔�o�b�t�@��
constexpr float LUMI_DIS_TIME = 1500.0f;	//���s��������
constexpr float LUMI_DIS_AREA = 100.0f;	//���s�����͈�

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

	//�ʏ탂�f���}�e���A���̍쐬
	normalModelMaterial_ = std::make_unique<ModelMaterial>();
	normalModelMaterial_->DataLoad(
		ModelMaterial::VS_TYPE::MODEL_NORMAL_1FRAME,
		ModelMaterial::PS_TYPE::MODEL_NORMAL);

	//�������f���}�e���A��
	lumiModelMaterial_ = std::make_unique<ModelMaterial>();
	lumiModelMaterial_->DataLoad(
		ModelMaterial::VS_TYPE::MODEL_NORMAL_1FRAME,
		ModelMaterial::PS_TYPE::LUMINESCENCE);

	//�X�t�B�A�}�b�v�}�e���A��
	sphereMaterial_ = std::make_unique<ModelMaterial>();
	sphereMaterial_->DataLoad(
		ModelMaterial::VS_TYPE::MODEL_NORMAL_1FRAME,
		ModelMaterial::PS_TYPE::SPHERE_MAP);
	//�X�t�B�A�A�b�v�̃e�N�X�`���\��t��
	int rough = LoadGraph(L"Data/Model/pbrmodel/steel/roughness.png");
	int metallic = LoadGraph(L"Data/Model/pbrmodel/steel/metallic.png");
	int sphmap = LoadGraph(L"Data/Model/pbrmodel/sph.png");
	//�e�N�X�`����3���ڂ���\��t����
	sphereMaterial_->SetTextures(2, rough);
	sphereMaterial_->SetTextures(3, metallic);
	sphereMaterial_->SetTextures(4, sphmap);


	//�T�E���h
	fireSound_ = std::make_shared<SoundController>();
	fireSound_->DataLoad(ResourceManager::SRC::FIRE_SND);

}

void Stage::Init(void)
{	
	//�����ڗp�X�e�[�W��3D��{���̏�����
	transform_.Init(
		Utility::VECTOR_ONE,
		Utility::VECTOR_ZERO,
		Quaternion(),
		Quaternion::Euler({ 0.0f, Utility::ANGLE_270_RAD, 0.0f }),
		Collider::TYPE::NONE
	);

	//�Փ˗p�X�e�[�W��3D��{���̏�����
	collTransform_.Init(
		transform_.scl_,
		transform_.pos_,
		transform_.quaRot_,
		transform_.quaRotLocal_,
		Collider::TYPE::STAGE
	);

	//���^���b�N�I�u�W�F�N�g��3D��{���̏�����
	metaTransform_.Init(
		transform_.scl_,
		transform_.pos_,
		transform_.quaRot_,
		transform_.quaRotLocal_,
		Collider::TYPE::NONE
	);

	//�ʏ탂�f���}�e���A���̍쐬
	normalModelMaterial_->Init();
	normalModelRenderer_ = std::make_unique<ModelRenderer>(transform_.modelId_, *normalModelMaterial_);

	//�������f���}�e���A���̍쐬
	lumiModelMaterial_->Init();
	lumiModelRenderer_ = std::make_unique<ModelRenderer>(transform_.modelId_, *lumiModelMaterial_);

	//�X�t�B�A�}�b�v�}�e���A���̍쐬
	sphereMaterial_->Init();
	sphereRenderer_ = std::make_unique<ModelRenderer>(metaTransform_.modelId_, *sphereMaterial_);

	//�����`�斳��
	isLumiDrawing_ = false; 


	//�g�F�̃G�t�F�N�g�̍Đ�	
	fireHandle_ = resMng_.Load(ResourceManager::SRC::FIRE_EFF).handleId_;
	float size = FIREPLACE_EFFECT_SIZE;
	int playHandle = PlayEffekseer3DEffect(fireHandle_);
	SetScalePlayingEffekseer3DEffect(playHandle, size, size, size);
	SetPosPlayingEffekseer3DEffect(playHandle, FIREPLACE_POS.x, FIREPLACE_POS.y, FIREPLACE_POS.z);

	//���̃��C�g�쐬
	fireLightHandle_ = CreatePointLightHandle(
		FIREPLACE_LIGHT_POS,
		FIREPLACE_LIGHT_RANGE,
		FIREPLACE_LIGHT_ATTEN_0, FIREPLACE_LIGHT_ATTEN_1, FIREPLACE_LIGHT_ATTEN_2
	);

	//���T�E���h
	fireSound_->Init(
		FIREPLACE_SE_VOLUME, FIREPLACE_SE_HEARING_AREA,
		FIREPLACE_LIGHT_POS, SoundController::REVERB_TYPE::PADDEDCELL);
	fireSound_->PlayLoop();

}

void Stage::Update(void)
{
	//�아�g�p����
	if (player_.IsUseAmulet())
	{
		//�����}�e���A���ɕύX
		isLumiDrawing_ = true;
		//�아�����ꏊ
		amuletActionPos_ = player_.GetTransform().GetPos();
		//�����^�C���̃��Z�b�g
		lumiProgressTime_ = 0.0f;
	}
	//���Ԍo�߂Œʏ�`��ɖ߂�
	if (lumiProgressTime_ >= Player::AMULET_USE_COOL_TIME)
	{
		isLumiDrawing_ = false;
	}

	//�t�H�O���
	VECTOR cameraPos = scnMng_.GetCamera().GetPos();
	float fogEnd, fogStart;
	GetFogStartEnd(&fogStart, &fogEnd);
#pragma region �萔�o�b�t�@�̍X�V
	//�ʏ�`��}�e���A��	
	normalModelMaterial_->SetConstBufVS(0, FLOAT4{ cameraPos.x,cameraPos.y,cameraPos.z,fogStart });
	normalModelMaterial_->SetConstBufVS(1, FLOAT4{ fogEnd ,0,0,0 });

	//�����`��}�e���A��
	if (isLumiDrawing_)
	{
		//���_�V�F�[�_�[
		lumiModelMaterial_->SetConstBufVS(0, FLOAT4{ cameraPos.x,cameraPos.y,cameraPos.z,fogStart });
		lumiModelMaterial_->SetConstBufVS(1, FLOAT4{ fogEnd ,0,0,0 });

		//�s�N�Z���V�F�[�_�[
		lumiProgressTime_ += scnMng_.GetDeltaTime();
		lumiModelMaterial_->SetConstBufPS(0,
			FLOAT4{ amuletActionPos_.x,amuletActionPos_.y,amuletActionPos_.z,lumiProgressTime_ });
		lumiModelMaterial_->SetConstBufPS(1,
			FLOAT4{ LUMI_DIS_TIME,LUMI_DIS_AREA });	//���s�������ԁA���s�����͈�
	}


	//�X�t�B�A�}�b�v	
	//���_�V�F�[�_�[
	sphereMaterial_->SetConstBufVS(0, FLOAT4{ cameraPos.x,cameraPos.y,cameraPos.z,fogStart });
	sphereMaterial_->SetConstBufVS(1, FLOAT4{ fogEnd ,0,0,0 });
	//�s�N�Z���V�F�[�_�[
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
	 //���f���̕`��
	if (!isLumiDrawing_)
	{
		normalModelRenderer_->Draw();	//�ʏ�`��
	}
	else
	{
		lumiModelRenderer_->Draw();		//�����`��

	}

	//���^���b�N�I�u�W�F�N�g�̕`��
	sphereRenderer_->Draw();


}

const Transform& Stage::GetCollStageTransform(void) const
{
	return collTransform_;
}
