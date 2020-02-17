#include "MaterialEditor.h"
#include <QtWidgets/QApplication>

#include <vec3.h>

#include <BulldozerFileManager.h>
#include <BD_StructMan.h>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MaterialEditor* w = new MaterialEditor();
	w->show();

	return a.exec();
}
