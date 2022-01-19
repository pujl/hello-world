#ifndef NETSENDPROCESSTHREAD_H
#define NETSENDPROCESSTHREAD_H

#include <QObject>
class NetSendProcessThread : public QObject
{
    Q_OBJECT
public:
    explicit NetSendProcessThread(QObject *parent = nullptr);

public:
    //const char* name,int devCode,
    void SendNetdataTread(const char* name,int devCode,unsigned char *info,unsigned long sendlen,unsigned char type);

public:
    void SendNetProcessData(unsigned char  DataArray[], unsigned long nSize);
public:
    unsigned char ProcessData[1000];
    unsigned long infolen;

signals:
    //处理完一针完整数据
    void ProcessSendOneFrame(unsigned char  *info, unsigned long sendLen);
};

#endif // NETSENDPROCESSTHREAD_H
