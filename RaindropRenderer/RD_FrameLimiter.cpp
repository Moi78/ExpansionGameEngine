#include "pch.h"
#include "RD_FrameLimiter.h"

RD_FrameLimiter::RD_FrameLimiter(int frameLimit) : m_frameLimit(frameLimit) {
	m_last_delta = 0.0f;
}

RD_FrameLimiter::~RD_FrameLimiter() {

}

void RD_FrameLimiter::SetFrameLimit(int nLimit) {
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

	m_last_delta = duration<double>(m_stop - m_start).count();
}

double RD_FrameLimiter::GetElapsedTime() {
	duration<double> elapsed = m_stop - m_start;

	return elapsed.count();
}

void RD_FrameLimiter::WaitAll(double dur) {
	
}

double RD_FrameLimiter::GetLastDeltaTime() {
	return m_last_delta;
}