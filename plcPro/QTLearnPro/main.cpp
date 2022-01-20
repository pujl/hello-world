#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowState(w.windowState() ^ Qt::WindowFullScreen);
    w.move(0,0);
    w.show();
    w.UpdateSize();
    return a.exec();
}
