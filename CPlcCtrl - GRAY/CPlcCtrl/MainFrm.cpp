// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "CPlcCtrl.h"

#include "MainFrm.h"
#include "PLCDev.h"
#include "CMainView.h"
#include "MakeFileContent.h"
#include <MMSystem.h>

#pragma comment(lib, "winmm.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_GETMINMAXINFO()
	ON_MESSAGE(NET_DATA_ARRIVE,OnTCPInfoRecv)
	ON_MESSAGE(NET_CONNECT_OK,OnConnectState)
	ON_MESSAGE(UM_DEV_UPDATE_MSG,OnDevUpdateMsg)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CMainFrame 构造/析构
#define  _RCD_DATA_FILE

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
	m_BackBrush.CreateSolidBrush(BACKCOLOR);
	m_pLCDev = NULL;
	//获取屏幕分辨率
	m_WndWidth = GetSystemMetrics(SM_CXSCREEN); 
	m_WndHigh = GetSystemMetrics(SM_CYSCREEN);
	m_bFinishedConn = TRUE;
	m_ConnectDevId = 0;
	m_ConnectDevNum = 0;

	CalcBinAddress();

#ifdef _RCD_DATA_FILE
	//初始化Tlog文件
	CString strTitle = CMakeFileContent::GetPLCInfoTitle();
	m_PLCLog.InitLogFile("设备日志",strTitle,m_strBinAddress);
	strTitle = CMakeFileContent::GetPLCInfoFormat();
#endif

	m_strAlarmMusic.Format("%s\\alarm.wav",m_strBinAddress);
}

CMainFrame::~CMainFrame()
{
	delete m_pLCDev;
}


BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	cs.cx = m_WndWidth;
	cs.cy = m_WndHigh;
	cs.style = WS_OVERLAPPED | FWS_ADDTOTITLE;
	cs.hMenu = NULL;

	return TRUE;
}


// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame 消息处理程序

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	SetWindowLong(m_hWnd,GWL_STYLE,WS_OVERLAPPED);
	SetWindowLong(m_hWnd,GWL_EXSTYLE,WS_EX_LTRREADING);
	SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED);

	m_PromptNotifier.Create(this);
	m_PromptNotifier.SetSkin(IDB_BMP_NOTIFIER);
	m_PromptNotifier.SetTextFont("Arial",110,TN_TEXT_NORMAL,TN_TEXT_UNDERLINE);
	m_PromptNotifier.SetTextColor(RGB(0,0,0),RGB(0,0,200));
	m_PromptNotifier.SetTextRect(CRect(10,35,m_PromptNotifier.m_nSkinWidth-10,m_PromptNotifier.m_nSkinHeight-5));	

	InitDevComm();

	SetTimer(2,6000,NULL);
	SetTimer(3,100,NULL);
	CoConnectLink();
	return 0;
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	BYTE iret=0;
	BYTE DevCode=0;
	switch(nIDEvent)
	{
	case 1:
		{
			if (!m_bFinishedConn)
			{
				break;
			}
			if(m_ConnectDevNum == DEVNUM_MAX)
			{
				KillTimer(nIDEvent);
				break;
			}
			switch(m_ConnectDevNum)
			{
			case 0://PLC
				{
					if (m_pLCDev->GetDevLink())
					{
						m_bFinishedConn=TRUE;
						m_ConnectDevNum++;
						break;
					}
					m_bFinishedConn=FALSE;
					m_ConnectDevId = DEVID_PLC;
					iret = TCPNetClientSetting(m_pLCDev->GetDevName(),DevCode);
					if(iret == 1)
					{
						//初始化失败
						SetPromptNotifier("PLC网络初始化失败!");
					}
				}
				break;
			default:
				break;
			}
		}
		break;
	case 2:
		{
			CoConnectLink();
		}
		break;
	case 3:
		{
			m_pLCDev->SendQryCmd();
		}
		break;
	default:
		break;
	}
	CFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::CoConnectLink()
{
	if(m_ConnectDevNum != DEVNUM_MAX)
	{
		return;
	}
	m_ConnectDevNum = 0;
	m_bFinishedConn = TRUE;
	m_ConnectDevId = DEVID_PLC;

	SetTimer(1,60,NULL);
}

CBrush&	CMainFrame::GetBackBrush()
{
	return m_BackBrush;
}

void CMainFrame::SetPromptNotifier(CString msg)
{
	if(m_PromptNotifier.m_hWnd)
		m_PromptNotifier.Show(msg,500,3000);
}
void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	lpMMI->ptMaxTrackSize.x = m_WndWidth;
	lpMMI->ptMaxTrackSize.y = m_WndHigh;
	lpMMI->ptMaxTrackSize.x = m_WndWidth;
	lpMMI->ptMaxTrackSize.y = m_WndHigh;
	CFrameWnd::OnGetMinMaxInfo(lpMMI);
}

void CMainFrame::InitDevComm()
{
	m_ConnectDevNum = 1;
	INITTCPNET();

	m_pLCDev = new CPLCDev(DEVNAME_PLC,this);
	m_pLCDev->SetDevCode(DEVID_PLC);
	m_pLCDev->SetFramePoint(this);
}

void CMainFrame::ConnectPLC()
{
	BYTE iret=0;
	BYTE DevCode=0;
	if(m_pLCDev->GetDevLink())
	{
		//已连接
		SetPromptNotifier("PLC设备已连接，请勿重复连接！");
	}else
	{
		iret = TCPNetClientSetting(m_pLCDev->GetDevName(),DevCode);
		if(iret == 1)
		{
			//初始化失败
			SetPromptNotifier("PLC网络初始化失败!");
		}
	}
}

