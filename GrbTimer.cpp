#include "GrbTimer.h"

using namespace std::chrono;

GrbTimer::GrbTimer()
{
	last = steady_clock::now();
}

float GrbTimer::Mark()
{
	const auto old = last;
	last = steady_clock::now();
	const duration<float> frameTime = last - old;
	return frameTime.count();
}

float GrbTimer::Peek() const
{
	return duration<float>(steady_clock::now() - last).count();
}
