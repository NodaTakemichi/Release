#pragma once
#include<vector>
#include<string>
class Command;

class Roulette
{
public:

	Roulette();
	~Roulette();

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	//ルーレットを止める指示出し関数
	void StopRoulette(const bool& autom);		//true：自動、false：手動

	//ルーレットにコマンド技をセットする
	void SetCommand(std::vector<Command*> cmd);

	//ルーレットの回転状況をリセットする
	void ResetRouSpin(void);

	//ルーレットの停止状態の取得関数
	bool GetRouStop(void) { return isStop_; }

	//決定したコマンドの取得
	Command* GetCmd(void);

private:

	
	int rouletteImg_;	//ルーレット画像
	int rouFrame_;		//枠
	int center_;		//中央
	//矢印画像
	int arrowImg_;

	//回転角度
	double angle_;
	//回転量
	double rotPower_;

	//ルーレットの回転状況
	bool isRouSpin_;
	//ルーレットが停止状態か判断
	bool isStop_;

	//フォントハンドル
	int fontHandle_;

	//コマンド技
	std::vector<Command*>cmdNames_;

	//待機時間
	float totalTime_;

	//SE関連
	int rotateSE_;
	int stopingSE_;
	int stopBtnSE_;

	//ルーレットの回転処理
	void RotateProcess(void);


};

