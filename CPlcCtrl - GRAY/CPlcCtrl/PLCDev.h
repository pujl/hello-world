#pragma once
#include "basedev.h"

const UINT PLC_MSGINIT = DEVID_PLC*1+1;
const UINT PLC_MSGCHANGE = DEVID_PLC*1+2;
const UINT PLC_MSGRECORD = DEVID_PLC*1+3;
const UINT PLC_MSGRLOCK  = DEVID_PLC*1+4;
const UINT PLC_MSGRALLERR = DEVID_PLC*1+5;
const UINT PLC_MSGRUN	= DEVID_PLC*1+6;
/*
typedef struct BITMODULE{
	unsigned short _BIT0	:1;
	unsigned short _BIT1	:1;
	unsigned short _BIT2	:1;
	unsigned short _BIT3	:1;
	unsigned short _BIT4	:1;
	unsigned short _BIT5	:1;
	unsigned short _BIT6	:1;
	unsigned short _BIT7	:1;

	unsigned short _BIT8	:1;
	unsigned short _BIT9	:1;
	unsigned short _BITA	:1;
	unsigned short _BITB	:1;
	unsigned short _BITC	:1;
	unsigned short _BITD	:1;
	unsigned short _BITE	:1;
	unsigned short _BITF	:1;
}BITMODULE;


typedef struct PLC_STATE{
	BITMODULE	_D800;
	BITMODULE	_D801;
	BITMODULE	_D802;
}PLC_STATE;
*/

class CPLCDev : public CBaseDev
{
public:
	CPLCDev(CString name,CWnd* pWnd);
	~CPLCDev(void);

public:
	void	TranslateMsg(unsigned char* pMsg,unsigned int len);
	void	ThingByLinkMsg(bool bLink);
	void	SendQryCmd();
	CAR_STATE	QryCarState(BYTE nId)  {return m_eCarState[nId];}
	lpPLC_RECORD   GetPlcRcd()		{return m_Record;}
	BOOL	GetLockState()			{return m_bLock;}
	BOOL	GetAllErr()				{return m_bAllErr;}
	BOOL	GetPLCRun()				{return m_bPLCRun;}
private:
	//判断设备状态是否发生变化，发生变化则显示
	BOOL	JudgePLCStateChange(BYTE nId,CAR_STATE eState);
private:
	CAR_STATE	m_eCarState[8];
	UCHAR		m_PLCNum[10];
	PLC_STATE	m_PLCState;
	BOOL		m_bInit;
	PLC_RECORD  m_Record[8];
	//综合异常
	BOOL		m_bAllErr;
	//互锁切
	BOOL		m_bLock;
	//PLC运行
	BOOL		m_bPLCRun;
};