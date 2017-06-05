#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName(ORGNAME);
    a.setApplicationName(APPNAME);
    MainWindow w;
    w.show();

    return a.exec();
}
