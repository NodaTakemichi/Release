#include <DxLib.h>
#include "../../Manager/InputManager.h"
#include "../../Manager/DataManager/CmdManager.h"
#include "../../Manager/DataManager/SoundManager.h"
#include "../DrawShader.h"
#include "../Utility.h"
#include "CmdButton.h"

//�t�H���g�T�C�Y
constexpr int FONT_SIZE = 12;
//�t�H���g�̑���
constexpr int FONT_THICK = 20;
//�t�H���g�J���[
constexpr int FONT_COLOR = 0xdddddd;

//�R�}���h���I�t�Z�b�g�lY
constexpr int COMMAND_NAME_OFFSET_Y = 14;
//�R�}���h���\���T�C�Y
constexpr int COMMAND_DATA_SHOW_SIZE_X = 300;
constexpr int COMMAND_DATA_SHOW_SIZE_Y = 130;
//�R�}���h���\�����W
constexpr int COMMAND_DATA_SHOW_POS_X = 290;
constexpr int COMMAND_DATA_SHOW_POS_Y = 120;
//�R�}���h���\�������T�C�Y
constexpr int COMMAND_DATA_FONT_SIZE = 32;


CmdButton::CmdButton()
{
	size_ = Vector2{};
	cmdNum_=0;
	cmdName_="";
	cmtDetailBack_ = 0;
	fontHandle_ = 0;
	btnImg_ = 0;

}

CmdButton::~CmdButton()
{
}

void CmdButton::Init(void)
{
	//�w�ʃ{�^��
	btnImg_ = LoadGraph("Data/Image/UI/CmdBtn.png");
	//�ڍהw��
	cmtDetailBack_ = LoadGraph("Data/Image/UI/ModeBtnImg.png");

	frameColorDepth_ = FRAME_COLOR_DEPTH_NORMAL;

	fontHandle_ = CreateFontToHandle("������", FONT_SIZE, FONT_THICK,
		DX_FONTTYPE_ANTIALIASING_4X4);

	//SE
	auto& snd = SoundManager::GetInstance();
	onBtnSE_ = snd.LoadSound(SoundManager::SOUND_TYPE::BTN_ON);
	//�{�^����Ƀ}�E�X������Ă��邩�ǂ���
	onBtnBefor_ = false;
	onBtn_ = false;

}

void CmdButton::Update(void)
{
	//SE�Đ�
	PlayOnBtnSE();
}

void CmdButton::Draw(void)
{
	auto& ds = DrawShader::GetInstance();

	//�g
	ds.DrawExtendGraphToShader(
		pos_, size_, btnImg_);

	//�R�}���h�����W
	Vector2 nPos = { pos_.x,pos_.y + COMMAND_NAME_OFFSET_Y };
	nPos.x += size_.x / 2;		//�������炷
	nPos.x -= Utility::CalcStringLength(cmdName_, fontHandle_) / 2;
	//�R�}���h��
	DrawStringToHandle(
		nPos.x, nPos.y, cmdName_.c_str(), 
		FONT_COLOR, fontHandle_);

}

void CmdButton::Release(void)
{
	DeleteGraph(btnImg_);
}

void CmdButton::Create(const Vector2& pos,const Vector2& size, const int& cmdNum)
{
	//������
	Init();

	pos_ = pos;
	size_ = size;
	cmdNum_ = cmdNum;

	//�R�}���h�f�[�^�̎擾
	cmdName_ = CmdManager::GetInstance().GetCmdData(cmdNum_).name;
}

bool CmdButton::MouseOnButton(void)
{
	//�}�E�X�ʒu
	auto& ins = InputManager::GetInstance();
	Vector2 mPos = ins.GetMousePos();

	if (IsMouseInRect(mPos, pos_, size_))onBtn_ = true;
	else onBtn_ = false;

	return onBtn_;
}

void CmdButton::SetCmd(const int& cmdNum)
{
	cmdNum_ = cmdNum;

	//�R�}���h���̎擾
	cmdName_ = CmdManager::GetInstance().GetCmdData(cmdNum).name;
}

bool CmdButton::IsMouseInRect(const Vector2& inside, const Vector2& outside, const Vector2& size)
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

void CmdButton::DrawCmdDetail(void)
{
	if (MouseOnButton())
	{
		auto& ds = DrawShader::GetInstance();

		//�}�E�X���W
		Vector2 basePos = InputManager::GetInstance().GetMousePos();
		
		//�f�[�^
		auto& type  = CmdManager::GetInstance().GetCmdData(cmdNum_).type;
		auto& target = CmdManager::GetInstance().GetCmdData(cmdNum_).target;
		auto& times = CmdManager::GetInstance().GetCmdData(cmdNum_).times;
		auto& buff = CmdManager::GetInstance().GetCmdData(cmdNum_).buff;


		//�T�C�Y
		Vector2 size = { COMMAND_DATA_SHOW_SIZE_X,COMMAND_DATA_SHOW_SIZE_Y };

		//�w�ʉ摜
		ds.DrawExtendGraphToShader(
			basePos- size, size, cmtDetailBack_);


		Vector2 strPos = basePos - Vector2{ COMMAND_DATA_SHOW_POS_X, COMMAND_DATA_SHOW_POS_Y };
		//���O
		std::string nameStr		= "�Z���@�@�@�F" + cmdName_;
		DrawStringToHandle(strPos.x, strPos.y, nameStr.c_str(), FONT_COLOR, fontHandle_);

		//�^�C�v
		strPos.y += COMMAND_DATA_FONT_SIZE;
		std::string typeStr		= "�^�C�v�@�@�F" + type;
		DrawStringToHandle(strPos.x, strPos.y, typeStr.c_str(), FONT_COLOR, fontHandle_);

		//�^�[�Q�b�g
		if (target != "NONE")
		{
			strPos.y += COMMAND_DATA_FONT_SIZE;
			std::string targetStr = "�^�[�Q�b�g�F" + target;
			DrawStringToHandle(strPos.x, strPos.y, targetStr.c_str(), FONT_COLOR, fontHandle_);
		}
		strPos.y += COMMAND_DATA_FONT_SIZE;
		//�{��
		if (times > 0.0f)
		{
			DrawFormatStringToHandle(strPos.x, strPos.y,
				0xdddddd, fontHandle_, "�U���{���@�F%0.1f", times);
		}

		//�o�t
		if (buff != "NONE")
		{
			std::string denoteBuff= ChageDenoteBuffName(buff);
			std::string buffStr = "�t�^�o�t�@�F" + denoteBuff;
			DrawStringToHandle(strPos.x, strPos.y, buffStr.c_str(), FONT_COLOR, fontHandle_);
		}
	}
}

std::string CmdButton::ChageDenoteBuffName(const std::string& buff)
{
	std::string denoteBuff="";
	if ("PALALYSIS" == buff)	 denoteBuff = "���";
	else if ("POISON" == buff)	 denoteBuff = "��";
	else if ("CONFUSION" == buff)denoteBuff = "����";
	else if ("AVOIDANCE" == buff)denoteBuff = "�U���Z���";
	else if ("P_UP" == buff)	 denoteBuff = "�U����UP";
	else if ("P_DOWN" == buff)	 denoteBuff = "�U����DOWN";
	else if ("D_UP" == buff)	 denoteBuff = "�h���UP";
	else if ("D_DOWN" == buff)	 denoteBuff = "�h���DOWN";
	else if ("S_UP" == buff)	 denoteBuff = "���xUP";
	else if ("S_DOWN" == buff)	 denoteBuff = "���xDOWN";


	return denoteBuff;
}
