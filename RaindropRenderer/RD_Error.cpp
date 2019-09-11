#include "pch.h"
#include "RD_Error.h"

//Error displayer
void dispErrorMessageBox(LPCWSTR Message) {
	MessageBox(nullptr, Message, TEXT("ERROR !!!"), MB_ICONERROR | MB_OK);
}