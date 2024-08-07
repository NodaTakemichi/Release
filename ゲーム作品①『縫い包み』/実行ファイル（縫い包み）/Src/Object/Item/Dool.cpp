#include "../../Manager/ResourceManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/SoundManager.h"
#include "../../Utility/Utility.h"
#include "../../Graphics/Drawable/3DModel/ModelMaterial.h"
#include "../../Graphics/Drawable/3DModel/ModelRenderer.h"
#include "../../Sound/SoundController.h"

#include "../Character/Player.h"
#include "Dool.h"

//�Ď�SE
constexpr int BURN_SE_VOLUME = 250;				//����
constexpr float BURN_SE_HEARING_AREA = 700.0f;	//�����͂�����

Dool::Dool(void)
{
}

Dool::~Dool(void)
{
}

void Dool::LoadData(void)
{
	modelMaterial_ = std::make_unique<ModelMaterial>();
	modelMaterial_->DataLoad(
		ModelMaterial::VS_TYPE::MODEL_NORMAL_1FRAME,
		ModelMaterial::PS_TYPE::BURNING_DOOL);

	//�ʂ��邮��ݏĎ��r�d
	burnDoolSound_ = std::make_shared<SoundController>();
	burnDoolSound_->DataLoad(ResourceManager::SRC::BURN_DOOL_SND);

}

void Dool::Init(void)
{

	//���f���}�e���A���̍쐬
	modelMaterial_->Init();
	modelRenderer_ = std::make_unique<ModelRenderer>(transform_.modelId_, *modelMaterial_);

	//�n�C�g�}�b�v�摜�̐ݒ�
	int heightMap = resMng_.Load(ResourceManager::SRC::HEIGHT_MAP_IMG).handleId_;
	modelMaterial_->SetTextures(2, heightMap);

	//�Ď�SE
	burnDoolSound_->Init(BURN_SE_VOLUME, BURN_SE_HEARING_AREA, 
		transform_.pos_, SoundController::REVERB_TYPE::PADDEDCELL);

	isBurnDirection_ = false;
}

void Dool::Update(void)
{
	if (!isBurnDirection_)return;

	//�Ď�SE�Đ�
	burnDoolSound_->PlayNonPlaying();


	//�Ď���������
	const float endTime = Player::BURN_END_TIME;

	//�萔�o�b�t�@�̍X�V
	burnTime_ += scnMng_.GetDeltaTime();
	modelMaterial_->SetConstBufPS(0, FLOAT4{ burnTime_ ,endTime,0.0f,0.0f});

	//�Ď����o�̏I��
	if (burnTime_ >= endTime)
	{
		isBurnDirection_ = false;
	}

}

void Dool::Draw(void)
{
	//�ʏ�`��
	if (!GetHasItem())
	{
		MV1DrawModel(transform_.modelId_);
	}
	else if(isBurnDirection_)
	{
		//�Ď��`��
		modelRenderer_->Draw();

	}
}

