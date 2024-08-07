#pragma once
#include<string>
#include<vector>
#include<unordered_map>

//ユニット画像のパス
const std::string UNIT_IMG_PATH = "Data/Image/Unit/";

//ユニットのデータ
struct UnitData
{
	//int num;					//ユニット番号
	std::string name;			//名前
	int imgHandle;				//画像ハンドル
	int hp;						//体力
	int attack;					//攻撃力
	int speed;					//素早さ
	std::vector<int> cmdNum;	//コマンド番号
};

class UnitDataManager
{
public:


	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static UnitDataManager& GetInstance(void);

	// 初期化
	void Init(void);
	// 解放
	void Release(void);

	//ユニットデータのロード
	void LoadUnitData(void);

	//指定ユニットの取得（検索）
	const UnitData& GetUnitData(const int& num);

	//指定ユニットの画像取得
	const int& GetUnitImg(const int& num);

private:
	// 静的インスタンス
	static UnitDataManager* instance_;

	//ファイル名
	std::string failName_;

	// ユニットデータ（番号をキー値とする）
	std::unordered_map<int,UnitData> unitDataMap_;


	// 外部から生成できない様にする
	UnitDataManager(void);
	// デストラクタも同様
	~UnitDataManager(void);


};

