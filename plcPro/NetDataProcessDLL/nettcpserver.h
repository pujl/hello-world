#ifndef NETTCPSERVER_H
#define NETTCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <vector>
#include <string>
#include "netdataprocessdll.h"
#include <QThread>
#include "netrecvprocessthread.h"
#include "netsendprocessthread.h"
using namespace std;
class NetTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit NetTcpServer(QObject *parent = nullptr);
    ~NetTcpServer();
public:
    //处理新建TCP网络连接函数
    void dealNewConnection();
public:
    QTcpSocket *m_pClientSocket;
    QThread *m_pRevthread;//接收线程
    NetRecvProcessThread *m_pNetRevThread;

    QThread *m_pSendthread;//发送线程
    NetSendProcessThread *m_pNetSendThread;
signals:
    void RecvNetDataSignal(const char* name,int devCode,unsigned char *info,unsigned long len,unsigned char type=0);
    //const char* name,int devCode,
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
    void disconnectclient();
private:
    string m_name;//网络连接名称
    int m_devCode;//设备标识号
public:
    //接收客户端数据
    void OnReceiveData();
    //断开网络连接
    void OnDisConnnected();
    //设置设备名称与设备ID
    void setNameAndDevCode(string name,int devCode);
    //监控端口
    bool Listen(quint16 port,QString LocalIP);

    //设置网络接收回调函数
    void setNetRevFunc(pNetRevFunc pRevProcessfunc);
    //设置网络接收回调函数
    void setNetStateFunc(pNetRevFunc pNetStatefunc);
    //打包发送网络数据
    void SendNetProcessData(unsigned char  DataArray[], unsigned long nSize);

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

#endif // NETTCPSERVER_H
