#pragma once


// CTsWnd

class CTsWnd : public CWnd
{
	DECLARE_DYNAMIC(CTsWnd)

public:
	CTsWnd();
	virtual ~CTsWnd();

public:
	//设置8个防撞条状态
	void	SetCollisionStripsState(BYTE nId,CAR_STATE eState);
	//设置当前是否计划停止状态,停止状态下不画车
	void	SetIsPlanStop(BOOL bStop);
private:
	//负责整个区域的绘制
	void DrawCS(CDC* pDC);
	//绘制8个防撞条
	void DrawStrips(CDC* pDC,int nIdx,CRect &rect);
	//绘制车
	void DrawCar(CDC* pDC,CRect &rect,float fRotateAng);
	//绘制碰撞效果
	void DrawCrashFlash(CDC* pDC,CRect &rect);
	//读配置文件
	void ReadIniFile();
	HBITMAP GetRotatedBitmap(HBITMAP hBitmap, float radians, COLORREF clrBack,int x_mid,int y_mid);
private:
	CPen penWhite;
	CPen penRed;
	CPen penBlue;
	CPen penYellow;
	CPen penSlow;
	CPen penBlack;
	CPen penGreen;
	CFont m_Font;
	UINT  m_StripFtSize;
	UINT  m_CarLeft;

	UINT	m_WndHigh;
	UINT	m_WndWidth;
	UINT	m_nStripWidth;
	UINT	m_nStripHigh;
	BOOL	m_bPlanStop;

	BITMAP   m_bm;
	CBitmap  m_bmp;
	CDC      m_Cbm;

	CAR_STATE	m_eCarStates[8];
	CRASH_PARAM  m_CrashParam;

protected:
	//{{AFX_MSG(CTsWnd)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


