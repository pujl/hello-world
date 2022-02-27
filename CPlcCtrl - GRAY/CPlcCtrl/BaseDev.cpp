// BaseDev.cpp : 实现文件
//

#include "stdafx.h"
#include "BaseDev.h"
#include <math.h>

// CBaseDev

const UINT LINK_COUNTOUT = 5;

CBaseDev::CBaseDev()
{
	
}

CBaseDev::CBaseDev(CString name,CWnd* pWnd)
{
	m_DevName = name;
	m_lpExtData = pWnd;
	m_bDevLink = FALSE;
	m_bCheckState = FALSE;
	m_TimerCount = 0;
	m_LinkFailedCnt = 0;
	m_pFrame = NULL;
	m_bFirstRun = TRUE;
}

CBaseDev::~CBaseDev()
{

}

// CBaseDev 成员函数
void CBaseDev::SetDevLink(BOOL bLink)
{
	if(m_bFirstRun)
	{
		m_bDevLink = bLink;
		m_UpdateIdenty = DEV_LINK_FIRST;
		UpdateDevInfo();
		m_bFirstRun = FALSE;
	}else if(m_bDevLink != bLink)
	{
		m_bDevLink = bLink;
		m_UpdateIdenty = DEV_LINK_MSG;
		UpdateDevInfo();

		ThingByLinkMsg(bLink);
	}
}

void CBaseDev::UpdateDevInfo()
{
	if((m_lpExtData!=NULL)&&AfxIsValidAddress(m_lpExtData,sizeof(CWnd))&&::IsWindow(((CWnd*)m_lpExtData)->GetSafeHwnd()))
	{
		m_lpExtData->PostMessage(UM_DEV_UPDATE_MSG,m_DevCode,m_UpdateIdenty);
	}
}

CString	CBaseDev::GetDevName()
{
	return m_DevName;
}

void CBaseDev::RecvLinkState()
{
	//收到后重置计数器
	m_LinkFailedCnt = 0;
	SetDevLink(TRUE);
}

void CBaseDev::LinkCountsPlus()
{
	if(m_LinkFailedCnt < LINK_COUNTOUT)
	{
		m_LinkFailedCnt ++;
		if(m_LinkFailedCnt >= LINK_COUNTOUT)
		{
			SetDevLink(FALSE);
		}
	}
}