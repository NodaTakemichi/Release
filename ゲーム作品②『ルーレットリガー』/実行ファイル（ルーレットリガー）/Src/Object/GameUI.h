#pragma once
#include<string>
#include<vector>
#include"../Common/Vector2.h"

class GameUI
{
public:
	GameUI();
	~GameUI();

	void Init(void);
	void Update(void);
	void Draw(void);
	void DrawActivSkill(void);	//発動技の表示
	void Release(void);

	//コマンド名のセット
	void SetCmdName(std::string name);

	//現在ターンのセット
	void SetTurnString(const std::string& name,const int& color);

private:
	//発動コマンド技の描画
	Vector2 cmdPos_;		//表示座標
	std::string cmdName_;	//コマンドネーム
	int cmdFontHandle_;

	//ターン文字、色、フォント
	std::string turnString_;
	int turnCol_;
	int turnFotnHandle_;


};

