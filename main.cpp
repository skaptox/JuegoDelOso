//  Copyright (c) 2017 Richard Games. All rights reserved.


#include <QApplication>
#include <QDesktopWidget>

#include "./gamewindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    GameWindow w;
    w.show();
    return a.exec();
}
