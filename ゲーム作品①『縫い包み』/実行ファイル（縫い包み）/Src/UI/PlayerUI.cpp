#include "../Application.h"
#include "../Utility/Utility.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Object/Character/Player.h"
#include "../Graphics/Drawable/2DImage/PixelMaterial.h"
#include "../Graphics/Drawable/2DImage/PixelRenderer.h"
#include "PlayerUI.h"

//護符の消費演出完了時間
static constexpr float AMULET_BURN_COMP_TIME = 4.0f;
//表示テキストサイズ
static constexpr int DISPLAY_TEXT_SIZE = 18;
//コメント描画時間
static constexpr float COMMENT_DISPLAY_TIME = 1.5f;
//センター画像サイズ
static constexpr int CENTER_IMAGE_SIZE = 30;

//護符の描画位置
static constexpr int AMULET_DRAWING_POS_X = 50;
static constexpr int AMULET_DRAWING_POS_Y = 570;
//護符の描画サイズ
static constexpr int AMULET_DRAWING_SIZE_X = 100;
static constexpr int AMULET_DRAWING_SIZE_Y = 250;
//焼却ゲージの描画座標（底辺位置からの距離）
static constexpr int GAUGE_DRAWING_BOTTOM_DIS_Y = 150;
//焼却ゲージの描画サイズ
static constexpr int GAUGE_DRAWING_SIZE_X = 300;
static constexpr int GAUGE_DRAWING_SIZE_Y = 20;

//プレイテキストの描画座標（底辺位置からの距離）
static constexpr int TEXT_DRAWING_BOTTOM_DIS_Y = 100;
//テキスト背面画像のオフセット描画距離
static constexpr int TEXT_BACKGROUND_OFFSET_DIS_X = 60;
static constexpr int TEXT_BACKGROUND_OFFSET_DIS_Y = 20;

//所持護符の描画位置
static constexpr int HAS_AMULET_DRAWING_POS_X = 30;
static constexpr int HAS_AMULET_DRAWING_POS_Y = 30;
//所持護符の描画サイズ
static constexpr int HAS_AMULET_DRAWING_SIZE = 50;
//所持ぬいぐるみの描画位置
static constexpr int HAS_DOOL_DRAWING_POS_X = 30;
static constexpr int HAS_DOOL_DRAWING_POS_Y = 100;

//焼却ゲージシェーダーの描画サイズ
constexpr float BURN_GAUGE_DRAWING_SIZE_X = 300.0f;		//描画サイズX
constexpr float BURN_GAUGE_DRAWING_SIZE_Y = 20.0f;		//描画サイズX

PlayerUI::PlayerUI(const Player& player):player_(player)
{
	centerImg_ = 0;
	amiletUiImage_ = 0;
	doolUiImage_ = 0;
	fontHandle_ = 0;
	actionTextBackImg_ = 0;
	commnetTextBackImg_ = 0;

	std::wstring actionText_=L"";
	std::wstring commnetText_ = L"";

	amuletBurnTime_ = 0.0f;	
	displayCoundTime_ = 0.0f;
	isDisplayCommentText_=false;
	isBurningAmulet_ = false;

}

PlayerUI::~PlayerUI(void)
{
}

