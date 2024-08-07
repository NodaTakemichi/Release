#include <DxLib.h>
#include "../Application.h"
#include "GameUI.h"

//�^�[���\���p�t�H���g
constexpr int TURN_FONT_SIZE = 28;
const Vector2 TURN_FONT_SHOE_POS = Vector2{610,30};
//�R�}���h�\���p�t�H���g
constexpr int CMD_FONT_SIZE = 18;
//�R�}���h�{�b�N�X
constexpr int CMD_BOX_FRAME_SIZE = 3;		//�g���̑���
constexpr int CMD_BOX_EXPA_VALUE = 5;		//�g��l

GameUI::GameUI()
{
	cmdPos_ = Vector2{};
	cmdName_="";
	cmdFontHandle_ = 0;
	turnString_="";
	turnCol_=0;
	turnFotnHandle_=0;
}

GameUI::~GameUI()
{
}

void GameUI::Init(void)
{
	//�R�}���h��
	cmdName_ = "";
	cmdPos_ = { 750,190 };

	//���݃^�[���̕���
	turnString_ = "";

	//�t�H���g�̓ǂݍ���
	LPCSTR font_path = "./Data/Font/SoukouMincho.ttf";
	if (AddFontResourceEx(font_path, FR_PRIVATE, NULL) <= 0)
	{
		//���s
		printf("�t�H���g�f�[�^�̓o�^���s\n");
	}
	//�t�H���g�̓o�^
	turnFotnHandle_ = CreateFontToHandle("���b����", TURN_FONT_SIZE, -1,
		DX_FONTTYPE_ANTIALIASING_EDGE_4X4, -1, 1);
	cmdFontHandle_ = CreateFontToHandle("������", CMD_FONT_SIZE, -1,
		DX_FONTTYPE_ANTIALIASING_4X4, -1, 1);

}

void GameUI::Update(void)
{
}

void GameUI::Draw(void)
{
	//���݃^�[����\��
 	auto n = turnString_.c_str();
	//���A�����A�s��
	int sizeX, sizeY, lineCnt;
	GetDrawStringSizeToHandle(
		&sizeX, &sizeY, &lineCnt, n, -1, turnFotnHandle_);

	auto fx = sizeX / 2;
	auto wx = Application::SCREEN_SIZE_X / 2;
	DrawStringToHandle(
		wx - fx, TURN_FONT_SHOE_POS.y, n, turnCol_, turnFotnHandle_, 0xffffff);
	DrawStringToHandle(
		TURN_FONT_SHOE_POS.x, TURN_FONT_SHOE_POS.y, 
		"\n�̃^�[��", 0xffffff, turnFotnHandle_, 0x555555);

}

void GameUI::DrawActivSkill(void)
{

	auto name = cmdName_.c_str();		//����
	auto len = GetStringLength(name);	//������
	//���A�����A�s��
	int sizeX, sizeY, lineCnt;
	GetDrawStringSizeToHandle(
		&sizeX, &sizeY, &lineCnt, name, -1, cmdFontHandle_);

	auto frameSize = CMD_BOX_FRAME_SIZE;	//�g���̑���
	auto boxExSize = CMD_BOX_EXPA_VALUE;	//�{�b�N�X�T�C�Y�̊g��l



	//�g
	DrawBox(
		cmdPos_.x - frameSize, 
		cmdPos_.y - frameSize,
		cmdPos_.x + sizeX + frameSize * 2 + boxExSize,
		cmdPos_.y + sizeY + frameSize * 2 + boxExSize,
		0xffffff, true);
	//�w�i
	DrawBox(
		cmdPos_.x, 
		cmdPos_.y,
		cmdPos_.x + sizeX + frameSize + boxExSize,
		cmdPos_.y + sizeY + frameSize + boxExSize,
		0x0, true);

	//�R�}���h��
	DrawStringToHandle(
		cmdPos_.x + boxExSize, cmdPos_.y + boxExSize,
		cmdName_.c_str(), 0xffffff, cmdFontHandle_);

}


void GameUI::Release(void)
{

}

void GameUI::SetCmdName(std::string name)
{
	cmdName_ = name;
}

void GameUI::SetTurnString(const std::string& name, const int& color)
{
	turnString_ = name;
	turnCol_ = color;
}


