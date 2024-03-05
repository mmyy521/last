#include <QApplication>
#include "main_window.h"
#include "loginwidget.h"
#include<QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.loginldap();

    return a.exec();
}
