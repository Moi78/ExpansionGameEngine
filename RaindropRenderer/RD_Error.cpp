#include "pch.h"
#include "RD_Error.h"

#ifdef _WIN32

//Error displayer
void dispErrorMessageBox(LPCWSTR Message) {
	MessageBox(nullptr, Message, TEXT("ERROR !!!"), MB_ICONERROR | MB_OK);
}

#else

void dispErrorMessageBox(std::string Message) {
	std::cerr << Message << std::endl;
}

#endif //_WIN32