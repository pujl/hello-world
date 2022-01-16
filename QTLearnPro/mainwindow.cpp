#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QApplication>
#include <QMessageBox>
#include <QFile>
#include <QTimer>
#include <QDateTime>
#include <QPainter>
#include <QPalette>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);

    ui->exitBtn->setText("退出\r\n程序");
    QString strStyle = ReadQssFile(":/MyQss/qss/stylesheet.qss");
    ui->exitBtn->setStyleSheet(strStyle);
    ui->exitBtn->resize(60,60);

    ui->TitleLabel->setText("XXXX淋雨倾斜显示程序");
    ui->TitleLabel->setStyleSheet(strStyle);
    ui->TitleLabel->adjustSize();

    QDateTime curDateTime=QDateTime::currentDateTime();
    ui->TimeLabel->setText(curDateTime.toString("yyyy-MM-dd hh:mm:ss"));
    ui->TimeLabel->setStyleSheet(strStyle);
    ui->TimeLabel->adjustSize();

    QTimer *timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this,&MainWindow::updateTimerLabel);
    timer->start(1000);
    connect(ui->exitBtn,&QPushButton::clicked,this,&MainWindow::quitApplication);

    m_TsWnd = new TsWnd(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::quitApplication()
{
   if(QMessageBox::information(this,tr("exit tip"),tr("Do you really want exit?")))
   {
       QApplication* app;
       app->exit(0);
   }
}

void MainWindow::updateTimerLabel()
{
    QDateTime curDateTime=QDateTime::currentDateTime();
    ui->TimeLabel->setText(curDateTime.toString("yyyy-MM-dd hh:mm:ss"));
}

QString MainWindow::ReadQssFile(const QString& filePath)
{
    QString strStyleSheet = "";
    QFile file(filePath);
    file.open(QFile::ReadOnly);
    if (file.isOpen())
    {
        strStyleSheet = QLatin1String(file.readAll());
    }
    return  strStyleSheet;
}

void MainWindow::UpdateSize()
{
    //exit按钮
    int x = this->geometry().width();
    int y = this->geometry().height();
    ui->exitBtn->move(x-80,y-80);
    //label
    int x1 = ui->TitleLabel->geometry().width();
    int y1 = ui->TitleLabel->geometry().height();
    ui->TitleLabel->move(x/2-x1/2,0);

    int x2 = ui->TimeLabel->geometry().width();
    int y2 = ui->TimeLabel->geometry().height();
    ui->TimeLabel->move(x-x2-10,y1/2-y2/2);

    m_TsWnd->resize(x,y/2);
    m_TsWnd->move(0,y1+50);
//    填充背景色
//    QPalette pal(m_TsWnd->palette());
//    pal.setColor(QPalette::Background,Qt::gray);
//    m_TsWnd->setAutoFillBackground(true);
//    m_TsWnd->setPalette(pal);
    m_TsWnd->show();
}



