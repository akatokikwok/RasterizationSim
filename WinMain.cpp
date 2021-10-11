#include "Window.h"
#include <sstream>

int CALLBACK/*调用约定,即stdcall,参数传栈*/ WinMain(
	HINSTANCE hInstance,/*指向结构指针,例如加载进内存*/
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,/*使用这个命令行来接收别人发来的一些参数,只接受字符串*/
	int       nCmdShow/*关联窗口创建后的展示方式*/
)
{
	try {
		// 填好窗口尺寸和窗口本身名字; 这个类的构造器和析构器负责创建和销毁窗口; PS!!!若调用2次构造器则允许创建2个窗口
		Window wnd(800, 300, "郭的测试窗口-----Box");

		MSG msg;// 需要一个消息结构体,用以保存消息数据
		BOOL gResult;
		while (gResult = GetMessage(&msg, nullptr, 0, 0) > 0) { // 持续获取线程里所有消息(因为非退出消息的其余所有消息均大于0)
			TranslateMessage(&msg);// 此API可能会在适当条件下把WM_KEYDOWN同时转换成了WM_CHAR,如若注释掉,则不会显示WM_CHAR消息
			DispatchMessage(&msg);
			// do app logic (test)
			while (!wnd.mouse.IsEmpty()) {
				const auto e = wnd.mouse.Read();
				if (e.GetType() == Mouse::Event::Type::Move) {// 检查当前鼠标运动状态是否为"移动",若符合则更新窗口标题为光标坐标
					std::ostringstream oss;
					oss << "Mouse Position: (" << e.GetPosX() << "," << e.GetPosY() << ")";
					wnd.SetTitle(oss.str());
				}
			}
		}
		if (gResult == -1) {
			return -1;// 如若gResult值为-1,就返回-1,表明此处有错误
		}

		return msg.wParam;
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