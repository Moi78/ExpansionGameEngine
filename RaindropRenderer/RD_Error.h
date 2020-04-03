#pragma once

#ifndef _RD_ERROR_H__
#define _RD_ERROR_H__

#ifdef _WIN32

#include <Windows.h>

//Error displayer
void dispErrorMessageBox(LPCWSTR Message);

#else

#include <iostream>

void dispErrorMessageBox(std::string);

#endif //_WIN32

#endif