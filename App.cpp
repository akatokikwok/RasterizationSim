#include "App.h"
#include <sstream>
#include <iomanip>

App::App()
	: wnd(800, 600, "郭的测试窗口-----Box")// 填好窗口尺寸和窗口本身名字; 这个类的构造器和析构器负责创建和销毁窗口; PS!!!若调用2次构造器则允许创建2个窗口
{}

int App::Go()
{
	while (true) {
		// 持续监测消息,这个静态方法返回一个optional<int>
		if (const auto ecode = Window::ProcessMessages()) { // 如若ecode有值,则佐证 此时正在退出,即走进了WM_Quit(返回true),非WM_QUIT消息则返回false,进不来这个if
			// if return optional has value, means we're quitting so return exit code
			return *ecode;
		}

		DoFrame();// 如若没收到消息,亦或是退出消息,则正常执行帧逻辑
	}
}

void App::DoFrame()
{
	const float t = timer.Peek();
	std::ostringstream oss;
	oss << "Time elapsed: " << std::setprecision(1) << std::fixed << t << "s";
	wnd.SetTitle(oss.str());
}