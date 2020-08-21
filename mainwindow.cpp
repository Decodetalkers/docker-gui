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
    MainWindow::shell("echo "+pass+" | sudo -S systemctl stop docker.service");
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
}
void MainWindow::test2(QString words)
{
    cout<<MainWindow::turned(words)<<endl;
    MainWindow::shell("docker rmi "+words);
}
void MainWindow::remove(QString words)
{
    MainWindow::shell("docker rm "+words);
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
        QString dd =main->shell("docker search "+search+" | awk -F '\\\\s+ ' '{print $1}'");
        QString dd2 =main->shell("docker search "+search+" | awk -F '\\\\s+ ' '{print $2}'");
        QString dd3 =main->shell("docker search "+search+" | awk -F '\\\\s+ ' '{print $3}'");
        string name=turnstring(dd);
        string dec =turnstring(dd2);
        string stars =turnstring(dd3);
        vector<string> strs = split(name,"\n");
        vector<string> strs2 = split(dec,"\n");
        vector<string> strs3 = split(stars,"\n");
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
           cout<<"ss"<<endl;
           char sss[100];

           strcpy(sss,strs[i].c_str());
           QLabel* test1=new QLabel();
           test1->setText(sss);
           QPushButton* test2=new QPushButton(QWidget::tr("下载"));
           ui->grid->addWidget(test1,i,0);
           ui->grid->addWidget(test2,i,3);
           cout<<"vv"<<endl;

           connect(test2,SIGNAL(pressed()),signalMapper,SLOT(map()),Qt::UniqueConnection);
           cout<<"vv"<<endl;
           signalMapper->setMapping(test2,sss);

           connect(signalMapper,SIGNAL(mapped(QString)),this,SLOT(test(QString)),Qt::UniqueConnection);
           cout<<i<<" "<<i/3<<" "<<i%3*2<<strs[i]<<endl;

        }

    }
}
void MainWindow::on_local_pressed()//本地信息页面
{
    signalMapper = new QSignalMapper(this);
    signalMapper2 = new QSignalMapper(this);
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
    ui->gridLayout_2->setColumnStretch(7,6);
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
        QLineEdit* test4 = new QLineEdit;
        ui->gridLayout_2->addWidget(test3,i,6);
        ui->gridLayout_2->addWidget(test4,i,7);
        string q=turnstring(test4->text());
        string ssss=q+" "+string(sss);
        char ff[100];
        strcpy(ff,ssss.c_str());
        connect(test3,SIGNAL(pressed()),signalMapper,SLOT(map()),Qt::UniqueConnection);
        signalMapper->setMapping(test3,ff);
        cout<<ff<<endl;
        connect(signalMapper,SIGNAL(mapped(QString)),this,SLOT(run(QString)),Qt::UniqueConnection);
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
        if(i !=0)
        {
            ui->gridLayout_3->addWidget(test2,i,5);
            cout<<"sss"<<endl;
            connect(test2,SIGNAL(pressed()),signalMapper,SLOT(map()),Qt::UniqueConnection);
            signalMapper->setMapping(test2,sss);
            connect(signalMapper,SIGNAL(mapped(QString)),this,SLOT(remove(QString)),Qt::UniqueConnection);
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
