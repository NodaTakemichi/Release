#include "../../Common/GetAttr.h"

#include "CmdManager.h"

CmdManager* CmdManager::instance_ = nullptr;

void CmdManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new CmdManager();
	}
	instance_->Init();
}

CmdManager& CmdManager::GetInstance(void)
{
	return *instance_;
}

void CmdManager::Init(void)
{
	fileName_ = "./Data/UnitData/CmdTable.xml";
	LoadCmdData();
}

void CmdManager::Release(void)
{
}

void CmdManager::LoadCmdData(void)
{
	//ドキュメントを保存する変数
	rapidxml::xml_document<> actDoc;
	//ファイルのオープン
	rapidxml::file<>xmlFile = fileName_.c_str();
	//解析構文して、変数にデータを格納
	actDoc.parse<0>(xmlFile.data());

	//tmxの情報を取得（中身にアクセス）
	auto cmd = actDoc.first_node("Cmd");
	if (cmd == nullptr)return;

	//アトリビュート取得関数
	auto getAttr = GetAttr();


	//ユニットのコマンド技取得
	std::string name, type, target, buff;
	double times = 0.0;
	int num,efNum;

	for (auto skill = cmd->first_node("Skill");
		skill != nullptr;
		skill = skill->next_sibling())
	{
		//技の番号取得
		if (!getAttr(skill, "num", num))num = 0;
		//技の名前取得
		if (!getAttr(skill, "name", name))name = std::string();
		//技のタイプ取得
		if (!getAttr(skill, "type", type))type = std::string();
		//技の技対象取得
		if (!getAttr(skill, "target", target))target = std::string();
		//技の倍率取得
		if (!getAttr(skill, "times", times))times = 0.0;

		//バフの種類取得
		if (!getAttr(skill, "buff", buff))buff = "NONE";

		//エフェクト番号取得
		if (!getAttr(skill, "efNum", efNum))efNum = 0;


		//コマンドの生成
		Parameter par = {
			name,
			type,
			target,
			static_cast<float>(times),
			buff,
			efNum
		};
		cmdDataMap_.emplace(std::make_pair(num, par));
	}

}

const Parameter& CmdManager::GetCmdData(const int& num)
{
	Parameter& par = cmdDataMap_.at(num);
	return par;
}

CmdManager::CmdManager(void)
{
}

CmdManager::~CmdManager(void)
{
}
