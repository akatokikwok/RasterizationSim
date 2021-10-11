#pragma once
#include <chrono>

class GrbTimer
{
public:
	GrbTimer();
	float Mark();// 返回上次mark到现在 所经历的一段时长
	float Peek() const;// 与Mark函数作用一致,只不过它不重设计数器
private:
	std::chrono::steady_clock::time_point last;
};