void PlayerUI::LoadData(void)
{
	auto& res = ResourceManager::GetInstance();
	//フォント登録
	res.Load(ResourceManager::SRC::ACTION_TEXT_FONT);

	//テキスト背景画像
	actionTextBackImg_ = res.Load(
		ResourceManager::SRC::ACTION_TEXT_BOX_IMG).handleId_;
	commnetTextBackImg_ = res.Load(
		ResourceManager::SRC::COMMENT_TEXT_BOX_IMG).handleId_;

	//照準画像
	centerImg_ = res.Load(ResourceManager::SRC::CENTER_UI_IMG).handleId_;

	//UI画像
	amiletUiImage_ = res.Load(ResourceManager::SRC::AMULET_UI_IMG).handleId_;
	doolUiImage_ = res.Load(ResourceManager::SRC::DOOL_UI_IMG).handleId_;



	// 護符シェーダーのマテリアル
	amuletMaterial_ = std::make_unique<PixelMaterial>();
	amuletMaterial_->DataLoad(PixelMaterial::PS_TYPE::DISTORTION);
	const int amuletImg = res.Load(ResourceManager::SRC::AMULET_IMG).handleId_;
	const int noiseImg = res.Load(ResourceManager::SRC::HEIGHT_MAP_IMG).handleId_;
	amuletMaterial_->SetTextures({ amuletImg ,noiseImg });

	//焼却ゲージマテリアル
	gaugeMaterial_ = std::make_unique<PixelMaterial>();
	gaugeMaterial_->DataLoad(PixelMaterial::PS_TYPE::BURN_GAUGE);
	int gaugeMask = res.Load(ResourceManager::SRC::GAUGE_MASK_IMG).handleId_;	//マスク
	int gaugeMain = res.Load(ResourceManager::SRC::GAUGE_MAIN_IMG).handleId_;	//メイン
	int gaugeFrame = res.Load(ResourceManager::SRC::GAUGE_FRAME_IMG).handleId_;	//フレーム
	gaugeMaterial_->SetTextures({ gaugeMain ,gaugeMask ,gaugeFrame });

}

void PlayerUI::Init(void)
{
	auto screenX = Application::SCREEN_SIZE_X;
	auto screenY = Application::SCREEN_SIZE_Y;

	actionText_ = L"";
	commnetText_ = L"";

	displayCoundTime_ = 0.0f;
	isDisplayCommentText_ = false;


	//フォントの作成
	fontHandle_ = CreateFontToHandle(L"NemukeMedium", DISPLAY_TEXT_SIZE, 0);

	// 護符レンダラー
	amuletMaterial_->Init();
	amuletRenderer_ = std::make_unique<PixelRenderer>(*amuletMaterial_);
	Vector2 pos = { AMULET_DRAWING_POS_X,AMULET_DRAWING_POS_Y };
	Vector2 size = { AMULET_DRAWING_SIZE_X, AMULET_DRAWING_SIZE_Y };
	amuletRenderer_->MakeSquereVertex(pos, size);

	//焼却ゲージ
	gaugeMaterial_->Init();
	gaugeRenderer_ = std::make_unique<PixelRenderer>(*gaugeMaterial_);
	size = { GAUGE_DRAWING_SIZE_X,GAUGE_DRAWING_SIZE_Y };
	pos = { screenX / 2 - size.x / 2, static_cast<float>(screenY - GAUGE_DRAWING_BOTTOM_DIS_Y )};
	gaugeRenderer_->MakeSquereVertex(pos, size);
}

void PlayerUI::Update(void)
{
	//コメントテキストの描画判断
	if (!isDisplayCommentText_)
	{
		commnetText_ = player_.GetCommentText();
		if (!commnetText_.empty())
		{
			isDisplayCommentText_ = true;
		}
	}
	else if (Utility::TimeOver(displayCoundTime_, COMMENT_DISPLAY_TIME))
	{
		isDisplayCommentText_ = false;
		displayCoundTime_ = 0.0f;
	}

	//護符の演出状態
	if (player_.IsUseAmulet())
	{
		isBurningAmulet_ = true;
	}
	if (isBurningAmulet_)
	{
		amuletBurnTime_ += SceneManager::GetInstance().GetDeltaTime();
		//定数バッファの更新
		amuletMaterial_->SetConstBuf({
			FLOAT4{amuletBurnTime_,AMULET_BURN_COMP_TIME} });
		//消費演出の終了
		if (amuletBurnTime_ >= AMULET_BURN_COMP_TIME)
		{
			amuletBurnTime_ = 0.0f;
			isBurningAmulet_ = false;
		}
	}


	//焼却時間の割合
	float par = player_.GetBurnTime() / Player::BURN_END_TIME;
	//焼却ゲージの定数バッファ更新
	gaugeMaterial_->SetConstBuf({ FLOAT4{par,BURN_GAUGE_DRAWING_SIZE_X,BURN_GAUGE_DRAWING_SIZE_Y,0.0f} });
}

