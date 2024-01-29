#include "Precompiled.h"
#include "TimeUtil.h"

float GGEngine::Core::TimeUtil::GetTime()
{
	//gets the difference since the start time
	static const auto startTime = std::chrono::high_resolution_clock::now();
	const auto currentTime = std::chrono::high_resolution_clock::now();
	const auto millisecond = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count();
	return millisecond / 1000.0f;
}

float GGEngine::Core::TimeUtil::GetDeltaTime()
{
	//get the dufference since the last time checked
	static auto lastCallTime = std::chrono::high_resolution_clock::now();
	const auto currentTime = std::chrono::high_resolution_clock::now();
	const auto millisecond = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastCallTime).count();
	lastCallTime = currentTime;
	return millisecond / 1000.0f;
}