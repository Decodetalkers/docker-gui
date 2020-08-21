#include "mainwindow.h"
#include "System.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    System s;
    MainWindow p;
    QObject::connect(&s,SIGNAL(showmainwindow(QString)),&p,SLOT(init(QString)));//显示主窗口信号
    QObject::connect(&p,SIGNAL(showfirstwindow(QString)),&s,SLOT(back(QString)));
    QObject::connect(&p,SIGNAL(quit()),&a,SLOT(quit()));
    s.show();
    s.setWindowTitle("test");
    s.resize(400,300);
    return a.exec();
}
