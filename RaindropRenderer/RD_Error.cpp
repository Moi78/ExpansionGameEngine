#include "pch.h"
#include "RD_Error.h"

#ifdef _WIN32

//Error displayer
void dispErrorMessageBox(std::wstring Message) {
	std::wcerr << "ERROR:" << Message << std::endl;

	MessageBox(nullptr, Message.c_str(), TEXT("ERROR !!!"), MB_ICONERROR | MB_OK);
}

#else

void dispErrorMessageBox(std::wstring Message) {
	std::wcerr << Message << std::endl;
}

#endif //_WIN32