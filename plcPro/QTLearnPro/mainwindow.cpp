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
#include <QDebug>

const quint8 DEVNUM_MAX = 1;

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

    ui->connectPLCBtn->setStyleSheet(strStyle);
    ui->connectPLCBtn->resize(60,60);
    ui->connectPLCBtn->hide();

    ui->TitleLabel->setText("XXXX淋雨倾斜显示程序");
    ui->TitleLabel->setStyleSheet(strStyle);
    ui->TitleLabel->adjustSize();

    QDateTime curDateTime=QDateTime::currentDateTime();
    ui->TimeLabel->setText(curDateTime.toString("yyyy-MM-dd hh:mm:ss"));
    ui->TimeLabel->setStyleSheet(strStyle);
    ui->TimeLabel->adjustSize();

    ui->exampleGrp->setStyleSheet(strStyle);
    ui->exampleGrp->resize(450,350);
    ui->stopStripExamp->resize(200,50);
    ui->stopStripExamp->setStyleSheet(strStyle);
    ui->normalStripExamp->resize(200,50);
    ui->normalStripExamp->setStyleSheet(strStyle);
    ui->errStripExamp->resize(200,50);
    ui->errStripExamp->setStyleSheet(strStyle);

    ui->stopTxt->setStyleSheet(strStyle);
    ui->stopTxt->adjustSize();
    ui->normalTxt->setStyleSheet(strStyle);
    ui->normalTxt->adjustSize();
    ui->errTxt->setStyleSheet(strStyle);
    ui->errTxt->adjustSize();

    ui->StateGrp->setStyleSheet(strStyle);
    ui->StateGrp->resize(450,350);

    ui->commStateTxt->setStyleSheet(strStyle);
    ui->commStateTxt->adjustSize();
    ui->allErrTxt->setStyleSheet(strStyle);
    ui->allErrTxt->adjustSize();
    ui->lockStateTxt->setStyleSheet(strStyle);
    ui->lockStateTxt->adjustSize();
    ui->PLCRunTxt->setStyleSheet(strStyle);
    ui->PLCRunTxt->adjustSize();

    QTimer *labeltimer = new QTimer(this);
    labeltimer->start(1000);
    connect(labeltimer,&QTimer::timeout,this,&MainWindow::updateTimerLabel);

    connect(ui->exitBtn,&QPushButton::clicked,this,&MainWindow::quitApplication);
    connect(ui->connectPLCBtn,&QPushButton::clicked,this,&MainWindow::connectPLC);

    //初始化设备
    InitDevComm();
    //定义设备连接定时器
    m_connDevSmallTimer = new QTimer(this);
    m_connDevSmallTimer->setInterval(100);
    m_connDevSmallTimer->start();
    m_connDevNum = 0;
    connect(m_connDevSmallTimer,&QTimer::timeout,this,&MainWindow::connDevSmallTimer);
    connect(m_pLCDev,SIGNAL(updateDevMsg(quint8,quint8)),this,SLOT(recvPLCMSg(quint8,quint8)));

    QTimer *connDevBigTimer = new QTimer(this);
    connDevBigTimer->setInterval(6000);
    connDevBigTimer->start();
    connect(connDevBigTimer,&QTimer::timeout,this,&MainWindow::connDevBigTimer);

    QTimer *sendQryTimer = new QTimer(this);
    sendQryTimer->setInterval(100);
    sendQryTimer->start();
    connect(sendQryTimer,&QTimer::timeout,this,&MainWindow::sendQryCmdTimer);

    m_TsWnd = new TsWnd(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::quitApplication()
{
//    if(QMessageBox::information(this,tr("exit tip"),tr("Do you really want exit?")))
//    {
//        QApplication* app;
//        app->quit();

//    }
    //触发closeEvent消息，进行主窗口关闭
    close();
}

void MainWindow::updateTimerLabel()
{
    QDateTime curDateTime=QDateTime::currentDateTime();
    ui->TimeLabel->setText(curDateTime.toString("yyyy-MM-dd hh:mm:ss"));
}

void MainWindow::connectPLC()
{
    int devCode = 0;
    if(m_pLCDev->GetDevLink() != LINK_SUCC)
    {
        TCPNetClientSetting(m_pLCDev->GetDevName().toStdString(),devCode,(pNetRevFunc)callbackNetRecvData,(pNetRevFunc)callbackNetState);
    }else
    {
        //设备已连接
    }
}

void MainWindow::connDevSmallTimer()
{
    m_connDevNum++;
    switch(m_connDevNum)
    {
    case 1:
        connectPLC();
        break;
    default:
        m_connDevNum = 0;
        m_connDevSmallTimer->stop();
        break;
    }
}

void MainWindow::recvPLCMSg(quint8 updateId,quint8 devCode)
{
    switch(updateId)
    {
    case DEV_LINK_MSG:
    {
        switch(devCode)
        {
        case DEVID_PLC:
        {
            //设备连接消息
            if(m_pLCDev->GetDevLink())
            {
                //成功
                qDebug()<<QString::fromLocal8Bit("PLC连接成功")<<endl;
            }else
            {
                //失败
                qDebug()<<QString::fromLocal8Bit("PLC断开连接")<<endl;
            }
        }
            break;
        default:
            break;
        }
    }
        break;
    case PLC_MSGINIT:
        break;
    case PLC_MSGCHANGE:
        break;
    case PLC_MSGRLOCK:
        break;
    case PLC_MSGRALLERR:
        break;
    case PLC_MSGRUN:
        break;
    default:
        break;
    }
}

void MainWindow::connDevBigTimer()
{
    if(m_connDevNum == 0)
    {
        //重新连接
        m_connDevSmallTimer->start();
    }
}

void MainWindow::sendQryCmdTimer()
{
    m_pLCDev->SendQryCmd();
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

void MainWindow::InitDevComm()
{
    InitNet();

    m_pLCDev = new plcDev(this);
    m_pLCDev->SetDevName(DEVNAME_PLC);
    m_pLCDev->SetDevCode(DEVID_PLC);
}

void MainWindow::UpdateSize()
{
    //exit按钮
    int x = this->geometry().width();
    int y = this->geometry().height();
    ui->exitBtn->move(x-80,y-80);

    ui->connectPLCBtn->move(x-80,y-200);
    //label
    int x1 = ui->TitleLabel->geometry().width();
    int y1 = ui->TitleLabel->geometry().height();
    ui->TitleLabel->move(x/2-x1/2,0);

    int x2 = ui->TimeLabel->geometry().width();
    int y2 = ui->TimeLabel->geometry().height();
    ui->TimeLabel->move(x-x2-10,y1/2-y2/2);

    quint32 topSpace = y/6;
    ui->exampleGrp->move(150,topSpace*4-20);

    ui->stopStripExamp->move(20,80);
    ui->normalStripExamp->move(20,170);
    ui->errStripExamp->move(20,260);

    ui->stopTxt->move(245,80);
    ui->normalTxt->move(245,170);
    ui->errTxt->move(245,260);

    ui->StateGrp->move(750,topSpace*4-20);
    ui->commStateTxt->move(20,50);
    ui->allErrTxt->move(20,130);
    ui->lockStateTxt->move(20,210);
    ui->PLCRunTxt->move(20,290);

    m_TsWnd->resize(x,y/2);
    m_TsWnd->move(0,y1+50);
//    填充背景色
//    QPalette pal(m_TsWnd->palette());
//    pal.setColor(QPalette::Background,Qt::gray);
//    m_TsWnd->setAutoFillBackground(true);
//    m_TsWnd->setPalette(pal);
    m_TsWnd->show();
}

void MainWindow::callbackNetRecvData(char *buffer)
{
    foreach(QWidget *w,qApp->topLevelWidgets())
    {
        if(MainWindow *mainWin=qobject_cast<MainWindow *>(w))
        {
            mainWin->ProcessNetRevData(buffer);
        }
    }
}

void MainWindow::callbackNetState(char *buffer)
{
    foreach(QWidget *w,qApp->topLevelWidgets())
    {
        if(MainWindow *mainWin=qobject_cast<MainWindow *>(w))
        {
            mainWin->ProcessNetState(buffer);
        }
    }
}

void MainWindow::ProcessNetRevData(char *buffer)
{
    NETINFO *pNetData;
    pNetData=(NETINFO *)buffer;
    while(pNetData->ReadPoint!=pNetData->WritePoint)
    {
        unsigned long infolen=0;
        unsigned char lenbuf[4];
        memset(lenbuf,0,4);
        unsigned char *pData=NULL;
        unsigned char port=0;
        unsigned long tempread=0;
        unsigned long i=0;
        tempread=pNetData->ReadPoint;
        for (i=0;i<4;i++)
        {
            lenbuf[i]=pNetData->pdata[tempread];
            tempread=(tempread+1)%MAX_SENDNET_BUFFER;
        }
        memcpy(&infolen,&lenbuf,4);

        port=pNetData->pdata[tempread];
        tempread=(tempread+1)%MAX_SENDNET_BUFFER;

        pData=new unsigned char [infolen];
        memset(pData,0,infolen);
        for (i=0;i<infolen;i++)
        {
            pData[i]=pNetData->pdata[tempread];
            tempread=(tempread+1)%MAX_SENDNET_BUFFER;
        }
        ProcessDeviceInfo(pData, infolen, port);
        pNetData->ReadPoint=tempread;
        if(pData)
        {
            delete pData;
            pData = NULL;
        }
    }
}

void MainWindow::ProcessDeviceInfo(unsigned char *info, unsigned long len, unsigned char port)
{
    unsigned char buft[200];
    memset(buft,0,200);
    QString str;
    static int topnum=0;
    topnum+=len+3;

    if (port==m_pLCDev->GetDevCode())
    {
        m_pLCDev->TranslateMsg(info,len);
    }
}

void MainWindow::ProcessNetState(char *buffer)
{
    NETINFO *pNetData;
    pNetData=(NETINFO *)buffer;
    if(NULL==pNetData)
        return ;
    while(pNetData->ReadPoint!=pNetData->WritePoint)
    {
        unsigned long infolen=0;
        unsigned char lenbuf[4];
        memset(lenbuf,0,4);
        unsigned char *pData=NULL;
      //  unsigned char port=0;
        unsigned long tempread=0;
        unsigned long i=0;
        tempread=pNetData->ReadPoint;
        for (i=0;i<4;i++)
        {
            lenbuf[i]=pNetData->pdata[tempread];
            tempread=(tempread+1)%MAX_SENDNET_BUFFER;
        }
        memcpy(&infolen,&lenbuf,4);

       // port=pNetData->pdata[tempread];
        tempread=(tempread+1)%MAX_SENDNET_BUFFER;
        if(infolen<8)
        {
            return ;
        }
        pData=new unsigned char [infolen];
        if(NULL==pData)
        {
            return ;
        }
        memset(pData,0,infolen);
        for (i=0;i<infolen;i++)
        {
            pData[i]=pNetData->pdata[tempread];
            tempread=(tempread+1)%MAX_SENDNET_BUFFER;
        }
        int type,m_Connect;
        memcpy(&type,pData,sizeof(type));
        memcpy(&m_Connect,&pData[sizeof(type)],sizeof(m_Connect));
        ProcessNetConnectState(type,m_Connect);
        pNetData->ReadPoint=tempread;
        if(pData)
        {
            delete [] pData;
            pData = NULL;
        }
    }
}

void MainWindow::ProcessNetConnectState(unsigned char DevCode, unsigned char bConnect)
{
    if(DevCode== m_pLCDev->GetDevCode())
    {
        m_pLCDev->SetDevLink((DEV_LINK_STATE)bConnect);
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    //退出事件
    QMessageBox::StandardButton result=QMessageBox::question(this,"确认","确定退出软件？",QMessageBox::Yes|QMessageBox::No,QMessageBox::No);
    if(QMessageBox::Yes==result)
    {
        event->accept();
        ExitNetDll();
        //处理退出清理
    }
    else
        event->ignore();
}



