#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QApplication>
#include <QMessageBox>
#include <QFile>

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

    connect(ui->exitBtn,&QPushButton::clicked,this,&MainWindow::QuitApplication);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::QuitApplication()
{
   if(QMessageBox::information(this,tr("exit tip"),tr("Do you really want exit?")))
   {
       QApplication* app;
       app->exit(0);
   }
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
    int x = this->geometry().width();
    int y = this->geometry().height();
    ui->exitBtn->move(x-80,y-80);
}


