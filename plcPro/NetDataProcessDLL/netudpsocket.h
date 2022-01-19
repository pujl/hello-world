#ifndef NETUDPSOCKET_H
#define NETUDPSOCKET_H

#include <QObject>
#include <QUdpSocket>
#include <vector>
#include <string>
#include <QThread>
#include "netrecvprocessthread.h"
#include "netsendprocessthread.h"
using namespace std;
class NetUdpSocket : public QUdpSocket
{
    Q_OBJECT
public:
    explicit NetUdpSocket(QObject *parent = nullptr);
    ~NetUdpSocket();

public:
    QThread *m_pRevthread;//接收线程
    NetRecvProcessThread *m_pNetRevThread;

    QThread *m_pSendthread;//发送线程
    NetSendProcessThread *m_pNetSendThread;
signals:
    void RecvNetDataSignal(const char* name,int devCode,unsigned char *info,unsigned long len,unsigned char type=0);
    //const char* name,int devCode,
    void SendNetDataSignal(const char* name,int devCode,unsigned char *info,unsigned long sendlen,unsigned char type=0);

public slots:
    //向主进程推送一帧完整的数据
    void GetOneFrame();
    //向设备发送一条完整数据
    void SendOneFrame(unsigned char  *info, unsigned long sendLen);

private:
    string m_name;  //网络连接名称
    int m_devCode;  //设备标识号

    string m_PeerIP;            //远程IP地址
    unsigned long m_PeerPort;    //远程端口
public:
    //接收客户端数据
    void OnReceiveData();

    //设置设备名称与设备ID
    void setNameAndDevCode(string name,int devCode);
    //设置远程连接IP与端口号
    void setPeerParam(string peername,unsigned long peerport);
    //设置网络接收回调函数
    void setNetRevFunc(pNetRevFunc pRevProcessfunc);


    void GetSendOriginal(unsigned char  *info, unsigned long sendLen);

    inline int getDevCode(){return m_devCode;}
    inline string getNetName(){return m_name;}
public:
    unsigned char tempbuf[MAX_TEMP_BUFFER];
    unsigned char sendbuf[MAX_TEMP_BUFFER];

private:
    pNetRevFunc m_pNetRevFunc;

};

#endif // NETUDPSOCKET_H
