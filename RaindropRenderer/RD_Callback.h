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

class RAINDROPRENDERER_API RD_Callback
{
public:
	RD_Callback();
	~RD_Callback();
};

#endif //_RD_CALLBACK_H__