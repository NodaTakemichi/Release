#pragma once
#include<string>
#include <functional>

//ボタン処理関数を指し示すポインタ
using btnProcess = std::function<void(void)>;

class Button
{
public:
	//ボタンテキスト
	std::wstring text_;	
	//ボタン機能
	btnProcess func_;	
};

