#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <iostream>
#include <istream>
#include <vector>
#include <sstream>
#include <qsignalmapper.h>
using namespace std;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString shell(QString shell);
signals:
    void quit();//退出信号
    void showfirstwindow(QString);

private slots:
    void test(QString);
    void test2(QString);
    void remove(QString);
    void more(QString);
    void restart(QString);
    void enter(QString);
    void run(QString);
    void runshell(QString);
    void runshellback(QString);
    void runshellture(QString);
    void init(QString);
    void on_back_clicked();

    void on_turn_clicked();

    void on_flash_clicked();
    char* turned(QString words);
    string turnstring(QString words);


    void on_local_pressed();

    void on_recent_clicked();

    void on_download_pressed();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    QString pass;
    QString uuu;
    QSignalMapper *signalMapper;
    QSignalMapper *signalMapper2;
    QSignalMapper *signalMapper3;
    QSignalMapper *signalMapper4;
    QSignalMapper *signalMapper5;
    QSignalMapper *signalMapper6;
};

#endif // MAINWINDOW_H
