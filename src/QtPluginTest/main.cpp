#include <QApplication>
#include <QMainWindow>

#include <iostream>

#include <EXPGE_Widget.h>

int main(int argc, char* argv[]) {
    QApplication qapp{argc, argv};

    QEXPGE* expge = new QEXPGE();
    if(!expge->InitEngine()) {
        std::cout << "FAILED" << std::endl;
    }

    QMainWindow mwin{};
    mwin.show();

    return qapp.exec();
}