LRESULT CMainFrame::OnTCPInfoRecv(WPARAM wParam, LPARAM lParam)
{
	ProcessDevMsg(wParam,lParam);
	return 0;
}

//网络状态
LRESULT CMainFrame::OnConnectState(WPARAM wParam, LPARAM lParam)
{
	NETINFO *pNetData;
	pNetData=(NETINFO *)wParam;
	while(pNetData->ReadPoint!=pNetData->WritePoint)
	{
		unsigned long infolen=0;
		unsigned char lenbuf[4];
		memset(lenbuf,0,4);
		unsigned char *pData=NULL;
		unsigned char port=0;
		unsigned long tempread=0;
		unsigned long i=0;
		tempread=pNetData->ReadPoint;
		for (i=0;i<4;i++)
		{
			lenbuf[i]=pNetData->pdata[tempread];
			tempread=(tempread+1)%MAX_SENDNET_BUFFER;
		}
		memcpy(&infolen,lenbuf,4);

		port=pNetData->pdata[tempread];
		tempread=(tempread+1)%MAX_SENDNET_BUFFER;

		pData=new unsigned char [infolen];
		memset(pData,0,infolen);
		for (i=0;i<infolen;i++)
		{
			pData[i]=pNetData->pdata[tempread];
			tempread=(tempread+1)%MAX_SENDNET_BUFFER;
		}
		int type,m_Connect;
		memcpy(&type,pData,sizeof(type));
		memcpy(&m_Connect,&pData[sizeof(type)],sizeof(m_Connect));
		ProcessNetConnectState(type,m_Connect);
		pNetData->ReadPoint=tempread;
		if(pData)
		{
			delete []pData;
			pData = NULL;
		}
	}
	return 0;
}

void CMainFrame::ProcessDevMsg(WPARAM wParam,LPARAM lParam)
{
	NETINFO *pNetData;
	pNetData=(NETINFO *)wParam;
	while(pNetData->ReadPoint!=pNetData->WritePoint)
	{
		unsigned long infolen=0;
		unsigned char lenbuf[4];
		memset(lenbuf,0,4);
		unsigned char *pData=NULL;
		unsigned char port=0;
		unsigned long tempread=0;
		unsigned long i=0;
		tempread=pNetData->ReadPoint;
		port = pNetData->pdata[tempread];
		tempread = (tempread + 1)%MAX_SENDNET_BUFFER;

		for (i=0;i<4;i++)
		{
			lenbuf[i]=pNetData->pdata[tempread];
			tempread=(tempread+1)%MAX_SENDNET_BUFFER;
		}
		memcpy(&infolen,lenbuf,4);

		pData=new unsigned char [infolen];
		memset(pData,0,infolen);

		for (int i=0;i<infolen;i++)
		{
			pData[i]=pNetData->pdata[tempread];
			tempread=(tempread+1)%MAX_SENDNET_BUFFER;
		}
		ProcessDeviceInfo(pData,infolen,port);
		pNetData->ReadPoint=tempread;
		if(pData)
		{
			delete []pData;
			pData = NULL;
		}
	}
}

void CMainFrame::ProcessNetConnectState(BYTE DevCode, BYTE bConnect)
{
	//bConnect = 0 断开连接消息  bConnect = 1 连接消息
	switch(DevCode)
	{
	case DEVID_PLC:
		{
			if(1 == bConnect)
			{
				m_pLCDev->SetDevLink(TRUE);
			}else
			{
				m_pLCDev->SetDevLink(FALSE);				
			}
		}
		break;
	default:
		break;
	}
	if(m_ConnectDevId == DevCode)
	{
		if (!m_bFinishedConn)
		{
			m_ConnectDevNum++;
			m_bFinishedConn=TRUE;
		}
	}
}

void CMainFrame::ProcessDeviceInfo(BYTE *info, unsigned long len, unsigned char port)
{
	switch(port)
	{
	case DEVID_PLC:
		m_pLCDev->TranslateMsg(info,len);
		break;
	default:
		break;
	}
}

LRESULT CMainFrame::OnDevUpdateMsg(WPARAM wParam,LPARAM lParam)
{
	//可以在这截获设备消息
	CMainView* pView = (CMainView*)GetActiveView();
	if((pView!=NULL)&&pView->IsKindOf(RUNTIME_CLASS(CMainView)))
	{
		//发送到显示类处理显示事宜
		pView->DevUpdate(wParam,lParam);
	}

	return 0;
}

UINT32 CMainFrame::GetSwMetrics(int index)
{
	switch(index)
	{
	case SW_CXVALUE:
		return m_WndWidth;
	case SW_CYVALUE:
		return m_WndHigh;
	default:
		return 0;
	}
}

CString	CMainFrame::GetProcessBinAddress()
{
	return m_strBinAddress;
}

void CMainFrame::CalcBinAddress()
{
	CString fullpath;
	fullpath=_T("");
	char path[256];
	GetModuleFileName(NULL,path,256);
	fullpath=(CString)path;
	m_strBinAddress =fullpath.Left(fullpath.ReverseFind('\\')+1);
}

void CMainFrame::PlayAlarmVoice()
{
	::PlaySound(m_strAlarmMusic, NULL, SND_ASYNC | SND_FILENAME | SND_NOSTOP | SND_NOWAIT);
}