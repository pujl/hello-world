#include "netdataprocessdll.h"
#include <QSettings>
#include <QCoreApplication>
#include <QDebug>
#include <QTextCodec>

#include "nettcpserver.h"
#include "nettcpclient.h"
#include "netudpsocket.h"
NETINFO ReadNetInfo;
NETINFO ConnectNetInfo;
extern vector <NetTcpClient* > TcpClientList;
extern vector <NetTcpServer* > TcpServerList;
extern vector <NetUdpSocket* > UdpSocketList;
NetDataProcessDLL::NetDataProcessDLL()
{
    m_pTimerFunc = NULL;
    connect(&timer,&QTimer::timeout,this,&NetDataProcessDLL::dealTimer);
}
void NetDataProcessDLL::ctrlTimer(int type)
{
    if(0==type)
    {
        if(timer.isActive())
            timer.stop();
        timer.start(100);
    }
    else
    {
         if(timer.isActive())
         {
             timer.stop();
         }
    }
}
void NetDataProcessDLL::dealTimer()
{
    static int num=0;
    num++;
    num %=10;
    m_pTimerFunc(num,m_pUser);

}
void NetDataProcessDLL::SetTimerFunc(pTIMERFUC ptimerfunc,void* pUser )
{
    m_pTimerFunc = ptimerfunc;
    m_pUser = pUser;
}
double addPlus(double a,double b)
{
    return a*2*b;
}
//网络初始化函数
void InitNet()
{
    memset(&ReadNetInfo,0,sizeof(ReadNetInfo));
    memset(&ConnectNetInfo,0,sizeof(ConnectNetInfo));
    vector<NetTcpClient *>::iterator tempclient;
    while(0!=TcpClientList.size())
    {
        tempclient = TcpClientList.begin();
        NetTcpClient *pClient=(NetTcpClient *)(*tempclient);
        pClient->disconnectFromHost();
        pClient->close();
        delete pClient;
        pClient = NULL;
        TcpClientList.erase(tempclient);

    }
    vector<NetTcpServer *>::iterator tempserver;
    while(0!=TcpServerList.size())
    {
        tempserver = TcpServerList.begin();
        NetTcpServer *pServer=(NetTcpServer *)(*tempserver);
        pServer->close();
        delete pServer;
        pServer = NULL;
        TcpServerList.erase(tempserver);

    }
    vector<NetUdpSocket *>::iterator tempudp;
    while(0!=UdpSocketList.size())
    {
        tempudp=UdpSocketList.begin();
        NetUdpSocket *pUdpSocket=(NetUdpSocket *)(*tempudp);
        pUdpSocket->disconnectFromHost();
        pUdpSocket->close();
        delete pUdpSocket;
        pUdpSocket = NULL;
        UdpSocketList.erase(tempudp);
    }
}
void ExitNetDll()
{
    vector<NetTcpClient *>::iterator tempclient;
    while(0!=TcpClientList.size())
    {
        tempclient = TcpClientList.begin();
        NetTcpClient *pClient=(NetTcpClient *)(*tempclient);
        pClient->disconnectFromHost();
        pClient->close();
        delete pClient;
        pClient = NULL;
        TcpClientList.erase(tempclient);

    }
    vector<NetTcpServer *>::iterator tempserver;
    while(0!=TcpServerList.size())
    {
        tempserver = TcpServerList.begin();
        NetTcpServer *pServer=(NetTcpServer *)(*tempserver);
//        pServer->disconnectclient();
        pServer->close();
        delete pServer;
        pServer = NULL;
        TcpServerList.erase(tempserver);

    }
    vector<NetUdpSocket *>::iterator tempudp;
    while(0!=UdpSocketList.size())
    {
        tempudp=UdpSocketList.begin();
        NetUdpSocket *pUdpSocket=(NetUdpSocket *)(*tempudp);
        pUdpSocket->disconnectFromHost();
        pUdpSocket->close();
        delete pUdpSocket;
        pUdpSocket = NULL;
        UdpSocketList.erase(tempudp);
    }

}

