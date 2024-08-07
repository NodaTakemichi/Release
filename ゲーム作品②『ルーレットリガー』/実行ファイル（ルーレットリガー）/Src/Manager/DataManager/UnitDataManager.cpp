#include<DxLib.h>
#include<sstream>
#include "../../Common/GetAttr.h"

#include "UnitDataManager.h"

UnitDataManager* UnitDataManager::instance_ = nullptr;

void UnitDataManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new UnitDataManager();
	}
	instance_->Init();
}

UnitDataManager& UnitDataManager::GetInstance(void)
{
	return *instance_;
}

void UnitDataManager::Init(void)
{
	//ファイル名
	failName_ = "./Data/UnitData/UnitTable.xml";

	//データのロード
	LoadUnitData();
}

void UnitDataManager::Release(void)
{
	unitDataMap_.clear();
}

void UnitDataManager::LoadUnitData(void)
{ 
	//ドキュメントを保存する変数
	rapidxml::xml_document<> actDoc;
	//ファイルのオープン
	rapidxml::file<>xmlFile = failName_.c_str();
	//解析構文して、変数にデータを格納
	actDoc.parse<0>(xmlFile.data());

	//tmxの情報を取得（中身にアクセス）
	auto data = actDoc.first_node("Data");
	if (data == nullptr)return;

	//アトリビュート取得関数
	auto getAttr = GetAttr();

	//データ保持
	std::string name, source;
	int num,hp, attack, speed;
	//コマンド番号配列
	std::vector<int> cmds;


	//各ユニットのデータの取得
	for (auto unit = data->first_node("Unit");
		unit != nullptr;
		unit = unit->next_sibling())
	{

#pragma region ユニットの基本情報
		auto par = unit->first_node();
		if (par == nullptr)return;
		//ユニットの番号取得
		if (!getAttr(par, "num", num))return;

		//ユニットの名前取得
		if (!getAttr(par, "name", name))return;
		//ユニットの画像パス取得
		if (!getAttr(par, "source", source))return;
		//ユニットのHP取得
		if (!getAttr(par, "hp", hp))return;
		//ユニットの攻撃力取得
		if (!getAttr(par, "attack", attack))return;
		//ユニットのスピード取得
		if (!getAttr(par, "speed", speed))return;
#pragma endregion


#pragma region コマンド番号(csv)の取得
		auto csvNode = unit->first_node("Cmd");
		if (csvNode == nullptr)return;

		//csvデータの取得
		//csvデータ格納用の変数宣言
		std::stringstream dataStream;
		//csvの中身を格納
		dataStream << csvNode->value();			//左に向かっデータを流し込む（dataStreamの中身はないから代入じゃなくてコピー）
		//1文字ずつ取り出して、格納していく
		std::string numStr;
		while (std::getline(dataStream, numStr, ','))
		{
			int cmd = std::stoi(numStr, nullptr, 10);	//10進数に変換
			cmds.push_back(cmd);
		}
#pragma endregion

		//画像のロード
		std::string path = UNIT_IMG_PATH + source;
		int handle = LoadGraph(path.c_str());

		//データの挿入
		UnitData paramter = {
			name,
			handle,
			hp,
			attack,
			speed,
			cmds
		};
		unitDataMap_.emplace(std::make_pair(num, paramter));

		//コマンドクリア
		cmds.clear();
	}

}



const UnitData& UnitDataManager::GetUnitData(const int& num)
{
	auto& data = unitDataMap_.at(num);
	return data;
}

const int& UnitDataManager::GetUnitImg(const int& num)
{
	auto& data = unitDataMap_.at(num);
	return data.imgHandle;
}


UnitDataManager::UnitDataManager(void)
{
}

UnitDataManager::~UnitDataManager(void)
{
}

