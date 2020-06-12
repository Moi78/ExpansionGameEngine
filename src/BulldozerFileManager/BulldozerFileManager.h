#pragma once

#ifndef _BULLDOZER_FILE_MANAGER_H__
#define _BULLDOZER_FILE_MANAGER_H__

#ifdef _WIN32
	#ifdef BULLDOZERFILEMANAGER_EXPORTS
		#define BULLDOZERFILEMANAGER_API __declspec(dllexport)
	#else
		#define BULLDOZERFILEMANAGER_API __declspec(dllimport)
	#endif

	#include <Windows.h>

#else

#define BULLDOZERFILEMANAGER_API
#define TEXT(x) L"x"

#endif //_WIN32

#include <fstream>
#include <iostream>
#include <string>

#ifdef _WIN32
void BULLDOZERFILEMANAGER_API dispErrorMessageBox(LPCWSTR Message);
#else
void dispErrorMessageBox(std::wstring);
#endif

std::string BULLDOZERFILEMANAGER_API getFileData(std::string filePath);
std::string BULLDOZERFILEMANAGER_API getFileExtension(std::string fileName);

#endif // _BULLDOZER_FILE_MANAGER_H__
