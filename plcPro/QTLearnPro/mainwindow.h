#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tswnd.h"
#include "../NetDataProcessDLL/netdataprocessdll.h"
#include "plcdev.h"
#include <QCloseEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public:
    void    UpdateSize();
    //回调函数
    NetDataProcessDLL NetDll;
    static void callbackNetRecvData(char *buffer);
    //接收网络连接状态
    static void callbackNetState(char *buffer);
    //处理网络接收函数
    void ProcessNetRevData(char *buffer);
    void ProcessDeviceInfo(unsigned char *info, unsigned long len, unsigned char port);
    //处理网络连接状态函数
    void ProcessNetState(char *buffer);//qichu
    void ProcessNetConnectState(unsigned char DevCode, unsigned char bConnect);

protected:
    void closeEvent(QCloseEvent *event);
private slots:
    //退出程序
    void quitApplication();
    void updateTimerLabel();
    void connectPLC();
    void connDevSmallTimer();
    void recvPLCMSg(quint8 updateId,quint8 devCode);
    void connDevBigTimer();
    //发送命令定时器
    void sendQryCmdTimer();
private:
    QString ReadQssFile(const QString& filePath);
    //初始化设备
    void    InitDevComm();
private:
    Ui::MainWindow *ui;

    TsWnd       *m_TsWnd;
    plcDev      *m_pLCDev;
    //连接设备用
    quint8      m_connDevNum;
    QTimer      *m_connDevSmallTimer;
};
#endif // MAINWINDOW_H
