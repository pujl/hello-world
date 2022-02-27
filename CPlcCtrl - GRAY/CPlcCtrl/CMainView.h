// CMainView.h : CMainView 类的接口
//


#pragma once
#include "CsutomBtn.h"
#include "ColorStatic.h"
#include "RectWnd.h"
#include "CPlcCtrlDoc.h"
#include "ListCtrlCl.h"
#include "TsWnd.h"

const UINT  MAX_NUM = 8;

class CMainView : public CFormView
{
protected: // 仅从序列化创建
	CMainView();
	DECLARE_DYNCREATE(CMainView)

public:
	enum{ IDD = IDD_CPLCCTRL_FORM };

// 属性
public:
	CCPlcCtrlDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnInitialUpdate(); // 构造后第一次调用

// 实现
public:
	virtual ~CMainView();

	void DevUpdate(UINT nDevCode,UINT nUpdateId);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

private:
	CCsutomBtn		m_ExitBtn;
	CCsutomBtn		m_ConnectBtn;
	CColorStatic	m_TimeStatic;
	CColorStatic	m_CapStatic;
	CColorStatic	m_ShowStatic1;
	CColorStatic	m_ShowStatic2;
	CColorStatic	m_ShowStatic3;
	HBITMAP			m_RedBMP;
	HBITMAP			m_YellowBMP;
	HBITMAP			m_OrangeBMP;
	HBITMAP			m_GrayBMP;
	HBITMAP			m_GreenBMP;

	CRectWnd		m_ShowWnd[3];
	CListCtrlCl		m_MsgList;
	CColorStatic	m_MsgCap;
	LOGFONT			m_lNormalFont;
	LOGFONT			m_lShowFont;
	LOGFONT			m_lTimeFont;
	LOGFONT			m_lCapFont;
	CPen			penSlow;
	CPen			penBlack;

	UINT			m_ErrNum;
	CString			m_strRcdFormat;
	//通信状态
	CColorStatic	m_TxCap;
	CStatic			m_TxStatic;
	//综合异常
	CColorStatic	m_ZhCap;
	CStatic			m_ZhStatic;
	//互锁切
	CColorStatic	m_LockCap;
	CStatic			m_LockStatic;
	//PLC运行
	CColorStatic	m_PLCRunCap;
	CStatic			m_PLCRunStatic;

	CTsWnd			m_TsWnd;
	UINT			m_PlaySoundFlag;
	UINT			m_CapFtSize;
	UINT			m_TimeFtSize;
	UINT			m_NormalFtSize;
	UINT			m_ShowFtSize;
	CString			m_StrTitle;
	int				m_TitleWidth;
	int 			m_TimeWidth;
// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnBtnExit();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBtnConnect();
	afx_msg void OnPaint();
};

#ifndef _DEBUG  // CMainView.cpp 中的调试版本
inline CCPlcCtrlDoc* CMainView::GetDocument() const
   { return reinterpret_cast<CCPlcCtrlDoc*>(m_pDocument); }
#endif

