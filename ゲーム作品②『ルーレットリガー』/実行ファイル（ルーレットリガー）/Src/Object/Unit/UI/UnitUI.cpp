#include<DxLib.h>
#include<string>
#include"../../../_debug/_DebugConOut.h"
#include"../../../Manager/SceneManager.h"
#include"../../../Utility/Utility.h"
#include"../../../Utility/DrawShader.h"
#include "UnitUI.h"

//HP�Q�[�W
constexpr int  HP_GAUGE_X = 20;
constexpr int  HP_GAUGE_Y = 120;
//�_���[�W�p�t�H���g
constexpr int DAMAGE_FONT_SIZE = 40;
constexpr int DAMAGE_FONT_THICK = 20;
constexpr int DAMAGE_FONT_COLOR = 0xff0000;
//�񕜗p�t�H���g
constexpr int HEAL_FONT_SIZE = 40;
constexpr int HEAL_FONT_THICK = 20;
constexpr int HEAL_FONT_COLOR = 0xffffff;
//���j�b�g�֘A�p�t�H���g
constexpr int UNIT_FONT_SIZE = 15;
constexpr int UNIT_FONT_THICK = 20;
constexpr int UNIT_FONT_COLOR = 0xe6e6e6;
//HP�ω������o
constexpr int HP_CHANGE_BACK_OFFSET_X = 50;				//�o�b�N�摜�̕\���I�t�Z�b�gX
constexpr int HP_CHANGE_NUM_SHOW_OFFSET_Y = 20;			//�ω�HP�̕\���I�t�Z�b�gY
const Vector2 DAMAGE_VALUE_LOCAL_POS = Vector2{ 0,-5 };	//�_���[�W�ʂ̃��[�J�����W
const Vector2 HEAL_VALUE_LOCAL_POS = Vector2{ -18,18 };	//�q�[���ʂ̃��[�J�����W
//HP�o�[
constexpr int HP_FRAME_SIZE = 1;			//HP�o�[�t���[���̑���
constexpr int HP_NUM_SHOW_OFFSET_Y = 5;		//HP���l�\���I�t�Z�b�g
//���j�b�g��
const Vector2 UNIT_NAME_FRAME_LOCAL_POS = Vector2{ -25,150 };	//���j�b�g���t���[���̃��[�J�����W
const Vector2 UNIT_NAME_LOCAL_POS = Vector2{ 0,157 };			//���j�b�g���̃��[�J�����W
//�o�t�A�C�R��
constexpr int BUFF_ICON_SIZE = 32;					//�o�t�A�C�R���̉摜�T�C�Y
constexpr int BUFF_ICON_SHOW_SPAN_X = 36;			//�o�t�A�C�R���̕\���Ԋu�w
const Vector2 BUFF_ICON_SHOW_LOCAL_POS = Vector2{ 10, 120 };	//�o�t�A�C�R���̃��[�J�����W

//�V�F�[�_�[�^�C�v
using PsType = DrawShader::PS_TYPE;

UnitUI::UnitUI(
	Vector2 pos, std::string& name, int& hp, int& nowHp, int& maxHp, bool& active) :
	unitPos_(pos), name_(name), hp_(hp), nowHp_(nowHp), maxHp_(maxHp), active_(active)
{
	nameFrameImg_= 0;
	targetImg_= 0;
	dmgFrameImg_= 0;
	healFrameImg_= 0;
	dmg_= 0;
	dmgFontHandle_= 0;
	healFontHandle_= 0;
	unitFontHandle_= 0;
	totalTime_=0.0f;
	dmgNumDrawing_=false;	
	healNumDrawing_ = false;
}

UnitUI::~UnitUI()
{
}

