// MainFrm.h : CMainFrame 类的接口
//


#pragma once
#include "TaskbarNotifier.h"
#include "TLog.h"
class CPLCDev;

#define  SW_CXVALUE 0
#define  SW_CYVALUE 1
const UINT DEVNUM_MAX = 1;

class CMainFrame : public CFrameWnd
{
	
protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 属性
public:

// 操作
public:

// 重写
public:
	virtual BOOL	PreCreateWindow(CREATESTRUCT& cs);
	CBrush&			GetBackBrush();
	void			SetPromptNotifier(CString msg);
	void			ConnectPLC();
	UINT32			GetSwMetrics(int index);
	CString			GetProcessBinAddress();
	void			PlayAlarmVoice();
private:
	void			InitDevComm();
	void			ProcessDevMsg(WPARAM wParam,LPARAM lParam);
	void			ProcessNetConnectState(BYTE DevCode, BYTE bConnect);
	void			ProcessDeviceInfo(BYTE *info, unsigned long len, unsigned char port);
	//生成设备，读配置文件
	void			ReadIniFile();
	void			CoConnectLink();
	void			CalcBinAddress();
public:
	CPLCDev					*m_pLCDev;
	CTLog					m_PLCLog;
private:
	CBrush					m_BackBrush;
	CTaskbarNotifier		m_PromptNotifier;
	//屏幕分辨率
	UINT					m_WndWidth;
	UINT					m_WndHigh;
	BOOL					m_bFinishedConn;
	UINT					m_ConnectDevId;
	UINT					m_ConnectDevNum;
	CString					m_strBinAddress;
	CString					m_strAlarmMusic;
// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg LRESULT OnTCPInfoRecv(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnConnectState(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnDevUpdateMsg(WPARAM wParam,LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


