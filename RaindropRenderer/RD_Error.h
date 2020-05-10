#pragma once

#ifndef _RD_ERROR_H__
#define _RD_ERROR_H__

#ifdef _WIN32

#include <Windows.h>
#include <iostream>
#include <string>

//Error displayer
void dispErrorMessageBox(std::wstring Message);

#else

#include <iostream>

void dispErrorMessageBox(std::wstring message);

#endif //_WIN32

#endif
