#include <chrono>

float updateTime(std::chrono::_V2::system_clock::time_point *startTime) {
	auto currentTime = std::chrono::high_resolution_clock::now();
	const float delta = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - *startTime).count() / 1000.0f;
	*startTime = currentTime;
	return delta;
}