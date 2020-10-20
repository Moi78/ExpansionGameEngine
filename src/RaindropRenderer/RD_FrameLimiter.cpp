#include "pch.h"
#include "RD_FrameLimiter.h"

RD_FrameLimiter::RD_FrameLimiter(int frameLimit) : m_frameLimit(frameLimit) {
	m_last_delta = 0.0f;
}

RD_FrameLimiter::~RD_FrameLimiter() {

}

void RD_FrameLimiter::SetFrameLimit(int nLimit) {
	std::cout << "Setting Frame Limit to " << nLimit << std::endl;
	m_frameLimit = nLimit;
}

int RD_FrameLimiter::GetFrameLimit() {
	return m_frameLimit;
}

void RD_FrameLimiter::start() {
	m_start = high_resolution_clock::now();
}

void RD_FrameLimiter::stop() {
	m_stop = high_resolution_clock::now();

	m_last_delta = std::chrono::duration_cast<std::chrono::milliseconds>(m_stop - m_start).count();
}

double RD_FrameLimiter::GetElapsedTime() {
	double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(m_stop - m_start).count();

	return elapsed;
}

void RD_FrameLimiter::WaitAll() {
	float frameTime = (1.0f / GetFrameLimit() * 1000);
	//std::cout << "Frame time " << GetFrameLimit() << std::endl;
	if (m_last_delta < frameTime) {
		long long delta = (long long)frameTime - GetElapsedTime();

		std::this_thread::sleep_for(std::chrono::milliseconds(delta));
	}
}

double RD_FrameLimiter::GetLastDeltaTime() {
	return m_last_delta;
}