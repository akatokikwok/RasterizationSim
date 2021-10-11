#include "App.h"
#include <sstream>
#include <iomanip>

App::App()
	: wnd(800, 600, "郭的测试窗口-----Box")// 填好窗口尺寸和窗口本身名字; 这个类的构造器和析构器负责创建和销毁窗口; PS!!!若调用2次构造器则允许创建2个窗口
{}

int App::Go()
{
	MSG msg;
	BOOL gResult;
	while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0) { // 持续检测消息,科普!: GetMessage方法在没有消息的时候它自身并不会工作
		// TranslateMessage will post auxilliary WM_CHAR messages from key msgs
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		DoFrame();
	}

	// check if GetMessage call itself borked
	if (gResult == -1) {
		throw CHWND_LAST_EXCEPT();
	}

	// wParam here is the value passed to PostQuitMessage
	return msg.wParam;
}

void App::DoFrame()
{
	const float t = timer.Peek();
	std::ostringstream oss;
	oss << "Time elapsed: " << std::setprecision(1) << std::fixed << t << "s";
	wnd.SetTitle(oss.str());
}