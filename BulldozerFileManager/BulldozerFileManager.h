#pragma once

#ifndef _BULLDOZER_FILE_MANAGER_H__
#define _BULLDOZER_FILE_MANAGER_H__

#ifdef BULLDOZERFILEMANAGER_EXPORTS
#define BULLDOZERFILEMANAGER_API __declspec(dllexport)
#else
#define BULLDOZERFILEMANAGER_API __declspec(dllimport)
#endif

#include <Windows.h>

#include <fstream>
#include <iostream>
#include <string>

void dispErrorMessageBox(LPCWSTR Message);

std::string BULLDOZERFILEMANAGER_API getFileData(std::string filePath);
std::string BULLDOZERFILEMANAGER_API getFileExtension(std::string fileName);

#endif // _BULLDOZER_FILE_MANAGER_H__