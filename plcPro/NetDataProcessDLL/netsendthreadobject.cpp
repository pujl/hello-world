#include "netsendthreadobject.h"
#include "netdataprocessdll.h"
NetSendThreadObject::NetSendThreadObject(QObject *parent) : QObject(parent)
{

}
void NetSendThreadObject::SendNetdataTread(unsigned char *info,unsigned long sendlen,unsigned char type)
{

//    SendMutex.lock();
    SendNetProcessData(info,sendlen);
//    SendMutex.unlock();


}
void NetSendThreadObject::SendNetProcessData(unsigned char  DataArray[], unsigned long nSize)
{

    unsigned char ProcessData[1000];
    unsigned long infolen=0;
    memset(ProcessData,0,sizeof(ProcessData));
    unsigned long i=0;
    unsigned char Validate=0x00;

    ProcessData[infolen++]=START_CHAR;
    for(i=0;i<nSize;i++)
    {
        if(DataArray[i]==START_CHAR||DataArray[i]==TRANSPARENT_CHAR||DataArray[i]==END_CHAR)
        {
            ProcessData[infolen]=TRANSPARENT_CHAR;
            infolen++;
            ProcessData[infolen]=DataArray[i];
            infolen++;
        }
        else
        {
            ProcessData[infolen]=DataArray[i];
            infolen++;
        }
        Validate=Validate^(DataArray[i]);
    }
    if(TRANSPARENT_CHAR==Validate||START_CHAR==Validate||END_CHAR==Validate)//判别校验字节是否为帧头帧尾或透明符
    {
        ProcessData[infolen]=TRANSPARENT_CHAR;
        infolen++;
        ProcessData[infolen]=Validate;
        infolen++;
    }
    else
    {
        ProcessData[infolen]=Validate;
        infolen++;
    }

    ProcessData[infolen]=END_CHAR;
    infolen++;
    emit ProcessSendOneFrame(ProcessData,infolen);
//    m_pClientSocket->write((char *)ProcessData,(quint64)infolen);


}

