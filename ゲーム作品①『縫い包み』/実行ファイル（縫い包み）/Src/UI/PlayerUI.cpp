#include "../Application.h"
#include "../Utility/Utility.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Object/Character/Player.h"
#include "../Graphics/Drawable/2DImage/PixelMaterial.h"
#include "../Graphics/Drawable/2DImage/PixelRenderer.h"
#include "PlayerUI.h"

//�아�̏���o��������
static constexpr float AMULET_BURN_COMP_TIME = 4.0f;
//�\���e�L�X�g�T�C�Y
static constexpr int DISPLAY_TEXT_SIZE = 18;
//�R�����g�`�掞��
static constexpr float COMMENT_DISPLAY_TIME = 1.5f;
//�Z���^�[�摜�T�C�Y
static constexpr int CENTER_IMAGE_SIZE = 30;

//�아�̕`��ʒu
static constexpr int AMULET_DRAWING_POS_X = 50;
static constexpr int AMULET_DRAWING_POS_Y = 570;
//�아�̕`��T�C�Y
static constexpr int AMULET_DRAWING_SIZE_X = 100;
static constexpr int AMULET_DRAWING_SIZE_Y = 250;
//�ċp�Q�[�W�̕`����W�i��ӈʒu����̋����j
static constexpr int GAUGE_DRAWING_BOTTOM_DIS_Y = 150;
//�ċp�Q�[�W�̕`��T�C�Y
static constexpr int GAUGE_DRAWING_SIZE_X = 300;
static constexpr int GAUGE_DRAWING_SIZE_Y = 20;

//�v���C�e�L�X�g�̕`����W�i��ӈʒu����̋����j
static constexpr int TEXT_DRAWING_BOTTOM_DIS_Y = 100;
//�e�L�X�g�w�ʉ摜�̃I�t�Z�b�g�`�拗��
static constexpr int TEXT_BACKGROUND_OFFSET_DIS_X = 60;
static constexpr int TEXT_BACKGROUND_OFFSET_DIS_Y = 20;

//�����아�̕`��ʒu
static constexpr int HAS_AMULET_DRAWING_POS_X = 30;
static constexpr int HAS_AMULET_DRAWING_POS_Y = 30;
//�����아�̕`��T�C�Y
static constexpr int HAS_AMULET_DRAWING_SIZE = 50;
//�����ʂ�����݂̕`��ʒu
static constexpr int HAS_DOOL_DRAWING_POS_X = 30;
static constexpr int HAS_DOOL_DRAWING_POS_Y = 100;

//�ċp�Q�[�W�V�F�[�_�[�̕`��T�C�Y
constexpr float BURN_GAUGE_DRAWING_SIZE_X = 300.0f;		//�`��T�C�YX
constexpr float BURN_GAUGE_DRAWING_SIZE_Y = 20.0f;		//�`��T�C�YX

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
	//�t�H���g�o�^
	res.Load(ResourceManager::SRC::ACTION_TEXT_FONT);

	//�e�L�X�g�w�i�摜
	actionTextBackImg_ = res.Load(
		ResourceManager::SRC::ACTION_TEXT_BOX_IMG).handleId_;
	commnetTextBackImg_ = res.Load(
		ResourceManager::SRC::COMMENT_TEXT_BOX_IMG).handleId_;

	//�Ə��摜
	centerImg_ = res.Load(ResourceManager::SRC::CENTER_UI_IMG).handleId_;

	//UI�摜
	amiletUiImage_ = res.Load(ResourceManager::SRC::AMULET_UI_IMG).handleId_;
	doolUiImage_ = res.Load(ResourceManager::SRC::DOOL_UI_IMG).handleId_;



	// �아�V�F�[�_�[�̃}�e���A��
	amuletMaterial_ = std::make_unique<PixelMaterial>();
	amuletMaterial_->DataLoad(PixelMaterial::PS_TYPE::DISTORTION);
	const int amuletImg = res.Load(ResourceManager::SRC::AMULET_IMG).handleId_;
	const int noiseImg = res.Load(ResourceManager::SRC::HEIGHT_MAP_IMG).handleId_;
	amuletMaterial_->SetTextures({ amuletImg ,noiseImg });

	//�ċp�Q�[�W�}�e���A��
	gaugeMaterial_ = std::make_unique<PixelMaterial>();
	gaugeMaterial_->DataLoad(PixelMaterial::PS_TYPE::BURN_GAUGE);
	int gaugeMask = res.Load(ResourceManager::SRC::GAUGE_MASK_IMG).handleId_;	//�}�X�N
	int gaugeMain = res.Load(ResourceManager::SRC::GAUGE_MAIN_IMG).handleId_;	//���C��
	int gaugeFrame = res.Load(ResourceManager::SRC::GAUGE_FRAME_IMG).handleId_;	//�t���[��
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


	//�t�H���g�̍쐬
	fontHandle_ = CreateFontToHandle(L"NemukeMedium", DISPLAY_TEXT_SIZE, 0);

	// �아�����_���[
	amuletMaterial_->Init();
	amuletRenderer_ = std::make_unique<PixelRenderer>(*amuletMaterial_);
	Vector2 pos = { AMULET_DRAWING_POS_X,AMULET_DRAWING_POS_Y };
	Vector2 size = { AMULET_DRAWING_SIZE_X, AMULET_DRAWING_SIZE_Y };
	amuletRenderer_->MakeSquereVertex(pos, size);

	//�ċp�Q�[�W
	gaugeMaterial_->Init();
	gaugeRenderer_ = std::make_unique<PixelRenderer>(*gaugeMaterial_);
	size = { GAUGE_DRAWING_SIZE_X,GAUGE_DRAWING_SIZE_Y };
	pos = { screenX / 2 - size.x / 2, static_cast<float>(screenY - GAUGE_DRAWING_BOTTOM_DIS_Y )};
	gaugeRenderer_->MakeSquereVertex(pos, size);
}

