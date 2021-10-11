#pragma once
#include "Window.h"
#include "GrbTimer.h"

class App
{
public:
	// 构造应用的时候会自动构造1个窗口实例
	App();
	// master 游戏窗口帧入口 / 消息循环
	int Go();
private:
	// 每一帧游戏逻辑
	void DoFrame();
private:
	Window wnd;// 应用持有1个窗口
	GrbTimer timer;// 应用持有1个计时器
};