#include "nettcpclient.h"
#include <QMutex>
vector <NetTcpClient* > TcpClientList;
extern NETINFO ReadNetInfo;
extern NETINFO ConnectNetInfo;
extern QMutex m_ConnectNetCS;
NetTcpClient::NetTcpClient(QObject *parent) : QTcpSocket(parent)
{

    m_pNetRevFunc = NULL;
    m_pNetStateFunc = NULL;

    connect(this,&NetTcpClient::connected,this,&NetTcpClient::dealNewConnection);
    connect(this,&NetTcpClient::readyRead,this,&NetTcpClient::OnReceiveData);
    connect(this,&NetTcpClient::disconnected,this,&NetTcpClient::OnDisConnnected);


    connect(this,&NetTcpClient::SendNetStateSignal,this,&NetTcpClient::SendNetState);

    memset(sendbuf,0,sizeof (sendbuf));
    memset(tempbuf,0,sizeof (tempbuf));



    m_devCode = 0;

    m_pRevthread = NULL;
    m_pNetRevThread = NULL;
    m_pNetRevThread = new NetRecvProcessThread;
    m_pRevthread = new QThread(this);
    m_pNetRevThread->moveToThread(m_pRevthread);

//    connect(m_pRevthread,&QThread::finished,m_pNetRevThread,&NetRecvProcessThread::deleteLater);//删除m_pNetRevThread会出现问题
    connect(this,&NetTcpClient::RecvNetDataSignal,m_pNetRevThread,&NetRecvProcessThread::ProcessRevThread);
    connect(m_pNetRevThread,&NetRecvProcessThread::ProcessRecvOneFrame,this,&NetTcpClient::GetOneFrame);
    m_pRevthread->start();

    m_pSendthread = NULL;
    m_pNetSendThread = NULL;
    m_pNetSendThread = new NetSendProcessThread;
    m_pSendthread = new QThread(this);
    m_pNetSendThread->moveToThread(m_pSendthread);
    connect(this,&NetTcpClient::SendNetDataSignal,m_pNetSendThread,&NetSendProcessThread::SendNetdataTread,Qt::DirectConnection);
    connect(m_pNetSendThread,&NetSendProcessThread::ProcessSendOneFrame,this,&NetTcpClient::SendOneFrame);
    m_pSendthread->start();

}
NetTcpClient::~NetTcpClient()
{
    disconnectFromHost();
    close();
    if(m_pRevthread)
    {
        m_pRevthread->quit();
        m_pRevthread->wait();
        delete m_pRevthread;
        m_pRevthread = NULL;
    }
    if(m_pSendthread)
     {
         m_pSendthread->quit();
         m_pSendthread->wait();
         delete m_pSendthread;
         m_pSendthread = NULL;
     }
    if(m_pNetRevThread)
    {
        delete m_pNetRevThread;
        m_pNetRevThread = NULL;
    }

    if(m_pNetSendThread)
    {
        delete m_pNetSendThread;
        m_pNetSendThread = NULL;
    }
}
void NetTcpClient::setNetRevFunc(pNetRevFunc pRevProcessfunc)
{
    m_pNetRevFunc = pRevProcessfunc;
}
void NetTcpClient::setNetStateFunc(pNetRevFunc pNetStatefunc)
{
    m_pNetStateFunc = pNetStatefunc;
}
void NetTcpClient::dealNewConnection()
{
    unsigned long len=0;
    int i=0;
    int type=0,b_Connect=0;
    len = sizeof(type);
    len += sizeof(b_Connect);

    type = m_devCode;
    b_Connect = 1;

    m_ConnectNetCS.lock();
    unsigned char port =1;
    unsigned long Curlen=(ConnectNetInfo.WritePoint+MAX_SENDNET_BUFFER-ConnectNetInfo.ReadPoint)%MAX_SENDNET_BUFFER;
    if ((MAX_SENDNET_BUFFER-Curlen)<=(len+5))
    {
        m_ConnectNetCS.unlock();
        return;
    }
    unsigned char *tempbuf=NULL;
    tempbuf=new unsigned char [len+5];
    if(NULL==tempbuf)
    {
        m_ConnectNetCS.unlock();
        return ;
    }
    memset(tempbuf,0,len+5);
    memcpy(tempbuf,&len,sizeof(unsigned long));
    memcpy(&tempbuf[4],&port,1);
    memcpy(&tempbuf[5],&type,sizeof(type));
    memcpy(&tempbuf[5+sizeof(type)],&b_Connect,sizeof(b_Connect));
    unsigned long tempwrite=ConnectNetInfo.WritePoint;
    for (i=0;i<int (len+5);i++)
    {
        ConnectNetInfo.pdata[tempwrite]=tempbuf[i];
        tempwrite=(tempwrite+1)%MAX_SENDNET_BUFFER;
    }
    ConnectNetInfo.WritePoint=tempwrite;
    if(tempbuf)
    {
        delete [] tempbuf;
        tempbuf=NULL;
    }
    m_ConnectNetCS.unlock();
    emit SendNetStateSignal();


}
void NetTcpClient::setNameAndDevCode(string name,int devCode)
{
    m_name.clear();
    m_name = name;
    m_devCode = devCode;
}

