#include "netrecvprocessthread.h"
#include "netdataprocessdll.h"
#include "nettcpserver.h"
#include "nettcpclient.h"
#include <QMutex>

QMutex RevMutex;
extern NETINFO ReadNetInfo;

const unsigned char START_CHAR1 = 0x80;
const unsigned char START_CHAR2 = 0x00;
const unsigned char START_CHAR3 = 0x07;
const unsigned char START_CHAR4 = 0x00;
const unsigned char START_CHAR5 = 0x22;

NetRecvProcessThread::NetRecvProcessThread(QObject *parent) : QObject(parent)
{
    memset(Read_Buf,0,MAX_TEMP_BUFFER);
    memset(Temp_Buf,0,MAX_TEMP_BUFFER);
    Read_Point=0;
    Write_Point=0;
    Frame_Lenth=0;
    Find_Start=false;
    m_bTranschar=0;

}
void NetRecvProcessThread::ProcessRevThread(const char* name,int devCode,unsigned char *info,unsigned long len,unsigned char type)
{
    RevMutex.lock();
    if(devCode == 0)
    {
        ProcessNetRecv_NONE(devCode,info,len,type);
    }else
    {
        ProcessNetRecv_7E(devCode,info,len,type);
    }
    RevMutex.unlock();
    name=name;
}

void NetRecvProcessThread::ProcessNetRecv_7E(int devCode,unsigned char *info,unsigned long recvlen,unsigned char type) //
{
    unsigned long Curlen=(Write_Point+MAX_TEMP_BUFFER-Read_Point)%MAX_TEMP_BUFFER;//获得当前读写长度差
    if ((MAX_TEMP_BUFFER-Curlen)<=(unsigned long )recvlen)//清除缓冲区的内容
    {
        return ;
    }
    int i=0;
    for (i=0;i<(int)recvlen;i++)
    {
        Temp_Buf[Write_Point]=info[i];
        Write_Point=(Write_Point+1)%MAX_TEMP_BUFFER;
    }
    //处理7e e7 ee 异或校验
    while(Read_Point!=Write_Point)
    {
        if(false==Find_Start&&START_CHAR==Temp_Buf[Read_Point]&&
            TRANSPARENT_CHAR!=Temp_Buf[(Read_Point+MAX_TEMP_BUFFER-1)%MAX_TEMP_BUFFER]&&
            0==m_bTranschar)//首先查找信息头
        {
            Read_Point=(Read_Point+1)%MAX_TEMP_BUFFER;
            Find_Start=true;//找到信息头
            Frame_Lenth=0;
        }
        else if(Find_Start)//有信息头后再找信息尾
        {
            if (START_CHAR==Temp_Buf[Read_Point]&&0==m_bTranschar)//在信息内容中包含信息头
            {
                Read_Point=(Read_Point+1)%MAX_TEMP_BUFFER;
                Find_Start=true;//找到信息头
                Frame_Lenth=0;
            }
            else if(TRANSPARENT_CHAR==Temp_Buf[Read_Point]&&0==m_bTranschar)//此处为透明符
            {
                m_bTranschar=1;
                Read_Point=(Read_Point+1)%MAX_TEMP_BUFFER;

            }
            else if (1==m_bTranschar)//此处为透明符后面的数据
            {
                Read_Buf[Frame_Lenth]=Temp_Buf[Read_Point];
                Frame_Lenth++;
                Read_Point=(Read_Point+1)%MAX_TEMP_BUFFER;
                m_bTranschar=0;
            }

            else if(END_CHAR==Temp_Buf[Read_Point]&&0==m_bTranschar)//此处为结束符
            {
                Read_Point=(Read_Point+1)%MAX_TEMP_BUFFER;
                if (!(Frame_Lenth<2))//帧长超过一定范围后（Frame_Lenth>=2）判别校验位是否正确
                {
                    unsigned char Validate=Read_Buf[0];
                    for (unsigned long j=1;j<Frame_Lenth-1;j++)
                    {
                        Validate=Validate^Read_Buf[j];
                    }
                    unsigned char sourcevalidate=Read_Buf[Frame_Lenth-1];
                    if (sourcevalidate == Validate)//校验正确
                    {
                        //处理满足条件的完整单帧信息
                        ExpandBuffer(Read_Buf,Frame_Lenth-1,(unsigned char)devCode);
                        //****************

                    }
                    Frame_Lenth=0;
                    Find_Start=false;

                }
                else
                {
                    Frame_Lenth=0;
                    Find_Start=false;
                }

            }
            else //非信息头，信息尾，和透明符的信息
            {
                Read_Buf[(Frame_Lenth++)%MAX_TEMP_BUFFER]=Temp_Buf[Read_Point];
                Read_Point=(Read_Point+1)%MAX_TEMP_BUFFER;
            }
        }
        else //没有信息头则临时读指针增加，对下一个字节进行判断
        {
            Read_Point=(Read_Point+1)%MAX_TEMP_BUFFER;
        }
    }
}

