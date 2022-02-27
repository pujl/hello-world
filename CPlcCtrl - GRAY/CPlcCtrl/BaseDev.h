#pragma once
#include "EnumStruct.h"
// CBaseDev 命令目标
const BYTE DEV_LINK_MSG = 0x00;
const BYTE DEV_LINK_FIRST = 0xFE;

class CMainFrame;

class CBaseDev : public CObject
{
public:
	CBaseDev();
	CBaseDev(CString name,CWnd* pWnd);
	virtual ~CBaseDev();
public:
	//设备名称
	inline void SetDevName(CString DevName)	{m_DevName = DevName;}
	//设备编号
	inline void SetDevCode(UCHAR DevCode)	{m_DevCode = DevCode;}
	//当前通信情况,
	//对于TCP设备，通过socket连接情况判断是否连接，默认未连接，通信连接，断开连接
	//对于UPD设备，通过链路命令来判断是否连接，默认未连接，链路收到为连接，3包收不到为断开连接
	void		SetDevLink(BOOL bLink);
	//设置刷新指针
	void		SetExData(CWnd* pwnd)	{m_lpExtData = pwnd;}
	//返回设备名称
	CString		GetDevName();
	//返回设备编号
	inline UINT	GetDevCode()		{return m_DevCode;}
	//返回link
	inline BOOL GetDevLink()		{return m_bDevLink;}
	//链路计数器+
	void		LinkCountsPlus();
	//设置MainFrame类指针
	void		SetFramePoint(CMainFrame* pFrame)		{m_pFrame = pFrame;}
protected:
	virtual void  TranslateMsg(unsigned char* pMsg,unsigned int len) = 0;
	//程序断开需要做的事
	virtual void  ThingByLinkMsg(bool bLink) = 0;
protected:
	void		UpdateDevInfo();
	//接收链路信息
	void		RecvLinkState();
protected:
	CString		m_DevName;
	CWnd*		m_lpExtData;
	UCHAR		m_DevCode;
	UINT		m_UpdateIdenty;	//数据刷新标识
	UINT		m_TimerCount;
	BOOL		m_bCheckState;
	CMainFrame	*m_pFrame;
	//链路失败次数
	UINT		m_LinkFailedCnt;
private:
	BOOL		m_bDevLink;
	BOOL		m_bFirstRun;
	
};