void NetTcpClient::OnReceiveData()
{
    qint64 recvlen=0;
    recvlen=read((char *)tempbuf,MAX_TEMP_BUFFER);
    if(recvlen>0)
    {
        emit RecvNetDataSignal(m_name.c_str(),m_devCode,tempbuf,(unsigned long)recvlen);
    }
//    ProcessNetRecv();

}

void NetTcpClient::OnDisConnnected()
{
    unsigned long len=0;
    int i=0;
    int type=0,b_Connect=0;
    len = sizeof(type);
    len += sizeof(b_Connect);
    type = m_devCode;
    b_Connect = 0;

    m_ConnectNetCS.lock();
    unsigned char port =1;
    unsigned long Curlen=(ConnectNetInfo.WritePoint+MAX_SENDNET_BUFFER-ConnectNetInfo.ReadPoint)%MAX_SENDNET_BUFFER;
    if ((MAX_SENDNET_BUFFER-Curlen)<=(len+5))
    {
        m_ConnectNetCS.unlock();
        return;
    }
    unsigned char *tempbuf=NULL;
    tempbuf=new unsigned char [len+5];
    if(NULL==tempbuf)
    {
        m_ConnectNetCS.unlock();
        return ;
    }
    memset(tempbuf,0,len+5);
    memcpy(tempbuf,&len,sizeof(unsigned long));
    memcpy(&tempbuf[4],&port,1);
    memcpy(&tempbuf[5],&type,sizeof(type));
    memcpy(&tempbuf[5+sizeof(type)],&b_Connect,sizeof(b_Connect));
    unsigned long tempwrite=ConnectNetInfo.WritePoint;
    for (i=0;i<int (len+5);i++)
    {
        ConnectNetInfo.pdata[tempwrite]=tempbuf[i];
        tempwrite=(tempwrite+1)%MAX_SENDNET_BUFFER;
    }
    ConnectNetInfo.WritePoint=tempwrite;
    if(tempbuf)
    {
        delete [] tempbuf;
        tempbuf=NULL;
    }
    m_ConnectNetCS.unlock();
    emit SendNetStateSignal();
}
void NetTcpClient::GetOneFrame()
{
    if(m_pNetRevFunc)
    {
        m_pNetRevFunc((char *)&ReadNetInfo);
    }

}
void NetTcpClient::SendOneFrame(unsigned char  *info, unsigned long sendLen)
{
    write((char *)info,(quint64)sendLen);

    unsigned char buft[200];
    memset(buft,0,200);
    QString str;
    qDebug()<<m_name.c_str()<<sendLen;
    if (sendLen<100)
    {
        memcpy(buft,info,sendLen);
        QString str;
        char aa[10]={0};
        str+=QString("send= ");
        for(unsigned int j=0;j<sendLen;j++)
        {
            if (j!=sendLen-1)
            {
                sprintf(aa,"%02X ",buft[j]);

            }
            else
                sprintf(aa,"%02X \r\n",buft[j]);
            str+=QString(aa);
        }
        qDebug()<<str.toUtf8().data();
    }

}

void NetTcpClient::GetSendOriginal(unsigned char  *info, unsigned long sendLen)
{
    //
    memcpy(sendbuf,info,sendLen);
    emit SendNetDataSignal(getNetName().c_str(),getDevCode(),sendbuf,sendLen);
}
void NetTcpClient::SendNetState()
{
    if(m_pNetStateFunc)
    {
         m_pNetStateFunc((char *)&ConnectNetInfo);
    }
}
