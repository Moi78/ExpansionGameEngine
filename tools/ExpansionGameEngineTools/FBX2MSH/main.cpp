#include "FBX2MSH.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FBX2MSH w;
    w.show();
    return a.exec();
}
