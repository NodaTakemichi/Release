#include <DxLib.h>
#include "../../Manager/DataManager/SoundManager.h"
#include "../../Manager/DataManager/UnitDataManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/InputManager.h"
#include "../DrawShader.h"
#include "../Utility.h"
#include "UnitButton.h"

using PsType = DrawShader::PS_TYPE;

//�t���[���J���[�F�ʏ�
constexpr COLOR_F FRAME_COLOR_NORMAL = { 0.6f, 0.6f, 0.6f, 1.0f };
//�t���[���J���[�F�}�E�X����Ɉʒu���Ă���
constexpr COLOR_F FRAME_COLOR_ON_MOUSE = { 1.0f, 0.9f, 0.0f, 1.0f };
//�t���[���J���[�F�������
constexpr COLOR_F FRAME_COLOR_PUSH_BUTTON = { 1.0f, 0.3f, 0.2f, 1.0f };

//�t�H���g�T�C�Y
constexpr int FONT_SIZE = 10;
//�t�H���g�̑���
constexpr int FONT_THICK = 20;
//�t�H���g�J���[
constexpr int FONT_COLOR = 0x111111;

//�J�[�h�g�I�t�Z�b�g�l
constexpr int CARD_FRAME_OFFSET = 10;
//���j�b�g�摜�I�t�Z�b�g�lY
constexpr int UNIT_IMAGE_OFFSET_Y = 20;
//���j�b�g���O�I�t�Z�b�g�lY
constexpr int UNIT_NAME_OFFSET_Y = 150;


UnitButton::UnitButton()
{
	size_ = Vector2{};
	unitImg_=0;
	name_="";
	fontHandle_=0;
	fColor_=COLOR_F();
}

UnitButton::~UnitButton()
{
}

void UnitButton::Init(void)
{

	//�t�H���g�̓o�^
	fontHandle_ = CreateFontToHandle("������", FONT_SIZE, FONT_THICK,
		DX_FONTTYPE_ANTIALIASING_4X4);

	//�t���[���J���[
	fColor_ = FRAME_COLOR_NORMAL;

	//SE
	auto& snd = SoundManager::GetInstance();
	clickSE_ = snd.LoadSound(SoundManager::SOUND_TYPE::BTN_CLICK);
	onBtnSE_ = snd.LoadSound(SoundManager::SOUND_TYPE::BTN_ON);

	//�{�^����Ƀ}�E�X������Ă��邩�ǂ���
	onBtnBefor_ = false;
	onBtn_ = false;

}

void UnitButton::Update(void)
{
	//SE�Đ�
	PlayOnBtnSE();
	PlayDecSE();

	//�{�^���𗣂���
	if (ButtonDecision())return;

	//�{�^��������
	if (PushButton())
	{
		fColor_ = FRAME_COLOR_PUSH_BUTTON;
		return;
	}

	//�}�E�X���{�^����ɂ���
	if (MouseOnButton())
	{
		fColor_ = FRAME_COLOR_ON_MOUSE;
		return;
	}

	fColor_ = FRAME_COLOR_NORMAL;
	return;

}

void UnitButton::Draw(void)
{
	auto& ds = DrawShader::GetInstance();
	auto totalTime = SceneManager::GetInstance().GetTotalTime();

	//�J�[�h�g
	int offset = CARD_FRAME_OFFSET;
	Vector2 framePos = { pos_.x - offset / 2,pos_.y - offset / 2 };
	Vector2 frameSize = { size_.x + offset,size_.y + offset };
	COLOR_F buf = COLOR_F{ totalTime };
	ds.DrawExtendGraphToShader(framePos, frameSize, -1, PsType::CARD_FRAME, fColor_, buf);

	//�w�ʉ摜
	DrawGraph(pos_.x, pos_.y, backImg_, true);

	//���j�b�g�摜
	ds.DrawExtendGraphToShader({ pos_.x,pos_.y + UNIT_IMAGE_OFFSET_Y }, { size_.x,size_.x }, unitImg_);


	//���S�ʒu
	int center = pos_.x + size_.x / 2;
	//������́i�����́j�������擾
	center -= Utility::CalcStringLength(name_, fontHandle_) / 2;
	DrawStringToHandle(center, pos_.y + UNIT_NAME_OFFSET_Y, name_.c_str(), FONT_COLOR, fontHandle_);
}

void UnitButton::Release(void)
{
	DeleteFontToHandle(fontHandle_);
}

void UnitButton::Create(Vector2 pos, int back, UnitData unit)
{
	//������
	Init();

	pos_ = pos;
	backImg_ = back;
	GetGraphSize(backImg_, &size_.x, &size_.y);

	//�摜
	unitImg_ = unit.imgHandle;
	//���O
	name_ = unit.name;
}

bool UnitButton::MouseOnButton(void)
{
	//�}�E�X�ʒu
	auto& ins = InputManager::GetInstance();
	Vector2 mPos = ins.GetMousePos();

	onBtn_ = false;
	if (IsMouseInRect(mPos, pos_, size_))onBtn_ = true;

	return onBtn_;
}

void UnitButton::ChengeUnitData(const UnitData& data)
{
	//�摜
	unitImg_ = data.imgHandle;
	//���O
	name_ = data.name;
}

bool UnitButton::IsMouseInRect(const Vector2& inside, const Vector2& outside, const Vector2& size)
{
	if (
		inside.x > outside.x &&
		inside.x < outside.x + size.x &&
		inside.y > outside.y &&
		inside.y < outside.y + size.y
		)
	{
		return true;
	}

	return false;
}
