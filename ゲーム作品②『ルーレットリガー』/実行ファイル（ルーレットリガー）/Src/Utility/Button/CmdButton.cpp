#include <DxLib.h>
#include "../../Manager/InputManager.h"
#include "../../Manager/DataManager/CmdManager.h"
#include "../../Manager/DataManager/SoundManager.h"
#include "../DrawShader.h"
#include "../Utility.h"
#include "CmdButton.h"

//フォントサイズ
constexpr int FONT_SIZE = 12;
//フォントの太さ
constexpr int FONT_THICK = 20;
//フォントカラー
constexpr int FONT_COLOR = 0xdddddd;

//コマンド名オフセット値Y
constexpr int COMMAND_NAME_OFFSET_Y = 14;
//コマンド情報表示サイズ
constexpr int COMMAND_DATA_SHOW_SIZE_X = 300;
constexpr int COMMAND_DATA_SHOW_SIZE_Y = 130;
//コマンド情報表示座標
constexpr int COMMAND_DATA_SHOW_POS_X = 290;
constexpr int COMMAND_DATA_SHOW_POS_Y = 120;
//コマンド情報表示文字サイズ
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
	//背面ボタン
	btnImg_ = LoadGraph("Data/Image/UI/CmdBtn.png");
	//詳細背面
	cmtDetailBack_ = LoadGraph("Data/Image/UI/ModeBtnImg.png");

	frameColorDepth_ = FRAME_COLOR_DEPTH_NORMAL;

	fontHandle_ = CreateFontToHandle("游明朝", FONT_SIZE, FONT_THICK,
		DX_FONTTYPE_ANTIALIASING_4X4);

	//SE
	auto& snd = SoundManager::GetInstance();
	onBtnSE_ = snd.LoadSound(SoundManager::SOUND_TYPE::BTN_ON);
	//ボタン上にマウスが乗っているかどうか
	onBtnBefor_ = false;
	onBtn_ = false;

}

void CmdButton::Update(void)
{
	//SE再生
	PlayOnBtnSE();
}

void CmdButton::Draw(void)
{
	auto& ds = DrawShader::GetInstance();

	//枠
	ds.DrawExtendGraphToShader(
		pos_, size_, btnImg_);

	//コマンド名座標
	Vector2 nPos = { pos_.x,pos_.y + COMMAND_NAME_OFFSET_Y };
	nPos.x += size_.x / 2;		//半分ずらす
	nPos.x -= Utility::CalcStringLength(cmdName_, fontHandle_) / 2;
	//コマンド名
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
	//初期化
	Init();

	pos_ = pos;
	size_ = size;
	cmdNum_ = cmdNum;

	//コマンドデータの取得
	cmdName_ = CmdManager::GetInstance().GetCmdData(cmdNum_).name;
}

bool CmdButton::MouseOnButton(void)
{
	//マウス位置
	auto& ins = InputManager::GetInstance();
	Vector2 mPos = ins.GetMousePos();

	if (IsMouseInRect(mPos, pos_, size_))onBtn_ = true;
	else onBtn_ = false;

	return onBtn_;
}

void CmdButton::SetCmd(const int& cmdNum)
{
	cmdNum_ = cmdNum;

	//コマンド名の取得
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

		//マウス座標
		Vector2 basePos = InputManager::GetInstance().GetMousePos();
		
		//データ
		auto& type  = CmdManager::GetInstance().GetCmdData(cmdNum_).type;
		auto& target = CmdManager::GetInstance().GetCmdData(cmdNum_).target;
		auto& times = CmdManager::GetInstance().GetCmdData(cmdNum_).times;
		auto& buff = CmdManager::GetInstance().GetCmdData(cmdNum_).buff;


		//サイズ
		Vector2 size = { COMMAND_DATA_SHOW_SIZE_X,COMMAND_DATA_SHOW_SIZE_Y };

		//背面画像
		ds.DrawExtendGraphToShader(
			basePos- size, size, cmtDetailBack_);


		Vector2 strPos = basePos - Vector2{ COMMAND_DATA_SHOW_POS_X, COMMAND_DATA_SHOW_POS_Y };
		//名前
		std::string nameStr		= "技名　　　：" + cmdName_;
		DrawStringToHandle(strPos.x, strPos.y, nameStr.c_str(), FONT_COLOR, fontHandle_);

		//タイプ
		strPos.y += COMMAND_DATA_FONT_SIZE;
		std::string typeStr		= "タイプ　　：" + type;
		DrawStringToHandle(strPos.x, strPos.y, typeStr.c_str(), FONT_COLOR, fontHandle_);

		//ターゲット
		if (target != "NONE")
		{
			strPos.y += COMMAND_DATA_FONT_SIZE;
			std::string targetStr = "ターゲット：" + target;
			DrawStringToHandle(strPos.x, strPos.y, targetStr.c_str(), FONT_COLOR, fontHandle_);
		}
		strPos.y += COMMAND_DATA_FONT_SIZE;
		//倍率
		if (times > 0.0f)
		{
			DrawFormatStringToHandle(strPos.x, strPos.y,
				0xdddddd, fontHandle_, "攻撃倍率　：%0.1f", times);
		}

		//バフ
		if (buff != "NONE")
		{
			std::string denoteBuff= ChageDenoteBuffName(buff);
			std::string buffStr = "付与バフ　：" + denoteBuff;
			DrawStringToHandle(strPos.x, strPos.y, buffStr.c_str(), FONT_COLOR, fontHandle_);
		}
	}
}

std::string CmdButton::ChageDenoteBuffName(const std::string& buff)
{
	std::string denoteBuff="";
	if ("PALALYSIS" == buff)	 denoteBuff = "麻痺";
	else if ("POISON" == buff)	 denoteBuff = "毒";
	else if ("CONFUSION" == buff)denoteBuff = "混乱";
	else if ("AVOIDANCE" == buff)denoteBuff = "攻撃技回避";
	else if ("P_UP" == buff)	 denoteBuff = "攻撃力UP";
	else if ("P_DOWN" == buff)	 denoteBuff = "攻撃力DOWN";
	else if ("D_UP" == buff)	 denoteBuff = "防御力UP";
	else if ("D_DOWN" == buff)	 denoteBuff = "防御力DOWN";
	else if ("S_UP" == buff)	 denoteBuff = "速度UP";
	else if ("S_DOWN" == buff)	 denoteBuff = "速度DOWN";


	return denoteBuff;
}
