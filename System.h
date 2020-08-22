#ifndef SYSTEM_H
#define SYSTEM_H
#include <QWidget>
#include "mainwindow.h"
namespace Ui {
class System;
}

class System : public QWidget
{
    Q_OBJECT

public:
    explicit System(QWidget *parent = nullptr);
    ~System();

signals:

    void showmainwindow(QString);//显示主窗口信号
    void quit();//退出信号

private slots:
    void on_open_clicked();
    void on_exit_clicked();
    void back(QString);


    void on_pushButton_show_clicked();

private:
    Ui::System *ui;
    QString pass;

};

#endif // MAINWINDOW_H
