#include "ChiliException.h"
#include <sstream>


ChiliException::ChiliException(int line, const char* file) noexcept
	:
	line(line),
	file(file)
{}

/// 拿取char*
const char* ChiliException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< GetOriginString();
	
	whatBuffer = oss.str();
	
	return whatBuffer.c_str();// 拿取char型指针
}

const char* ChiliException::GetType() const noexcept
{
	return "Chili Exception";
}

int ChiliException::GetLine() const noexcept
{
	return line;
}

const std::string& ChiliException::GetFile() const noexcept
{
	return file;
}

/// 使用std::ostringstream格式化 错误文件和错误行数的字符串 并打印输出
std::string ChiliException::GetOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "[File] " << file << std::endl
		<< "[Line] " << line;
	return oss.str();
}