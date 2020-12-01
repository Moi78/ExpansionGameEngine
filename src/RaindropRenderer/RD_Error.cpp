#include "pch.h"
#include "RD_Error.h"

#ifdef _WIN32

//Error displayer
void dispErrorMessageBox(std::wstring Message) {
	std::wcerr << "ERROR:" << Message << std::endl;

	MessageBox(nullptr, Message.c_str(), TEXT("ERROR !!!"), MB_ICONERROR | MB_OK);
}

std::wstring StrToWStr(std::string str) {
	std::wstring wstr(str.begin(), str.end());
	return wstr;
}

#else

void dispErrorMessageBox(std::wstring Message) {
	std::wcerr << Message << std::endl;
}

std::wstring StrToWStr(std::string str) {
	std::wstring wstr(str.begin(), str.end());
	return wstr;
}

#endif //_WIN32
