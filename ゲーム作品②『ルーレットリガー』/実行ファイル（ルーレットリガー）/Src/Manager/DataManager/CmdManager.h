#pragma once
#include<string>
#include<vector>
#include<unordered_map>

struct Parameter
{
	std::string name;		//コマンド名
	std::string type;		//コマンドタイプ(キャスト前)
	std::string target;		//ターゲット(キャスト前)
	float times;			//技：倍率
	std::string buff;		//バフタイプ(キャスト前)
	int efNum;				//エフェクト番号
};

class CmdManager
{

public:
	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static CmdManager& GetInstance(void);

	// 初期化
	void Init(void);

	// 解放
	void Release(void);

	//コマンドデータのロード
	void LoadCmdData(void);

	//指定コマンドの取得（検索）
	const Parameter& GetCmdData(const int& num);

private:
	// 静的インスタンス
	static CmdManager* instance_;

	//ファイル名
	std::string fileName_;
	// コマンドデータ（番号をキー値とする）
	std::unordered_map<int, Parameter> cmdDataMap_;


	// 外部から生成できない様にする
	CmdManager(void);
	// デストラクタも同様
	~CmdManager(void);

};

