#pragma once
#include <exception>
#include <string>

/* 
	所有异常将继承的基类
*/
class ChiliException : public std::exception
{
public:
	ChiliException(int line, const char* file) noexcept;
	const char* what() const noexcept override;  // 错误信息 从这个接口 被打印出来
	virtual const char* GetType() const noexcept;// 错误类型 从这个接口 被打印出来
	int GetLine() const noexcept;				 // 打印行数
	const std::string& GetFile() const noexcept; // 打印文件
	std::string GetOriginString() const noexcept;// 把错误信息格式化为字符串,然后输出
private:
	int line;		 // 错误出现在哪一行
	std::string file;// 错误出现在哪一个文件
protected:
	mutable std::string whatBuffer;// 仅用于承载 std::ostringstream对象的.str(), 由于是保护继承, 所以任何异常子类都可以使用访问它
};