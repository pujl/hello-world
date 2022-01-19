#ifndef PLCDEV_H
#define PLCDEV_H

#include <QObject>
#include "basedev.h"

const quint32 PLC_MSGINIT      = DEVID_PLC*1+1;
const quint32 PLC_MSGCHANGE    = DEVID_PLC*1+2;
const quint32 PLC_MSGRECORD    = DEVID_PLC*1+3;
const quint32 PLC_MSGRLOCK     = DEVID_PLC*1+4;
const quint32 PLC_MSGRALLERR   = DEVID_PLC*1+5;
const quint32 PLC_MSGRUN       = DEVID_PLC*1+6;

class plcDev : public baseDev
{
public:
    plcDev();
    plcDev(QObject *parent = 0);

public:
    void    TranslateMsg(quint8* pMsg,quint32 len);
    void	ThingByLinkMsg(DEV_LINK_STATE bLink);
    void	SendQryCmd();
    CAR_STATE       QryCarState(quint8 nId)   {return m_eCarState[nId];}
    lpPLC_RECORD    GetPlcRcd()             {return m_Record;}
    bool	GetLockState()                  {return m_bLock;}
    bool	GetAllErr()                     {return m_bAllErr;}
    bool	GetPLCRun()                     {return m_bPLCRun;}
private:
    //判断设备状态是否发生变化，发生变化则显示
    bool	JudgePLCStateChange(quint8 nId,CAR_STATE eState);
private:
    CAR_STATE	m_eCarState[8];
    quint8		m_PLCNum[10];
    PLC_STATE	m_PLCState;
    bool		m_bInit;
    PLC_RECORD  m_Record[8];
    //综合异常
    bool		m_bAllErr;
    //互锁切
    bool		m_bLock;
    //PLC运行
    bool		m_bPLCRun;
};

#endif // PLCDEV_H