int TCPNetServerSetting(string netName,int &DevCode,pNetRevFunc netRevFunc,pNetRevFunc netstateFunc)//设置
{
       QString str=QCoreApplication::applicationDirPath()+"/debug/config.ini";///config.ini
       QSettings m_IniFile(str,QSettings::Format::IniFormat);
       m_IniFile.setIniCodec(QTextCodec::codecForName("UTF-8"));
       //m_IniFile.setIniCodec("UTF-8");
       QString str1=QString::fromUtf8("NetConfig");
   //     QByteArray  ab;
   //     ab.from=QTextCodec::fromUnicode(str1);
   //    QString str1=QTextCodec::convertToUnicode(uiy,10);
       int listenport=0;//监听端口 ipstr;

       QString keystr=QString(netName.c_str());
       keystr+="ListenPort";
       m_IniFile.beginGroup(str1);
   //   if(m_IniFile.childKeys().contains("对上本地IP地址")). frfr
   //    {
   //       m_IniFile.setValue("ip","192.168.1.36");
   //    }
   //    else
   //    {
           listenport=m_IniFile.value(keystr).toInt();
           QString LocalIP=m_IniFile.value("LocalIP").toString();
   //    }

        m_IniFile.endGroup();
        NetTcpServer *pTcpServer=NULL;
        pTcpServer=new NetTcpServer;
        str1.clear();
        str1=QString::fromUtf8("DevConfig");
        keystr.clear();
        keystr=QString(netName.c_str());
        m_IniFile.beginGroup(str1);
        DevCode = m_IniFile.value(keystr).toInt();
        m_IniFile.endGroup();

        pTcpServer->setNameAndDevCode(netName,DevCode);
        if(netRevFunc)
        {
            pTcpServer->setNetRevFunc(netRevFunc);
        }
        if(netstateFunc)
        {
            pTcpServer->setNetStateFunc(netstateFunc);
        }
        if(pTcpServer->Listen(listenport,LocalIP))
        {
            TcpServerList.push_back(pTcpServer);
            return 0;
        }



//        qDebug()<<listenport;

        return 1;
}
int SendNetData(unsigned char devcode,unsigned char *buf,unsigned int len)
{
    //string name=qname.toStdString();
    bool bfind=false;
    vector<NetTcpServer *>::iterator tempserver;
    for(tempserver=TcpServerList.begin();tempserver!=TcpServerList.end();tempserver++ )
    {
        if(NetTcpServer *pServer=qobject_cast<NetTcpServer *>(*tempserver))
        {
            if(pServer->getDevCode() == devcode)
            {
                pServer->GetSendOriginal(buf,len);
                //                pServer->SendNetProcessData(buf,len);
                bfind=true;
                break;
            }
        }

    }
    if(true==bfind)
    {
        return 0;
    }
    vector<NetTcpClient *>::iterator tempclient;
    for(tempclient=TcpClientList.begin();tempclient!=TcpClientList.end();tempclient++)
    {
        if(NetTcpClient *pClient=qobject_cast<NetTcpClient *>(*tempclient))
        {
            if(pClient->getDevCode() == devcode)
            {
                pClient->GetSendOriginal(buf,len);
                bfind=true;
                break;
            }
        }
    }
    if(true==bfind)
    {
        return 0;
    }
    vector<NetUdpSocket *>::iterator tempudp;
    for(tempudp=UdpSocketList.begin();tempudp!=UdpSocketList.end();tempudp++)
    {
        if(NetUdpSocket *pUdpSocket=qobject_cast<NetUdpSocket *>(*tempudp))
        {
            if(pUdpSocket->getDevCode() == devcode)
            {
                pUdpSocket->GetSendOriginal(buf,len);
                bfind=true;
                break;
            }
        }
    }
    return 0;
}
int TCPNetClientSetting(string netName,int &DevCode,pNetRevFunc netRevFunc,pNetRevFunc netstateFunc)//设置
{
    vector<NetTcpClient *>::iterator tempclient;
    for(tempclient=TcpClientList.begin();tempclient!=TcpClientList.end();tempclient++)
    {
        if(NetTcpClient *pClient=qobject_cast<NetTcpClient *>(*tempclient))
        {
            if(0==pClient->getNetName().compare(netName.c_str()))
            {
                pClient->disconnectFromHost();
                pClient->close();
                delete pClient;
                pClient = NULL;
                TcpClientList.erase(tempclient);
                break;
            }
        }
    }
    QString str=QCoreApplication::applicationDirPath()+"/config.ini";//config.ini
    QSettings m_IniFile(str,QSettings::Format::IniFormat);
    m_IniFile.setIniCodec(QTextCodec::codecForName("UTF-8"));
    QString str1=QString::fromUtf8("NetConfig");
    int peerport=0;//对方端口 ipstr;
    QString keystr;
    keystr=QString(netName.c_str());
    keystr+="RemoteIP";
    m_IniFile.beginGroup(str1);

    QString RemoteIP;
    RemoteIP=m_IniFile.value(keystr).toString();
    keystr.clear();
    keystr=QString(netName.c_str());
    keystr+="PeerPort";
    peerport=m_IniFile.value(keystr).toInt();
    m_IniFile.endGroup();

    NetTcpClient *pCurrentSocket=NULL;
    pCurrentSocket=new NetTcpClient;
    if(NULL==pCurrentSocket)
    {
        return 3;
    }
    str1.clear();
    str1=QString::fromUtf8("DevConfig");
    keystr.clear();
    keystr=QString(netName.c_str());
    m_IniFile.beginGroup(str1);
    DevCode = m_IniFile.value(keystr).toInt();
    m_IniFile.endGroup();

    pCurrentSocket->setNameAndDevCode(netName,DevCode);
    if(netRevFunc)
    {
        pCurrentSocket->setNetRevFunc(netRevFunc);
    }
    if(netstateFunc)
    {
        pCurrentSocket->setNetStateFunc(netstateFunc);
    }
    pCurrentSocket->connectToHost(QHostAddress(RemoteIP),peerport);
    TcpClientList.push_back(pCurrentSocket);


     return 0;
}

