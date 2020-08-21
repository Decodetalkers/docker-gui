#include "System.h"
#include "ui_system.h"
#include <QKeyEvent>
#include <QProcess>
#include <qdebug.h>
#define T_PrivPtr( o ) (( StationaryLampSet *) o )

System::System(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::System)
{

    ui->setupUi(this);
}

System::~System()
{
    delete ui;
}
void System::on_open_clicked()
{
    if(ui->lineEdit->text()==""){
        ui->label_pass->setText("请输入密码！");
        ui->label_pass->setStyleSheet("color:rgb(255, 0, 0);");
        return;
    }

    //安全的强制类型转换
    MainWindow *main = static_cast<MainWindow*>(parentWidget());

    QString done=main->shell( "echo "+ui->lineEdit->text()+"| sudo -S systemctl start docker.service");
    QString finish=main->shell("systemctl status docker | grep -i Active | awk -F ':' '{print $2}' | awk '{print $1}'");
    qDebug() << finish;
    if(finish=="inactive\n"){
         ui->lineEdit->setText("");
         ui->label_pass->show();
         ui->label_pass->setText("密码错误！");
         ui->label_pass->setStyleSheet("color:rgb(255, 0, 0);");
      }else{
       emit showmainwindow(ui->lineEdit->text()); //发出显示主窗口信号
       qDebug()<<ui->lineEdit->text();
       System::close();
    }

}
void System::on_exit_clicked()
{
    System::close();
}
void System::back(QString word)
{
    pass=word;
    System::show();
}
