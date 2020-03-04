#pragma once

#ifndef _RD_FRAME_LIMITER_H__
#define _RD_FRAME_LIMITER_H__

#ifdef RAINDROPRENDERER_EXPORTS
#define RAINDROPRENDERER_API __declspec(dllexport)
#else
#define RAINDROPRENDERER_API __declspec(dllimport)
#endif

#include <ctime>
#include <chrono>
#include <ratio>

#include <thread>

using namespace std::chrono;

class RAINDROPRENDERER_API RD_FrameLimiter
{
public:
	RD_FrameLimiter(int frameLimit);
	~RD_FrameLimiter();

	void SetFrameLimit(int nLimit);
	int GetFrameLimit();

	void start();
	void stop();

	double GetElapsedTime();

	void WaitAll(double dur);

	double GetLastDeltaTime();
private:
	int m_frameLimit;

	high_resolution_clock::time_point m_start;
	high_resolution_clock::time_point m_stop;

	double m_last_delta;
};

#endif