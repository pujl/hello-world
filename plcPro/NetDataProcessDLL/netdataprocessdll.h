#ifndef NETDATAPROCESSDLL_H
#define NETDATAPROCESSDLL_H


#include <QtCore/qglobal.h>
#include <QObject>
#include <QTimer>
#include <string>
#include <QtGlobal>
using namespace std;
#if defined(NETDATAPROCESSDLL_LIBRARY)
#  define NETDATAPROCESSDLL_EXPORT Q_DECL_EXPORT
#else
#  define NETDATAPROCESSDLL_EXPORT Q_DECL_IMPORT
#endif
const unsigned long MAX_SENDNET_BUFFER =10*1024;//网络接收数据环行缓冲区字节数
const unsigned long MAX_TEMP_BUFFER=10*1024;//网络接收数据环行缓冲区字节数
const unsigned char START_CHAR = 0x7E;
const unsigned char  END_CHAR = 0xE7;
const unsigned char TRANSPARENT_CHAR = 0xEE;
typedef struct tagNETINFO
{
    unsigned long WritePoint;//写指针
    unsigned long ReadPoint;//读指针
    unsigned char pdata[MAX_SENDNET_BUFFER];//信息的具体内容

}NETINFO,*PNETINFO;

typedef void (*pTIMERFUC)(int,void* pUser);
typedef void (*pNetRevFunc)(char *);//网络接收函数指针
//typedef void (_stdcall * pTIMERFUC)(int,void* pUser);
//typedef void (_stdcall * pNetRevFunc)(char *);//网络接收函数指针
class NETDATAPROCESSDLL_EXPORT NetDataProcessDLL : public QObject
{
    Q_OBJECT
public:
    NetDataProcessDLL();
private:
    QTimer timer;
 public:
    void dealTimer();
    void ctrlTimer(int type);
public:
    void SetTimerFunc(pTIMERFUC ptimerfunc,void* pUser );
public:
    void* m_pUser;
private:
    pTIMERFUC m_pTimerFunc;


};

extern double NETDATAPROCESSDLL_EXPORT addPlus(double a,double b);

extern void NETDATAPROCESSDLL_EXPORT InitNet();
extern int NETDATAPROCESSDLL_EXPORT TCPNetServerSetting(string CommName,int &DevCode,pNetRevFunc netRevFunc,pNetRevFunc netstateFunc);//设置
extern int NETDATAPROCESSDLL_EXPORT SendNetData(unsigned char devCode,unsigned char *buf,unsigned int len);
extern int NETDATAPROCESSDLL_EXPORT TCPNetClientSetting(string name,int &DevCode,pNetRevFunc netRevFunc,pNetRevFunc netstateFunc);
extern int NETDATAPROCESSDLL_EXPORT UDPNetSetting(string name,int &DevCode,pNetRevFunc netRevFunc);//设置
extern void NETDATAPROCESSDLL_EXPORT ExitNetDll();

#endif // NETDATAPROCESSDLL_H
