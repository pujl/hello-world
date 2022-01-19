#include "nettcpserver.h"
#include <QMutex>
vector <NetTcpServer* > TcpServerList;
extern NETINFO ReadNetInfo;
extern NETINFO ConnectNetInfo;
QMutex m_ConnectNetCS;
NetTcpServer::NetTcpServer(QObject *parent) : QTcpServer(parent)
{
    m_pClientSocket = NULL;
    m_pNetRevFunc = NULL;
    m_pNetStateFunc = NULL;
    connect(this,&NetTcpServer::newConnection,this,&NetTcpServer::dealNewConnection);

    connect(this,&NetTcpServer::SendNetStateSignal,this,&NetTcpServer::SendNetState);

    memset(sendbuf,0,sizeof (sendbuf));
    memset(tempbuf,0,sizeof (tempbuf));



    m_devCode = 0;

    m_pRevthread = NULL;
    m_pNetRevThread = NULL;
    m_pNetRevThread = new NetRecvProcessThread;
    m_pRevthread = new QThread(this);
    m_pNetRevThread->moveToThread(m_pRevthread);

//    connect(m_pRevthread,&QThread::finished,m_pNetRevThread,&NetRecvProcessThread::deleteLater);//删除m_pNetRevThread会出现问题
    connect(this,&NetTcpServer::RecvNetDataSignal,m_pNetRevThread,&NetRecvProcessThread::ProcessRevThread);
    connect(m_pNetRevThread,&NetRecvProcessThread::ProcessRecvOneFrame,this,&NetTcpServer::GetOneFrame);
    m_pRevthread->start();

    m_pSendthread = NULL;
    m_pNetSendThread = NULL;
    m_pNetSendThread = new NetSendProcessThread;
    m_pSendthread = new QThread(this);
    m_pNetSendThread->moveToThread(m_pSendthread);
    connect(this,&NetTcpServer::SendNetDataSignal,m_pNetSendThread,&NetSendProcessThread::SendNetdataTread,Qt::DirectConnection);
    connect(m_pNetSendThread,&NetSendProcessThread::ProcessSendOneFrame,this,&NetTcpServer::SendOneFrame);
    m_pSendthread->start();

}
NetTcpServer::~NetTcpServer()
{

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
void NetTcpServer::disconnectclient()
{
//    if(m_pClientSocket)
//    {
//        m_pClientSocket->disconnectFromHost();
//        m_pClientSocket->close();
//        m_pClientSocket = NULL;
//    }

}
void NetTcpServer::setNetRevFunc(pNetRevFunc pRevProcessfunc)
{
    m_pNetRevFunc = pRevProcessfunc;
}
void NetTcpServer::setNetStateFunc(pNetRevFunc pNetStatefunc)
{
    m_pNetStateFunc = pNetStatefunc;
}
bool NetTcpServer::Listen(quint16  port,QString LocalIP)
{
    if(isListening())
    {
        close();
    }
    return listen(QHostAddress(LocalIP),port);


}
void NetTcpServer::dealNewConnection()
{
    if(m_pClientSocket)
    {
        m_pClientSocket->disconnectFromHost();
        m_pClientSocket->close();
        m_pClientSocket = NULL;
    }
    m_pClientSocket =nextPendingConnection();
    connect(m_pClientSocket,&QTcpSocket::readyRead,this,&NetTcpServer::OnReceiveData);
    connect(m_pClientSocket,&QTcpSocket::disconnected,this,&NetTcpServer::OnDisConnnected);


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
void NetTcpServer::setNameAndDevCode(string name,int devCode)
{
    m_name.clear();
    m_name = name;
    m_devCode = devCode;
}

void NetTcpServer::OnReceiveData()
{

    qint64 recvlen=0;
    recvlen=m_pClientSocket->read((char *)tempbuf,MAX_TEMP_BUFFER);
    if(recvlen>0)
    {
        emit RecvNetDataSignal(m_name.c_str(),m_devCode,tempbuf,(unsigned long)recvlen);
    }
//    ProcessNetRecv();

}

void NetTcpServer::OnDisConnnected()
{
    if(m_pClientSocket)
    {
        m_pClientSocket->disconnectFromHost();
        m_pClientSocket->close();
        m_pClientSocket = NULL;

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
}
void NetTcpServer::GetOneFrame()
{
    if(m_pNetRevFunc)
    {
        m_pNetRevFunc((char *)&ReadNetInfo);
    }

}
void NetTcpServer::SendOneFrame(unsigned char  *info, unsigned long sendLen)
{
    m_pClientSocket->write((char *)info,(quint64)sendLen);

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

void NetTcpServer::GetSendOriginal(unsigned char  *info, unsigned long sendLen)
{
    //
    memcpy(sendbuf,info,sendLen);
    emit SendNetDataSignal(getNetName().c_str(),getDevCode(),sendbuf,sendLen);
}
void NetTcpServer::SendNetProcessData(unsigned char  DataArray[], unsigned long nSize)
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

    m_pClientSocket->write((char *)ProcessData,(quint64)infolen);

    unsigned char buft[200];
    memset(buft,0,200);
    QString str;
    qDebug()<<m_name.c_str()<<infolen;
    if (infolen<100)
    {
        memcpy(buft,ProcessData,infolen);
        QString str;
        char aa[10]={0};
        str+=QString("send= ");
        for(unsigned int j=0;j<infolen;j++)
        {
            if (j!=infolen-1)
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
void NetTcpServer::SendNetState()
{
    if(m_pNetStateFunc)
    {
        m_pNetStateFunc((char *)&ConnectNetInfo);
    }
}