void UnitUI::Init(void)
{
	//���O�g,�^�[�Q�b�g�摜�̓o�^
	nameFrameImg_ = LoadGraph("./Data/Image/UI/NameFrame.png");
	targetImg_ = LoadGraph("./Data/Image/UI/target.png");

	//�o�t�A�C�R���̉摜�o�^
	LoadDivGraph(
		"Data/Image/Icon/BuffIcons.png", static_cast<int>(Buff::BUFF_TYPE::MAX),
		1, static_cast<int>(Buff::BUFF_TYPE::MAX),
		BUFF_ICON_SIZE, BUFF_ICON_SIZE, buffIconHandles_);

	//�_���[�W�t���[���摜
	dmgFrameImg_ = LoadGraph("./Data/Image/UI/DmgNum2.png");
	//�_���[�W�`�攻��
	dmgNumDrawing_ = false;	

	//�񕜒l�t���[���摜
	healFrameImg_ = LoadGraph("./Data/Image/UI/HealNum.png");
	//�񕜒l�`�攻��
	healNumDrawing_ = false;	

	//�t�H���g�̓ǂݍ���
	LPCSTR font_path = "./Data/Font/ZeroGothic.otf";
	if(AddFontResourceEx(font_path, FR_PRIVATE, NULL) <= 0)
	{
		//���s
		printf("�t�H���g�f�[�^�̓o�^���s\n");
	}
	//�_���[�W�t�H���g
	dmgFontHandle_ = CreateFontToHandle("��S�V�b�N", DAMAGE_FONT_SIZE, DAMAGE_FONT_THICK, -1,
		DX_FONTTYPE_ANTIALIASING_4X4);
	//�񕜃t�H���g
	healFontHandle_ = CreateFontToHandle("������", HEAL_FONT_SIZE, HEAL_FONT_THICK, -1,
		DX_FONTTYPE_ANTIALIASING_EDGE_4X4, -1, 1);
	//���j�b�g�t�H���g
	unitFontHandle_ = CreateFontToHandle("������", UNIT_FONT_SIZE, UNIT_FONT_THICK, -1,
		DX_FONTTYPE_ANTIALIASING_4X4);


	totalTime_ = 0.0f;

}

void UnitUI::Draw(void)
{
	//HP�ϓ����l�\��
	auto DrawChangeHP = [&](
		int img,int value,int fontHandle,int color, Vector2 offsetPos) {
		//�_���[�W�\�L�摜
		int imgPosX = unitPos_.x - HP_CHANGE_BACK_OFFSET_X;
		int imgPosY = unitPos_.y;
		DrawGraph(imgPosX, imgPosY, img, true);

		//�_���[�W���l
		//������̕��A����
		std::string dmgString = std::to_string(value);
		int halfWidth = Utility::CalcStringLength(dmgString, fontHandle) / 2;
		//�摜�T�C�Y
		int imgSizeX, imgSizeY;
		GetGraphSize(dmgFrameImg_, &imgSizeX, &imgSizeY);
		//�����\�����W
		Vector2 stringPos = {
			imgPosX + (imgSizeX / 2) - halfWidth + offsetPos.x ,
			imgPosY + (imgSizeY / 2) - HP_CHANGE_NUM_SHOW_OFFSET_Y + offsetPos.y ,
		};
		//�����`��
		DrawStringToHandle(
			stringPos.x, stringPos.y, dmgString.c_str(),
			color, fontHandle);
	};

	//�_���[�W���l�̕\��
	if (dmgNumDrawing_)
	{
		DrawChangeHP(dmgFrameImg_, dmg_, dmgFontHandle_, 
			DAMAGE_FONT_COLOR, DAMAGE_VALUE_LOCAL_POS);
	}

	//�񕜗ʕ\��
	if (healNumDrawing_)
	{
		DrawChangeHP(healFrameImg_, dmg_, healFontHandle_, 
			HEAL_FONT_COLOR, HEAL_VALUE_LOCAL_POS);
	}
}

void UnitUI::Release(void)
{
	DeleteGraph(nameFrameImg_);
	DeleteGraph(targetImg_);
	DeleteGraph(dmgFrameImg_);
}

