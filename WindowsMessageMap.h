#pragma once
#include <unordered_map>
#include <Windows.h>

class WindowsMessageMap
{
public:
	WindowsMessageMap();
	std::string operator()(DWORD msg, LPARAM lp, WPARAM wp) const;
private:
	std::unordered_map<DWORD, std::string> map;//无序MAP,把消息ID映射到字符串
};