void PlayerUI::Update(void)
{
	//�R�����g�e�L�X�g�̕`�攻�f
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

	//�아�̉��o���
	if (player_.IsUseAmulet())
	{
		isBurningAmulet_ = true;
	}
	if (isBurningAmulet_)
	{
		amuletBurnTime_ += SceneManager::GetInstance().GetDeltaTime();
		//�萔�o�b�t�@�̍X�V
		amuletMaterial_->SetConstBuf({
			FLOAT4{amuletBurnTime_,AMULET_BURN_COMP_TIME} });
		//����o�̏I��
		if (amuletBurnTime_ >= AMULET_BURN_COMP_TIME)
		{
			amuletBurnTime_ = 0.0f;
			isBurningAmulet_ = false;
		}
	}


	//�ċp���Ԃ̊���
	float par = player_.GetBurnTime() / Player::BURN_END_TIME;
	//�ċp�Q�[�W�̒萔�o�b�t�@�X�V
	gaugeMaterial_->SetConstBuf({ FLOAT4{par,BURN_GAUGE_DRAWING_SIZE_X,BURN_GAUGE_DRAWING_SIZE_Y,0.0f} });
}

void PlayerUI::Draw(void)
{
	//�A�N�V�����e�L�X�g�̕`��
	DrawTextUI();

	//�ċp���Ԃ̕`��
	DrawBurningTime();

	//�아����o
	DrawBurningAmulet();

	//�����A�C�e���̕\��
	DrawHasItem();

	//�Ə��摜
	auto screenX = Application::SCREEN_SIZE_X/2;
	auto screenY = Application::SCREEN_SIZE_Y/2;
	DrawExtendGraph(screenX - CENTER_IMAGE_SIZE, screenY - CENTER_IMAGE_SIZE, 
		screenX + CENTER_IMAGE_SIZE, screenY + CENTER_IMAGE_SIZE, centerImg_, true);

}

void PlayerUI::DrawTextUI(void)
{
	//�e�L�X�g�̕`�悷�郉���_��
	auto drawText = [&](std::wstring text,int font ,int bgImg) 
	{
		auto screenX = Application::SCREEN_SIZE_X;
		auto screenY = Application::SCREEN_SIZE_Y;

		int halfLen = Utility::GetStringLength(
			text, font) / 2;

		//�`����W
		int x, y;
		x = screenX / 2;
		y = screenY - TEXT_DRAWING_BOTTOM_DIS_Y;

		//�e�L�X�g�w�i�摜
		const int offsetX = TEXT_BACKGROUND_OFFSET_DIS_X;
		const int offsetY = TEXT_BACKGROUND_OFFSET_DIS_Y;
		DrawExtendGraph(
			x - halfLen - offsetX, y - offsetY,
			x + halfLen + offsetX, y + offsetY + DISPLAY_TEXT_SIZE,
			bgImg, true);

		x -= halfLen;
		DrawStringToHandle(x, y, text.c_str(), 0xdddddd, font);

	};


	//�e�L�X�g�̕`��
	if (isDisplayCommentText_)
	{
		//�R�����g�e�L�X�g�̕`��
		drawText(commnetText_, fontHandle_, commnetTextBackImg_);
	}
	else if (!player_.GetActionText().empty())
	{
		//�A�N�V�����e�L�X�g�̕`��
		actionText_ = player_.GetActionText();
		drawText(actionText_, fontHandle_, actionTextBackImg_);
	}
}

void PlayerUI::DrawBurningTime(void)
{
	if (player_.GetBurnTime() > 0.0f)
	{
		//�Q�[�W
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

	//�아�̏�������
	pos = { HAS_AMULET_DRAWING_POS_X,HAS_AMULET_DRAWING_POS_Y };
	int diffX = HAS_AMULET_DRAWING_SIZE;
	int size = HAS_AMULET_DRAWING_SIZE;
	int amuletNum = player_.GetAmuletNum();
	//���ɕ��ׂ�
	for (int i = 0; i < amuletNum; i++)
	{
		DrawExtendGraph(
			static_cast<int>(pos.x) + diffX * i, static_cast<int>(pos.y),
			static_cast<int>(pos.x) + diffX * i + size, static_cast<int>(pos.y) + size,
			amiletUiImage_, true);
	}

	//�ʂ�����݂̏������
	pos = { HAS_DOOL_DRAWING_POS_X,HAS_DOOL_DRAWING_POS_Y };
	bool haveDool = player_.HaveTheDoll();
	if (haveDool)
	{
		DrawExtendGraph(
			static_cast<int>(pos.x), static_cast<int>(pos.y),
			static_cast<int>(pos.x) + size, static_cast<int>(pos.y) + size, doolUiImage_, true);
	}
}
