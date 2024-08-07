#include <DxLib.h>
#include <cassert>
#include "StringUtility.h"

std::wstring StringUtility::StringToWstring(const std::string& str)
{
    std::wstring ret;
    //1�x�ڂ̕ϊ��͕����񐔂�m�邽��
    auto result = MultiByteToWideChar(
        CP_UTF8,
        0,
        str.c_str(), //���͕�����
        static_cast<int>(str.length()),
        nullptr,
        0);
    assert(result >= 0);
    ret.resize(result); //�m�ۂ���

    //2�x�ڂ̌Ăяo���͕ϊ�
    result = MultiByteToWideChar(
        CP_UTF8,
        0,
        str.c_str(), //���͕�����
        static_cast<int>(str.length()),
        ret.data(),
        static_cast<int>(ret.size()));

    return ret;
}

std::string StringUtility::WstringToString(const std::wstring& wstr)
{
    std::string ret;
    //1�x�ڂ̕ϊ��͕����񐔂�m�邽��
    auto result = WideCharToMultiByte(
        CP_ACP,
        0,
        wstr.c_str(), //���͕�����
        static_cast<int>(wstr.length()),
        nullptr,
        0,
        nullptr,
        nullptr);
    assert(result >= 0);
    ret.resize(result); //�m�ۂ���

    //2�x�ڂ̌Ăяo���͕ϊ�
    result = WideCharToMultiByte(
        CP_ACP,
        0,
        wstr.c_str(), //���͕�����
        static_cast<int>(wstr.length()),
        ret.data(),
        static_cast<int>(ret.size()),
        nullptr,
        nullptr);

    return ret;
}