void UnitUI::SetBuff(std::vector<Buff*> buffs)
{
	buffs_ = buffs;
}

void UnitUI::SetDmgValue(const bool& drawing, const int& dmg)
{
	dmgNumDrawing_ = drawing;
	dmg_ = dmg;
}

void UnitUI::SetHealValue(const bool& drawing, const int& heal)
{
	healNumDrawing_ = drawing;
	dmg_ = heal;
}

void UnitUI::DrawHpShader(const Vector2& pos , const COLOR_F& color)
{
	//HP�̊���
	float nowRatio = static_cast<float>(nowHp_) / static_cast<float>(maxHp_);
	float hpRatio = static_cast<float>(hp_) / static_cast<float>(maxHp_);
	COLOR_F subBuf = COLOR_F{
		nowRatio,
		hpRatio
	};

	//�`��
	DrawShader::GetInstance().DrawExtendGraphToShader(
		pos, { HP_GAUGE_X ,HP_GAUGE_Y }, -1, PsType::HP_SHADER, color, subBuf
	);

}

void UnitUI::DrawHpFrame(const Vector2& pos)
{
	//HP�t���[���̕`��
	auto frameSize = HP_FRAME_SIZE;
	DrawBox(pos.x - frameSize, pos.y - frameSize,
		pos.x + HP_GAUGE_X + frameSize, pos.y + HP_GAUGE_Y + frameSize,
		0xffffff, true);

	//HP�̐��l�\��
	DrawFormatStringToHandle(
		pos.x, pos.y + HP_GAUGE_Y + frameSize + HP_NUM_SHOW_OFFSET_Y,
		0xffffff, unitFontHandle_, "HP\n%d", nowHp_);

}

void UnitUI::DrawName(const std::string& name, const Vector2& uPos, const COLOR_F& color)
{
	totalTime_ = SceneManager::GetInstance().GetTotalTime();
	auto unitSize = static_cast<int>(UnitBase::DRAWING_SIZE);	

	//�g���W
	Vector2 framePos = uPos ;			
	framePos += UNIT_NAME_FRAME_LOCAL_POS;		
	//���O�g�̕\��
	//�A�N�e�B�u��Ԃ��ǂ����ŐF��ύX
	COLOR_F c = active_ ? color : COLOR_F{ 1.0f,1.0f,1.0f,1.0f };
	COLOR_F buf = COLOR_F{
		c.r,
		c.g,
		c.b,
		totalTime_
	};
	auto& ds = DrawShader::GetInstance();
	ds.DrawGraphToShader(framePos, nameFrameImg_, PsType::IUMI_FRAME, buf);

	//���O���W
	Vector2 namePos = { };	
	namePos = Vector2{ uPos.x + unitSize / 2, uPos.y } + UNIT_NAME_LOCAL_POS;
	//������́i�����́j�������擾
	auto halfLen = Utility::CalcStringLength(name, unitFontHandle_) / 2;
	//���O�`��
	DrawStringToHandle(
		namePos.x - halfLen, namePos.y,
		name.c_str(), 0xffffff, unitFontHandle_);

}

void UnitUI::DrawBuffIcon()
{
	int count = 0;
	for (auto& buff : buffs_)
	{
		if (!buff->IsAlive())continue;

		//�o�t�̌���
		Buff::BUFF_TYPE iconType= buff->GetBuff();

		const int spanX = BUFF_ICON_SHOW_SPAN_X;
		const Vector2 localPos = BUFF_ICON_SHOW_LOCAL_POS;
		//�o�t�A�C�R�����W
		Vector2 pos = {};
		pos = Vector2{ unitPos_.x + (count * spanX), unitPos_.y } + localPos;

		DrawGraph(pos.x, pos.y, buffIconHandles_[static_cast<int>(iconType)], true);

		count++;
	}
}


