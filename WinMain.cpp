#include "Window.h"

int CALLBACK/*调用约定,即stdcall,参数传栈*/ WinMain(
	HINSTANCE hInstance,/*指向结构指针,例如加载进内存*/
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,/*使用这个命令行来接收别人发来的一些参数,只接受字符串*/
	int       nCmdShow/*关联窗口创建后的展示方式*/
)
{
	Window wnd(800, 300, "Donkey Fart Box");
	MSG msg;// 需要一个消息结构体,用以保存消息数据
	BOOL gResult;
	while (gResult = GetMessage(&msg, nullptr, 0, 0) > 0) { // 持续获取线程里所有消息(因为非退出消息的其余所有消息均大于0)
		TranslateMessage(&msg);// 此API可能会在适当条件下把WM_KEYDOWN同时转换成了WM_CHAR,如若注释掉,则不会显示WM_CHAR消息
		DispatchMessage(&msg);
	}
	if (gResult == -1) {
		return -1;// 如若gResult值为-1,就返回-1,表明此处有错误
	}

	return msg.wParam;
}