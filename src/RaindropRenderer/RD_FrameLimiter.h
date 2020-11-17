#pragma once

#ifndef _RD_FRAME_LIMITER_H__
#define _RD_FRAME_LIMITER_H__

#ifdef _WIN32
#ifdef RAINDROPRENDERER_EXPORTS
#define RAINDROPRENDERER_API __declspec(dllexport)
#else
#define RAINDROPRENDERER_API __declspec(dllimport)
#endif

#else

#define RAINDROPRENDERER_API

#endif //_WIN32

#include <ctime>
#include <chrono>
#include <ratio>
#include <iostream>

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

	void WaitAll();

	double GetLastDeltaTime();
private:
	int m_frameLimit;

	std::chrono::milliseconds m_start;
	std::chrono::milliseconds m_stop;

	long long m_last_delta;
};

#endif