void PlayerUI::Draw(void)
{
	//アクションテキストの描画
	DrawTextUI();

	//焼却時間の描画
	DrawBurningTime();

	//護符消費演出
	DrawBurningAmulet();

	//所持アイテムの表示
	DrawHasItem();

	//照準画像
	auto screenX = Application::SCREEN_SIZE_X/2;
	auto screenY = Application::SCREEN_SIZE_Y/2;
	DrawExtendGraph(screenX - CENTER_IMAGE_SIZE, screenY - CENTER_IMAGE_SIZE, 
		screenX + CENTER_IMAGE_SIZE, screenY + CENTER_IMAGE_SIZE, centerImg_, true);

}

void PlayerUI::DrawTextUI(void)
{
	//テキストの描画するラムダ式
	auto drawText = [&](std::wstring text,int font ,int bgImg) 
	{
		auto screenX = Application::SCREEN_SIZE_X;
		auto screenY = Application::SCREEN_SIZE_Y;

		int halfLen = Utility::GetStringLength(
			text, font) / 2;

		//描画座標
		int x, y;
		x = screenX / 2;
		y = screenY - TEXT_DRAWING_BOTTOM_DIS_Y;

		//テキスト背景画像
		const int offsetX = TEXT_BACKGROUND_OFFSET_DIS_X;
		const int offsetY = TEXT_BACKGROUND_OFFSET_DIS_Y;
		DrawExtendGraph(
			x - halfLen - offsetX, y - offsetY,
			x + halfLen + offsetX, y + offsetY + DISPLAY_TEXT_SIZE,
			bgImg, true);

		x -= halfLen;
		DrawStringToHandle(x, y, text.c_str(), 0xdddddd, font);

	};


	//テキストの描画
	if (isDisplayCommentText_)
	{
		//コメントテキストの描画
		drawText(commnetText_, fontHandle_, commnetTextBackImg_);
	}
	else if (!player_.GetActionText().empty())
	{
		//アクションテキストの描画
		actionText_ = player_.GetActionText();
		drawText(actionText_, fontHandle_, actionTextBackImg_);
	}
}

void PlayerUI::DrawBurningTime(void)
{
	if (player_.GetBurnTime() > 0.0f)
	{
		//ゲージ
		gaugeRenderer_->Draw();
	}
}

void PlayerUI::DrawBurningAmulet(void)
{
	if (isBurningAmulet_)
	{
		amuletRenderer_->Draw();
	}
}

void PlayerUI::DrawHasItem(void)
{
	Vector2 pos;

	//護符の所持枚数
	pos = { HAS_AMULET_DRAWING_POS_X,HAS_AMULET_DRAWING_POS_Y };
	int diffX = HAS_AMULET_DRAWING_SIZE;
	int size = HAS_AMULET_DRAWING_SIZE;
	int amuletNum = player_.GetAmuletNum();
	//横に並べる
	for (int i = 0; i < amuletNum; i++)
	{
		DrawExtendGraph(
			static_cast<int>(pos.x) + diffX * i, static_cast<int>(pos.y),
			static_cast<int>(pos.x) + diffX * i + size, static_cast<int>(pos.y) + size,
			amiletUiImage_, true);
	}

	//ぬいぐるみの所持状態
	pos = { HAS_DOOL_DRAWING_POS_X,HAS_DOOL_DRAWING_POS_Y };
	bool haveDool = player_.HaveTheDoll();
	if (haveDool)
	{
		DrawExtendGraph(
			static_cast<int>(pos.x), static_cast<int>(pos.y),
			static_cast<int>(pos.x) + size, static_cast<int>(pos.y) + size, doolUiImage_, true);
	}
}
