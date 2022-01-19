#ifndef NETRECVPROCESSTHREAD_H
#define NETRECVPROCESSTHREAD_H
#include "netdataprocessdll.h"
#include <QObject>
//网络接收处理线程
class NetRecvProcessThread : public QObject
{
    Q_OBJECT
public:
    explicit NetRecvProcessThread(QObject *parent = nullptr);

public:
    //type -1 服务器 -2 客户端 -3 无连接(udp)
    void ProcessRevThread(const char* name,int devCode,unsigned char *info,unsigned long recvlen,unsigned char type);

public:
    //解析数据函数，分格式
    void ProcessNetRecv_7E(int devCode,unsigned char *info,unsigned long recvlen,unsigned char type);
    void ProcessNetRecv_NONE(int devCode,unsigned char *info,unsigned long recvlen,unsigned char type);
    //处理函数
    void ExpandBuffer(unsigned char *buffer, unsigned long len, unsigned char port);
signals:
    //处理完一针完整数据
    void ProcessRecvOneFrame();

public:
    unsigned char Read_Buf[MAX_TEMP_BUFFER];
    unsigned char Temp_Buf[MAX_TEMP_BUFFER];
    unsigned long Read_Point;
    unsigned long Write_Point;
    unsigned long Frame_Lenth;
    bool Find_Start;//找到信息头
    unsigned char m_bTranschar;//透明符标志位

};

#endif // NETRECVPROCESSTHREAD_H