void NetRecvProcessThread::ProcessNetRecv_NONE(int devCode,unsigned char *info,unsigned long recvlen,unsigned char type)
{
    unsigned long Curlen=(Write_Point+MAX_TEMP_BUFFER-Read_Point)%MAX_TEMP_BUFFER;//获得当前读写长度差
    if ((MAX_TEMP_BUFFER-Curlen)<=(unsigned long )recvlen)//清除缓冲区的内容
    {
        return ;
    }
    int i=0;
    for (i=0;i<(int)recvlen;i++)
    {
        Temp_Buf[Write_Point]=info[i];
        Write_Point=(Write_Point+1)%MAX_TEMP_BUFFER;
    }

    unsigned int nlen = 0;
    nlen = (Write_Point - Read_Point+MAX_TEMP_BUFFER)%MAX_TEMP_BUFFER;
    while(nlen>=11)
    {
        if((Temp_Buf[Read_Point] == START_CHAR1)&&
            (Temp_Buf[(Read_Point+1)%MAX_TEMP_BUFFER] == START_CHAR2)&&
            (Temp_Buf[(Read_Point+2)%MAX_TEMP_BUFFER] == START_CHAR3)&&
            (Temp_Buf[(Read_Point+3)%MAX_TEMP_BUFFER] == START_CHAR4)&&
            (Temp_Buf[(Read_Point+4)%MAX_TEMP_BUFFER] == START_CHAR5))
        {
            //find head
            for(unsigned int i = 0;i<6;i++)
            {
                Read_Buf[i] = Temp_Buf[(i+Read_Point+5)%MAX_TEMP_BUFFER];
            }

            ExpandBuffer(Read_Buf,6,(unsigned char)devCode);
            Read_Point = (Read_Point+11)%MAX_TEMP_BUFFER;
            nlen = (Write_Point - Read_Point + MAX_TEMP_BUFFER)%MAX_TEMP_BUFFER;
        }else
        {
            Read_Point = (Read_Point + 1)%MAX_TEMP_BUFFER;
            nlen = (Write_Point - Read_Point + MAX_TEMP_BUFFER)%MAX_TEMP_BUFFER;
        }
    }
}

void NetRecvProcessThread::ExpandBuffer(unsigned char *buffer, unsigned long len, unsigned char port)
{
    unsigned long Curlen=(ReadNetInfo.WritePoint+MAX_SENDNET_BUFFER-ReadNetInfo.ReadPoint)%MAX_SENDNET_BUFFER;
    if ((MAX_SENDNET_BUFFER-Curlen)<=(len+5))
    {
        return;
    }
    unsigned char *tempbuf=NULL;
    tempbuf=new unsigned char [len+5];
    if(NULL==tempbuf)
    {
        return ;
    }
    memset(tempbuf,0,len+5);
    memcpy(tempbuf,&len,sizeof(unsigned long));
    memcpy(&tempbuf[4],&port,1);
    memcpy(&tempbuf[5],buffer,len);
    unsigned long tempwrite=ReadNetInfo.WritePoint;
    for (unsigned long i=0;i<len+5;i++)
    {
        ReadNetInfo.pdata[tempwrite]=tempbuf[i];
        tempwrite=(tempwrite+1)%MAX_SENDNET_BUFFER;
    }
    ReadNetInfo.WritePoint=tempwrite;
    if(tempbuf)
    {
        delete [] tempbuf;
        tempbuf=NULL;
    }
    emit ProcessRecvOneFrame();
//    if(m_pNetRevFunc)
//    {
//        m_pNetRevFunc((char *)(&ReadNetInfo));
//    }



//    unsigned char buft[200];
//    memset(buft,0,200);
//    QString str;
//    qDebug()<<m_name.c_str()<<len;
//    if (len<100)
//    {
//        memcpy(buft,buffer,len);
//        QString str;
//        char aa[10]={0};
//        str+=QString("rev= ");
//        for(unsigned int j=0;j<len;j++)
//        {
//            if (j!=len-1)
//            {
//                sprintf(aa,"%02X ",buft[j]);

//            }
//            else
//                sprintf(aa,"%02X \r\n",buft[j]);
//            str+=QString(aa);
//        }
//        qDebug()<<str.toUtf8().data();
//    }


}
