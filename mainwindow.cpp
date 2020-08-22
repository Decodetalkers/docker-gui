#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QProcess>
#include <qdebug.h>
#include <QDebug>
#include <QLabel>
#include <QPushButton>
#include <QLayout>
#include <iostream>
#include <string>
#include <istream>
#include <vector>
#include <sstream>
using namespace std;
vector<string> split(const string& src, string separate_character)
{
    vector<string> strs;
    int separate_characterLen = separate_character.size();//分割字符串的长度,这样就可以支持如“,,”多字符串的分隔符
    int lastPosition = 0, index = -1;
    while (-1 != (index = src.find(separate_character, lastPosition)))
    {
        strs.push_back(src.substr(lastPosition, index - lastPosition));
        lastPosition = index + separate_characterLen;
    }
    string lastString = src.substr(lastPosition);//截取最后一个分隔符后的内容
    if (!lastString.empty())
        strs.push_back(lastString);//如果最后一个分隔符后还有内容就入队
    return strs;
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MainWindow::on_local_pressed();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init(QString word)
{
    pass=word;
    MainWindow::show();
}
QString MainWindow::shell(QString shell)
{
    FILE * fp;
    int res;
    QString  pword;
    char resultStr[100] = {"0"}; //define MAX_USB_NUM 3
    char* ch; QByteArray ba = shell.toLatin1(); ch=ba.data();
    //执行预先设定的命令，并读出该命令的标准输出
    cout<<ch<<endl;
     fp = popen(ch, "r");
     if(!fp)
     {
        perror("popen fail");
        printf("popen error: %s/n", strerror(errno));
        return "";
     }

       /* 循环读管道 #define MAX_USB_NUM 3  */
       while(fgets(resultStr, 3, fp) ){pword+=QString(QLatin1String(resultStr));}
       /*等待命令执行完毕并关闭管道及文件指针*/
       res = pclose(fp);
       if ( res == -1 || pword=="")
       {
           //printf("close popen file pointer fp error!\n");
           return "";
       }
       else
       {
           //printf("popen res is :%d\n", res);
           return pword;

       }
}

void MainWindow::on_back_clicked()
{
    MainWindow::shell("pkexec systemctl stop docker.service");
    emit showfirstwindow(pass); //发出显示主窗口信号
    MainWindow::close();
}

void MainWindow::on_turn_clicked()
{
    MainWindow::shell("echo "+pass+" | sudo -S deepin-editor /etc/docker/daemon.json");
}
char* MainWindow::turned(QString words)
{
    QByteArray ba = words.toLatin1();
    char *first=ba.data();
    ba.clear();
    ba.squeeze();
    return(first);
}

string MainWindow::turnstring(QString words)
{
    QByteArray ba = words.toLatin1();
    string first=ba.data();
    return(first);
}
void MainWindow::test(QString words)
{
    cout<<MainWindow::turned(words)<<endl;
    MainWindow::shell("docker pull "+words);
    MainWindow::on_flash_clicked();
}
void MainWindow::test2(QString words)
{
    cout<<MainWindow::turned(words)<<endl;
    MainWindow::shell("docker rmi "+words);
    MainWindow::on_local_pressed();
}
void MainWindow::remove(QString words)
{
    MainWindow::shell("docker rm "+words);
    MainWindow::on_recent_clicked();
}
void MainWindow::more(QString words)
{
    MainWindow::shell("deepin-terminal -e docker inspect "+words);
}
void MainWindow::restart(QString words)
{
    MainWindow::shell("docker restart "+words);
    MainWindow::on_recent_clicked();
}
void MainWindow::enter(QString words)
{
    string xxx=MainWindow::turnstring(words);
    string yyy=xxx+" /bin/bash";
    QString qstr2 = QString::fromStdString(yyy);
    MainWindow::shell("deepin-terminal -e docker exec -it "+qstr2);
}
void MainWindow::run(QString words)
{
    cout<<MainWindow::turned(words)<<endl;
    string xxx=MainWindow::turnstring(ui->lineEdit_2->text());
    string yyy=MainWindow::turnstring(words);
    string zzz=xxx+yyy;
    QString qstr2 = QString::fromStdString(zzz);
    MainWindow::shell("docker run "+qstr2);
}
void MainWindow::runshellture(QString words)
{
    cout<<MainWindow::turned(words)<<endl;
    string xxx=MainWindow::turnstring(ui->lineEdit_2->text());
    string yyy=MainWindow::turnstring(words);
    string zzz=xxx+yyy;
    QString qstr2 = QString::fromStdString(zzz);
    MainWindow::shell("deepin-terminal -e docker run "+qstr2);
}
void MainWindow::runshell(QString words)
{
    string xxx=MainWindow::turnstring(words);
    string yyy=xxx+" /bin/bash";
    QString qstr2 = QString::fromStdString(yyy);
    MainWindow::shell("deepin-terminal -e docker run -it "+qstr2);
}
void MainWindow::runshellback(QString words)
{
    string xxx = MainWindow::turnstring(words);
    string yyy = xxx+" "+xxx +" /bin/bash";
    QString qstr2 =QString::fromStdString(yyy);
    MainWindow::shell("deepin-terminal -e docker run -itd --name "+qstr2);
}
void MainWindow::on_flash_clicked()
{
    signalMapper = new QSignalMapper(this);
    QList<QLabel* > labs =ui->scrollAreaWidgetContents->findChildren<QLabel*>();
    QList<QPushButton*> btns = ui->scrollAreaWidgetContents->findChildren<QPushButton*>();
    foreach (QPushButton* btn, btns) {   delete btn;  }
    foreach (QLabel* lab, labs) { delete lab;}
    if(ui->lineEdit->text()==""){
        ui->label->setText("你还没有输入呢");
        ui->label->setStyleSheet("color:rgb(255, 0, 0);");
        return;
    }
    else{
        ui->label->setText("输入镜像名字");
        ui->label->setStyleSheet("color:rgb(0, 0, 0);");
        QString search =ui->lineEdit->text();
        ui->grid->setColumnStretch(0,5);
        ui->grid->setColumnStretch(1,5);
        ui->grid->setColumnStretch(2,2);
        ui->grid->setColumnStretch(3,2);
        ui->grid->setMargin(15);
        ui->grid->setColumnMinimumWidth(2, 15);
        MainWindow *main = static_cast<MainWindow*>(parentWidget());
        QString test = shell("docker images | awk -F '\\\\s+ ' '{print $1}'");
        QString dd =main->shell("docker search "+search+" | awk -F '\\\\s+ ' '{print $1}'");
        QString dd2 =main->shell("docker search "+search+" | awk -F '\\\\s+ ' '{print $2}'");
        QString dd3 =main->shell("docker search "+search+" | awk -F '\\\\s+ ' '{print $3}'");
        string name=turnstring(dd);
        string dec =turnstring(dd2);
        string stars =turnstring(dd3);
        string local = turnstring(test);
        //cout<<local<<endl;
        vector<string> strs = split(name,"\n");
        vector<string> strs2 = split(dec,"\n");
        vector<string> strs3 = split(stars,"\n");
        vector<string> strs4 = split(local,"\n");
        int v=0;
        if(strs.size()==1)
        {

            ui->label->setText("什么也没有");
            ui->label->setStyleSheet("color:rgb(255, 0, 0);");
            return;
        }
        for (int i=0;i<strs2.size();i++)
        {
            char yyy[100];
            strcpy(yyy,strs2[i].c_str());
            QLabel* test1=new QLabel;
            test1->setText(yyy);
            ui->grid->addWidget(test1,i,1);
        }
        for (int i=0;i<strs3.size();i++)
        {
            char yyy[100];
            strcpy(yyy,strs3[i].c_str());
            QLabel* test1=new QLabel;
            test1->setText(yyy);
            ui->grid->addWidget(test1,i,2);
        }
        for (int i=0;i<strs.size();i++)
        {
         //cout<<"ss"<<endl;
           char sss[100];

           strcpy(sss,strs[i].c_str());
           QLabel* test1=new QLabel();
           test1->setText(sss);
           QPushButton* test2=new QPushButton(QWidget::tr("下载"));
           ui->grid->addWidget(test1,i,0);
           if (i !=0)
           {
               v=0;
               for(int j=1;j<strs4.size();j++)
               {
                   char yyy[100];
                   strcpy(yyy,strs4[j].c_str());
                   int c= strcmp(sss,yyy);
                   cout<<c<<endl;
                   cout<<yyy<<"  "<<sss<<endl;
                   if(c==0)
                   {
                       v=1;
                       break;
                   }
                   cout<<v<<endl;
               }
               cout<<v<<endl;
               if(v !=1)
               {
                   ui->grid->addWidget(test2,i,3);
                   //cout<<"vv"<<endl;

                   connect(test2,SIGNAL(pressed()),signalMapper,SLOT(map()),Qt::UniqueConnection);
                   //cout<<"vv"<<endl;
                   signalMapper->setMapping(test2,sss);

                   connect(signalMapper,SIGNAL(mapped(QString)),this,SLOT(test(QString)),Qt::UniqueConnection);
                   cout<<i<<" "<<i/3<<" "<<i%3*2<<strs[i]<<endl;
               }

            }
        }

    }
}
void MainWindow::on_local_pressed()//本地信息页面
{
    signalMapper = new QSignalMapper(this);
    signalMapper2 = new QSignalMapper(this);
    signalMapper3 = new QSignalMapper(this);
    signalMapper4 = new QSignalMapper(this);
    signalMapper5 = new QSignalMapper(this);
    signalMapper6 = new QSignalMapper(this);
    ui->stackedWidget->setCurrentIndex(0);
    QList<QPushButton*> btns = ui->scrollAreaWidgetContents_3->findChildren<QPushButton*>();
    QList<QLabel*> labs =ui->scrollAreaWidgetContents_3->findChildren<QLabel*>();
    QList<QLineEdit*> edits=ui->scrollAreaWidgetContents_3->findChildren<QLineEdit*>();
    foreach (QLabel* lab,labs) { delete  lab;}
    foreach (QPushButton* btn, btns) {   delete btn;  }
    foreach (QLineEdit* edi, edits) {   delete edi;  }
    MainWindow *main = static_cast<MainWindow*>(parentWidget());
    ui->gridLayout_2->setColumnStretch(0,4);
    ui->gridLayout_2->setColumnStretch(1,4);
    ui->gridLayout_2->setColumnStretch(2,4);
    ui->gridLayout_2->setColumnStretch(3,4);
    ui->gridLayout_2->setColumnStretch(4,4);
    ui->gridLayout_2->setColumnStretch(5,2);
    ui->gridLayout_2->setColumnStretch(6,2);
    ui->gridLayout_2->setColumnStretch(7,2);
    ui->gridLayout_2->setColumnStretch(8,2);
    ui->gridLayout_2->setColumnStretch(9,2);
    ui->gridLayout_2->setColumnStretch(10,1);
    QString test = main->shell(" docker images | awk -F '\\\\s+ ' '{print $1}'");
    QString test2 = main->shell("docker images | awk -F '\\\\s+ ' '{print $2}'");
    QString test3 = main->shell("docker images | awk -F '\\\\s+ ' '{print $3}'");
    QString test4 = main->shell("docker images | awk -F '\\\\s+ ' '{print $4}'");
    QString test5 = main->shell("docker images | awk -F '\\\\s+ ' '{print $5}'");
    string finall = MainWindow::turnstring(test);
    string finall2 = MainWindow::turnstring(test2);
    string finall3 = MainWindow::turnstring(test3);
    string finall4 = MainWindow::turnstring(test4);
    string finall5 = MainWindow::turnstring(test5);
    cout<<finall2<<finall3<<endl;
    vector<string> strs = split(finall,"\n");
    vector<string> strs2 = split(finall2,"\n");
    vector<string> strs3 = split(finall3,"\n");
    vector<string> strs4 = split(finall4,"\n");
    vector<string> strs5 = split(finall5,"\n");
    for(int i=0;i<strs.size();i++)
    {
        char sss[100];
        strcpy(sss,strs[i].c_str());
        QLabel* test1 = new QLabel;
        test1->setText(sss);
        QPushButton* test2=new QPushButton(QWidget::tr("卸载"));

        ui->gridLayout_2->addWidget(test1,i,0);
        if(i!=0)
        {

            ui->gridLayout_2->addWidget(test2,i,5);
            connect(test2,SIGNAL(pressed()),signalMapper2,SLOT(map()),Qt::UniqueConnection);
            signalMapper2->setMapping(test2,sss);
            connect(signalMapper2,SIGNAL(mapped(QString)),this,SLOT(test2(QString)),Qt::UniqueConnection);
        }

    }
    for(int i=1;i<strs.size();i++)
    {
        char sss[100];
        strcpy(sss,strs[i].c_str());
        QPushButton* test3=new QPushButton(QWidget::tr("运行"));
        QPushButton* test4=new QPushButton(QWidget::tr("增量运行"));
        QPushButton* test5=new QPushButton(QWidget::tr("后台运行"));
        QPushButton* test6=new QPushButton(QWidget::tr("终端运行"));
        ui->gridLayout_2->addWidget(test3,i,6);
        ui->gridLayout_2->addWidget(test4,i,7);
        ui->gridLayout_2->addWidget(test5,i,8);
        ui->gridLayout_2->addWidget(test6,i,9);
        connect(test3,SIGNAL(pressed()),signalMapper,SLOT(map()),Qt::UniqueConnection);
        connect(test4,SIGNAL(pressed()),signalMapper3,SLOT(map()),Qt::UniqueConnection);
        connect(test5,SIGNAL(pressed()),signalMapper4,SLOT(map()),Qt::UniqueConnection);
        connect(test6,SIGNAL(pressed()),signalMapper5,SLOT(map()),Qt::UniqueConnection);
        signalMapper->setMapping(test3,sss);
        signalMapper5->setMapping(test6,sss);
        signalMapper3->setMapping(test4,sss);
        signalMapper4->setMapping(test5,sss);
        connect(signalMapper,SIGNAL(mapped(QString)),this,SLOT(run(QString)),Qt::UniqueConnection);
        connect(signalMapper5,SIGNAL(mapped(QString)),this,SLOT(runshellture(QString)),Qt::UniqueConnection);
        connect(signalMapper3,SIGNAL(mapped(QString)),this,SLOT(runshell(QString)),Qt::UniqueConnection);
        connect(signalMapper4,SIGNAL(mapped(QString)),this,SLOT(runshellback(QString)),Qt::UniqueConnection);
    }
    for(int i=0;i<strs2.size();i++)
    {
        char sss[100];
        strcpy(sss,strs2[i].c_str());
        QLabel* test1 = new QLabel;
        test1->setText(sss);
        ui->gridLayout_2->addWidget(test1,i,1);
    }
    for(int i=0;i<strs3.size();i++)
    {
        char sss[100];
        strcpy(sss,strs3[i].c_str());
        cout<<sss<<endl;
        QLabel* test1 = new QLabel;
        test1->setText(sss);
        ui->gridLayout_2->addWidget(test1,i,2);
    }
    for(int i=0;i<strs4.size();i++)
    {
        char sss[100];
        strcpy(sss,strs4[i].c_str());
        cout<<sss<<endl;
        QLabel* test1 = new QLabel;
        test1->setText(sss);
        ui->gridLayout_2->addWidget(test1,i,3);
    }
    for(int i=0;i<strs5.size();i++)
    {
        char sss[100];
        strcpy(sss,strs5[i].c_str());
        cout<<sss<<endl;
        QLabel* test1 = new QLabel;
        test1->setText(sss);
        ui->gridLayout_2->addWidget(test1,i,4);
    }
}

void MainWindow::on_recent_clicked()//当前运行状态（包括目前的容器）管理
{
    ui->stackedWidget->setCurrentIndex(1);
    signalMapper = new QSignalMapper(this);
    signalMapper2 = new QSignalMapper(this);
    signalMapper3 =new QSignalMapper(this);
    signalMapper4=new QSignalMapper(this);
    QList<QPushButton*> btns = ui->scrollAreaWidgetContents_4->findChildren<QPushButton*>();
    QList<QLabel*> labs =ui->scrollAreaWidgetContents_4->findChildren<QLabel*>();
    foreach (QLabel* lab,labs) { delete  lab;}
    foreach (QPushButton* btn, btns) {   delete btn;  }
    MainWindow *main = static_cast<MainWindow*>(parentWidget());
    ui->gridLayout_2->setColumnStretch(0,5);
    ui->gridLayout_2->setColumnStretch(1,5);
    ui->gridLayout_2->setColumnStretch(2,5);
    ui->gridLayout_2->setColumnStretch(3,5);
    ui->gridLayout_2->setColumnStretch(4,5);
    ui->gridLayout_2->setColumnStretch(5,2);
    ui->gridLayout_2->setColumnStretch(6,2);
    ui->gridLayout_2->setColumnStretch(7,2);
    ui->gridLayout_2->setColumnStretch(8,2);
    QString test = shell("docker ps -a | awk -F '\\\\s+ ' '{print $1}'");
    QString test2 = shell("docker ps -a | awk -F '\\\\s+ ' '{print $2}'");
    QString test3 = shell("docker ps -a | awk -F '\\\\s+ ' '{print $3}'");
    QString test4 = shell("docker ps -a | awk -F '\\\\s+ ' '{print $4}'");
    QString test5 = shell("docker ps -a | awk -F '\\\\s+ ' '{print $5}'");
    string text = turnstring(test);
    string text2 = turnstring(test2);
    string text3 = turnstring(test3);
    string text4 = turnstring(test4);
    string text5 = turnstring(test5);
    vector<string> strs = split(text,"\n");
    vector<string> strs2 = split(text2,"\n");
    vector<string> strs3 = split(text3,"\n");
    vector<string> strs4 = split(text4,"\n");
    vector<string> strs5 = split(text5,"\n");
    for (int i=0 ;i<strs.size();i++)
    {
        char sss[100];
        strcpy(sss,strs[i].c_str());
        QLabel* test1 = new QLabel;
        test1->setText(sss);
        ui->gridLayout_3->addWidget(test1,i,0);
        QPushButton* test2=new QPushButton(QWidget::tr("移除"));
        QPushButton* test3=new QPushButton(QWidget::tr("详细信息"));
        QPushButton* test4=new QPushButton(QWidget::tr("重启容器"));
        QPushButton* test5=new QPushButton(QWidget::tr("进入容器"));
        if(i !=0)
        {
            ui->gridLayout_3->addWidget(test2,i,5);
            ui->gridLayout_3->addWidget(test3,i,6);
            ui->gridLayout_3->addWidget(test4,i,7);
            ui->gridLayout_3->addWidget(test5,i,8);
            cout<<"sss"<<endl;
            connect(test2,SIGNAL(pressed()),signalMapper,SLOT(map()),Qt::UniqueConnection);
            signalMapper->setMapping(test2,sss);
            connect(signalMapper,SIGNAL(mapped(QString)),this,SLOT(remove(QString)),Qt::UniqueConnection);
            connect(test3,SIGNAL(pressed()),signalMapper2,SLOT(map()),Qt::UniqueConnection);
            signalMapper2->setMapping(test3,sss);
            connect(signalMapper2,SIGNAL(mapped(QString)),this,SLOT(more(QString)),Qt::UniqueConnection);
            connect(test4,SIGNAL(pressed()),signalMapper3,SLOT(map()),Qt::UniqueConnection);
            signalMapper3->setMapping(test4,sss);
            connect(signalMapper3,SIGNAL(mapped(QString)),this,SLOT(restart(QString)),Qt::UniqueConnection);
            connect(test5,SIGNAL(pressed()),signalMapper4,SLOT(map()),Qt::UniqueConnection);
            signalMapper4->setMapping(test5,sss);
            connect(signalMapper4,SIGNAL(mapped(QString)),this,SLOT(enter(QString)),Qt::UniqueConnection);
        }

    }
    for (int i=0 ;i<strs2.size();i++)
    {
        char sss[100];
        strcpy(sss,strs2[i].c_str());
        QLabel* test1 = new QLabel;
        test1->setText(sss);
        ui->gridLayout_3->addWidget(test1,i,1);

    }
    for (int i=0 ;i<strs3.size();i++)
    {
        char sss[100];
        strcpy(sss,strs3[i].c_str());
        QLabel* test1 = new QLabel;
        test1->setText(sss);
        ui->gridLayout_3->addWidget(test1,i,2);

    }
    for (int i=0 ;i<strs4.size();i++)
    {
        char sss[100];
        strcpy(sss,strs4[i].c_str());
        QLabel* test1 = new QLabel;
        test1->setText(sss);
        ui->gridLayout_3->addWidget(test1,i,3);

    }
    for (int i=0 ;i<strs5.size();i++)
    {
        char sss[100];
        strcpy(sss,strs5[i].c_str());
        QLabel* test1 = new QLabel;
        test1->setText(sss);
        ui->gridLayout_3->addWidget(test1,i,4);

    }
}

void MainWindow::on_download_pressed()//对新镜像搜索和下载页面
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_pushButton_2_clicked()//对没用的容器做一些清理
{
    MainWindow::shell("pkexec echo y | docker container prune");
}
