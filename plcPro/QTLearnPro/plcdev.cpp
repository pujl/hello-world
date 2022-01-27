#include "plcdev.h"
#include "../NetDataProcessDLL/netdataprocessdll.h"

plcDev::plcDev()
{
    for(int i =0;i<8;i++)
    {
        m_eCarState[i] = INIT_STATE;
        m_Record[i].bChange = false;
        m_Record[i].m_eCurState = INIT_STATE;
    }
    m_bInit = true;
    m_bAllErr = false;
    m_bLock = false;
    m_bPLCRun = false;
}

plcDev::plcDev(QObject *parent) : baseDev(parent)
{
    for(int i =0;i<8;i++)
    {
        m_eCarState[i] = INIT_STATE;
        m_Record[i].bChange = false;
        m_Record[i].m_eCurState = INIT_STATE;
    }
    m_bInit = true;
    m_bAllErr = false;
    m_bLock = false;
    m_bPLCRun = false;
}

void plcDev::TranslateMsg(quint8* pMsg,quint32 len)
{
    //赋值
    unsigned short nIdx = 0x01;
    unsigned short nTemp = 0;
    if(len == 6)
    {
        memcpy((quint8*)&m_PLCState,pMsg,len);

        //判断是否计划停止,停止不判断正常故障
        if((m_PLCState._D800 & nIdx) == nIdx)
        {
            //if(!m_bInit)
            {
                for(int i =0;i<8;i++)
                {
                    m_eCarState[i] = INIT_STATE;
                }
                //m_bInit = TRUE;

                //刷新
                updateDevMsg(PLC_MSGINIT,m_DevCode);
            }
        }else
        {
            //判断异常
            bool bReport = false;
            for(quint8 i =0;i<8;i++)
            {
                nTemp = nIdx<<(i+2);
                if((m_PLCState._D800 & nTemp) == nTemp)
                {
                    bool bChange = JudgePLCStateChange(i,ERROR_STATE);

                    if(bChange)
                    {
                        //上报状态
                        bReport = true;
                    }
                }
            }
            for(quint8 i =0;i<8;i++)
            {
                nTemp = nIdx<<(i+2);
                if((m_PLCState._D801 & nTemp) == nTemp)
                {
                    bool bChange = JudgePLCStateChange(i,NORAML_STATE);

                    //上报状态

                    if(bChange)
                    {
                        //上报状态
                        bReport = true;
                    }
                }
            }
            if(bReport)
            {
                updateDevMsg(PLC_MSGCHANGE,m_DevCode);
            }
        }
        //判断互锁切
        nTemp = nIdx<<1;
        if((m_PLCState._D800 & nTemp) == nTemp)
        {
            if(!m_bLock)
            {
                m_bLock = true;
                updateDevMsg(PLC_MSGRLOCK,m_DevCode);

            }
        }else
        {
            if(m_bLock)
            {
                m_bLock = false;
                updateDevMsg(PLC_MSGRLOCK,m_DevCode);
            }
        }
        //判断综合异常？一般只有有设备异常，就会异常，可以做一个指示灯
        nTemp = nIdx<<0x0A;
        if((m_PLCState._D800 & nTemp) == nTemp)
        {
            if(!m_bAllErr)
            {
                m_bAllErr = true;
                updateDevMsg(PLC_MSGRALLERR,m_DevCode);
            }
        }else
        {
            if(m_bAllErr)
            {
                m_bAllErr = false;
                updateDevMsg(PLC_MSGRALLERR,m_DevCode);
            }
        }
        //判断PLC运行
        nTemp = nIdx<<0x0B;
        if((m_PLCState._D800 & nTemp) == nTemp)
        {
            if(!m_bPLCRun)
            {
                m_bPLCRun = true;
                updateDevMsg(PLC_MSGRUN,m_DevCode);
            }
        }else
        {
            if(m_bPLCRun)
            {
                m_bPLCRun = false;
                updateDevMsg(PLC_MSGRUN,m_DevCode);
            }
        }
    }
}

void plcDev::ThingByLinkMsg(DEV_LINK_STATE eLink)
{
    //设备断开时，所有界面均恢复初始状态？？？检查网络等等？？？
    if(eLink == LINK_FAILED)
    {
        for(int i =0;i<8;i++)
        {
            m_eCarState[i] = INIT_STATE;
        }
        //m_bInit = FALSE;

        //刷新
        updateDevMsg(PLC_MSGINIT,m_DevCode);

    }
}

bool plcDev::JudgePLCStateChange(quint8 nId,CAR_STATE eState)
{
    if(m_eCarState[nId] != eState)
    {
        //状态切换
        m_eCarState[nId] = eState;
        m_Record[nId].m_eCurState = eState;

        if(eState == ERROR_STATE)
        {
            m_Record[nId].bChange = true;

            m_Record[nId].m_ErrTime = QDateTime::currentDateTime();
        }else
        {
            m_Record[nId].bChange = true;

           m_Record[nId].m_RecoverTime = QDateTime::currentDateTime();
        }
        return true;
    }else
    {
        m_Record[nId].bChange = false;
        return false;
    }
}

void plcDev::SendQryCmd()
{
    if(GetDevLink() == LINK_SUCC)
    {
        unsigned char buf[11];
        buf[0] = 0x00;
        buf[1] = 0x00;
        buf[2] = 0x07;
        buf[3] = 0x00;
        buf[4] = 0x22;
        buf[5] = 0x00;
        buf[6] = 0x18;
        buf[7] = 0x01;
        buf[8] = 0x18;
        buf[9] = 0x02;
        buf[10] = 0x18;
        SendNetData(0,buf,11);
    }
}
