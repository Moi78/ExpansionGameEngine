#pragma once

#ifndef _RD_CALLBACK_H__
#define _RD_CALLBACK_H__

#ifdef RAINDROPRENDERER_EXPORTS
#define RAINDROPRENDERER_API __declspec(dllexport)
#else
#define RAINDROPRENDERER_API __declspec(dllimport)
#endif


#include <iostream>
#include <functional>

#define VDFUNCPTR(x) std::bind(&x)
#define CL_VCFUNCPTR(x) std::bind(&x, this)

class RAINDROPRENDERER_API RD_Callback
{
public:
	RD_Callback(std::function<void()> f);
	~RD_Callback();

	void Call();

private:
	std::function<void()> m_f;
};

#endif //_RD_CALLBACK_H__