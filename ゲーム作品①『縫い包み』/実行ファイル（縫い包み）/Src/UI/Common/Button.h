#pragma once
#include<string>
#include <functional>

//�{�^�������֐����w�������|�C���^
using btnProcess = std::function<void(void)>;

class Button
{
public:
	//�{�^���e�L�X�g
	std::wstring text_;	
	//�{�^���@�\
	btnProcess func_;	
};

