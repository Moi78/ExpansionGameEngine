#include "MaterialEditor.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MaterialEditor w;
	w.show();
	return a.exec();
}
