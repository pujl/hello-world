#ifndef NETTCPCLIENT_H
#define NETTCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <vector>
#include <string>
#include <QThread>
#include "netrecvprocessthread.h"
#include "netsendprocessthread.h"
using namespace std;
class NetTcpClient : public QTcpSocket
{
    Q_OBJECT
public:
    explicit NetTcpClient(QObject *parent = nullptr);
     ~NetTcpClient();

public:
    QThread *m_pRevthread;//接收线程
    NetRecvProcessThread *m_pNetRevThread;

    QThread *m_pSendthread;//发送线程
    NetSendProcessThread *m_pNetSendThread;
signals:
    void RecvNetDataSignal(const char* name,int devCode,unsigned char *info,unsigned long len,unsigned char type=0);
    //name-设备名称,devCode-设备编号,
    void SendNetDataSignal(const char* name,int devCode,unsigned char *info,unsigned long sendlen,unsigned char type=0);
    //网络连接状态信号
    void SendNetStateSignal();
public slots:
    //向主进程推送一帧完整的数据
    void GetOneFrame();
    //向设备发送一条完整数据
    void SendOneFrame(unsigned char  *info, unsigned long sendLen);
    //通过回调函数向主程序发送网络连接状态
    void SendNetState();
private:
    string m_name;//网络连接名称
    int m_devCode;//设备标识号
public:
    //处理新建TCP网络连接函数
    void dealNewConnection();
public:
    //接收客户端数据
    void OnReceiveData();
    //断开网络连接
    void OnDisConnnected();
    //设置设备名称与设备ID
    void setNameAndDevCode(string name,int devCode);
    //设置网络接收回调函数
    void setNetRevFunc(pNetRevFunc pRevProcessfunc);
    //设置网络接收回调函数
    void setNetStateFunc(pNetRevFunc pNetStatefunc);

    void GetSendOriginal(unsigned char  *info, unsigned long sendLen);

    inline int getDevCode(){return m_devCode;}
    inline string getNetName(){return m_name;}
public:
    unsigned char tempbuf[MAX_TEMP_BUFFER];
    unsigned char sendbuf[MAX_TEMP_BUFFER];

private:
    pNetRevFunc m_pNetRevFunc;
    pNetRevFunc m_pNetStateFunc;
};

#endif // NETTCPCLIENT_H