int UDPNetSetting(string netName,int &DevCode,pNetRevFunc netRevFunc)
{
    vector<NetUdpSocket *>::iterator tempudp;
    for(tempudp=UdpSocketList.begin();tempudp!=UdpSocketList.end();tempudp++)
    {
        if(NetUdpSocket *pUdp=qobject_cast<NetUdpSocket *>(*tempudp))
        {
            if(0==pUdp->getNetName().compare(netName.c_str()))
            {
                pUdp->disconnectFromHost();
                pUdp->close();
                delete pUdp;
                pUdp = NULL;
                UdpSocketList.erase(tempudp);
                break;
            }
        }
    }
    QString str=QCoreApplication::applicationDirPath()+"/debug/config.ini";///config.ini
    QSettings m_IniFile(str,QSettings::Format::IniFormat);
    m_IniFile.setIniCodec(QTextCodec::codecForName("UTF-8"));
    QString str1=QString::fromUtf8("NetConfig");
    int peerport=0;//对方端口 ipstr;
    QString keystr;
    keystr=QString(netName.c_str());
    keystr+="RemoteIP";
    m_IniFile.beginGroup(str1);

    unsigned long LocalPort=0;//本地端口
    QString RemoteIP;
    RemoteIP=m_IniFile.value(keystr).toString();
    keystr.clear();
    keystr=QString(netName.c_str());
    keystr+="PeerPort";
    peerport=m_IniFile.value(keystr).toInt();

    keystr.clear();
    keystr=QString(netName.c_str());
    keystr+="LocalPort";
    LocalPort=m_IniFile.value(keystr).toInt();

    QString LocalIP=m_IniFile.value("LocalIP").toString();

    m_IniFile.endGroup();

    NetUdpSocket *pUdpSocket=NULL;
    pUdpSocket=new NetUdpSocket;
    if(NULL==pUdpSocket)
    {
        return 3;
    }
    str1.clear();
    str1=QString::fromUtf8("DevConfig");
    keystr.clear();
    keystr=QString(netName.c_str());
    m_IniFile.beginGroup(str1);
    DevCode = m_IniFile.value(keystr).toInt();
    m_IniFile.endGroup();
    string peerst;
    peerst=RemoteIP.toUtf8().data();
    pUdpSocket->setPeerParam(peerst,peerport);
    pUdpSocket->setNameAndDevCode(netName,DevCode);
    pUdpSocket->bind(QHostAddress(LocalIP),LocalPort);
    if(netRevFunc)
    {
        pUdpSocket->setNetRevFunc(netRevFunc);
    }

    UdpSocketList.push_back(pUdpSocket);
    return 0;
}
