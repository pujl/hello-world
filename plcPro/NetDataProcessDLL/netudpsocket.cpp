#include "netudpsocket.h"
#include <QHostAddress>
extern NETINFO ReadNetInfo;
vector <NetUdpSocket* > UdpSocketList;
NetUdpSocket::NetUdpSocket(QObject *parent) : QUdpSocket(parent)
{
    m_pNetRevFunc = NULL;

    connect(this,&NetUdpSocket::readyRead,this,&NetUdpSocket::OnReceiveData);

    memset(sendbuf,0,sizeof (sendbuf));
    memset(tempbuf,0,sizeof (tempbuf));

    m_devCode = 0;

    m_PeerPort = 0;

    m_pRevthread = NULL;
    m_pNetRevThread = NULL;
    m_pNetRevThread = new NetRecvProcessThread;
    m_pRevthread = new QThread(this);
    m_pNetRevThread->moveToThread(m_pRevthread);

//    connect(m_pRevthread,&QThread::finished,m_pNetRevThread,&NetRecvProcessThread::deleteLater);//删除m_pNetRevThread会出现问题
    connect(this,&NetUdpSocket::RecvNetDataSignal,m_pNetRevThread,&NetRecvProcessThread::ProcessRevThread);
    connect(m_pNetRevThread,&NetRecvProcessThread::ProcessRecvOneFrame,this,&NetUdpSocket::GetOneFrame);
    m_pRevthread->start();

    m_pSendthread = NULL;
    m_pNetSendThread = NULL;
    m_pNetSendThread = new NetSendProcessThread;
    m_pSendthread = new QThread(this);
    m_pNetSendThread->moveToThread(m_pSendthread);
    connect(this,&NetUdpSocket::SendNetDataSignal,m_pNetSendThread,&NetSendProcessThread::SendNetdataTread,Qt::DirectConnection);
    connect(m_pNetSendThread,&NetSendProcessThread::ProcessSendOneFrame,this,&NetUdpSocket::SendOneFrame);
    m_pSendthread->start();

}

NetUdpSocket::~NetUdpSocket()
{

    this->close();
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
void NetUdpSocket::setNetRevFunc(pNetRevFunc pRevProcessfunc)
{
    m_pNetRevFunc = pRevProcessfunc;
}

void NetUdpSocket::setNameAndDevCode(string name,int devCode)
{
    m_name.clear();
    m_name = name;
    m_devCode = devCode;
}

void NetUdpSocket::OnReceiveData()
{
    QHostAddress cliAddress;
    quint16 port;
     qint64 recvlen=readDatagram((char *)tempbuf,MAX_TEMP_BUFFER,&cliAddress,&port);
    if(recvlen>0)
    {
        emit RecvNetDataSignal(m_name.c_str(),m_devCode,tempbuf,(unsigned long)recvlen);
    }
//    ProcessNetRecv();

}

void NetUdpSocket::GetOneFrame()
{
    if(m_pNetRevFunc)
    {
        m_pNetRevFunc((char *)&ReadNetInfo);
    }

}
void NetUdpSocket::SendOneFrame(unsigned char  *info, unsigned long sendLen)
{
    writeDatagram((char *)info,(quint64)sendLen,QHostAddress(QString(m_PeerIP.c_str())),(quint64)m_PeerPort);

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

void NetUdpSocket::GetSendOriginal(unsigned char  *info, unsigned long sendLen)
{
    //
    memcpy(sendbuf,info,sendLen);
    emit SendNetDataSignal(getNetName().c_str(),getDevCode(),sendbuf,sendLen);
}

void NetUdpSocket::setPeerParam(string peername,unsigned long peerport)
{

    m_PeerIP = peername;
    m_PeerPort = peerport;
}


