#include "FBX2MSH.h"
#include <QtWidgets/QApplication>

#ifdef _DEBUG
	#include <Windows.h>
	#include <stdio.h>
#endif //_DEBUG

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	#ifdef _DEBUG
		FreeConsole();
		AllocConsole();
		AttachConsole(GetCurrentProcessId());

		freopen("CON", "w", stdout);
		freopen("CON", "w", stderr);
		freopen("CON", "r", stdin);
	#endif //_DEBUG

	FBX2MSH w;
	w.show();
	return a.exec();
}
