#include "../../Manager/ResourceManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/SoundManager.h"
#include "../../Utility/Utility.h"
#include "../../Graphics/Drawable/3DModel/ModelMaterial.h"
#include "../../Graphics/Drawable/3DModel/ModelRenderer.h"
#include "../../Sound/SoundController.h"

#include "../Character/Player.h"
#include "Dool.h"

//焼失SE
constexpr int BURN_SE_VOLUME = 250;				//音量
constexpr float BURN_SE_HEARING_AREA = 700.0f;	//音が届く距離

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

	//ぬいるぐるみ焼失ＳＥ
	burnDoolSound_ = std::make_shared<SoundController>();
	burnDoolSound_->DataLoad(ResourceManager::SRC::BURN_DOOL_SND);

}

void Dool::Init(void)
{

	//モデルマテリアルの作成
	modelMaterial_->Init();
	modelRenderer_ = std::make_unique<ModelRenderer>(transform_.modelId_, *modelMaterial_);

	//ハイトマップ画像の設定
	int heightMap = resMng_.Load(ResourceManager::SRC::HEIGHT_MAP_IMG).handleId_;
	modelMaterial_->SetTextures(2, heightMap);

	//焼失SE
	burnDoolSound_->Init(BURN_SE_VOLUME, BURN_SE_HEARING_AREA, 
		transform_.pos_, SoundController::REVERB_TYPE::PADDEDCELL);

	isBurnDirection_ = false;
}

void Dool::Update(void)
{
	if (!isBurnDirection_)return;

	//焼失SE再生
	burnDoolSound_->PlayNonPlaying();


	//焼失完了時間
	const float endTime = Player::BURN_END_TIME;

	//定数バッファの更新
	burnTime_ += scnMng_.GetDeltaTime();
	modelMaterial_->SetConstBufPS(0, FLOAT4{ burnTime_ ,endTime,0.0f,0.0f});

	//焼失演出の終了
	if (burnTime_ >= endTime)
	{
		isBurnDirection_ = false;
	}

}

void Dool::Draw(void)
{
	//通常描画
	if (!GetHasItem())
	{
		MV1DrawModel(transform_.modelId_);
	}
	else if(isBurnDirection_)
	{
		//焼失描画
		modelRenderer_->Draw();

	}
}

