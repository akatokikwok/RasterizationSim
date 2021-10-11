#include "Window.h"
#include "App.h"
#include <sstream>

int CALLBACK/*调用约定,即stdcall,参数传栈*/ WinMain(
	HINSTANCE hInstance,/*指向结构指针,例如加载进内存*/
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,/*使用这个命令行来接收别人发来的一些参数,只接受字符串*/
	int       nCmdShow/*关联窗口创建后的展示方式*/
)
{
	try {
		return App{}.Go();
	}
	catch (const ChiliException& e) {
		MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e) {
		MessageBox(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...) {
		MessageBox(nullptr, "No details available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;


}