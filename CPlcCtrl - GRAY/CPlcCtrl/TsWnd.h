#pragma once


// CTsWnd

class CTsWnd : public CWnd
{
	DECLARE_DYNAMIC(CTsWnd)

public:
	CTsWnd();
	virtual ~CTsWnd();

public:
	//����8����ײ��״̬
	void	SetCollisionStripsState(BYTE nId,CAR_STATE eState);
	//���õ�ǰ�Ƿ�ƻ�ֹͣ״̬,ֹͣ״̬�²�����
	void	SetIsPlanStop(BOOL bStop);
private:
	//������������Ļ���
	void DrawCS(CDC* pDC);
	//����8����ײ��
	void DrawStrips(CDC* pDC,int nIdx,CRect &rect);
	//���Ƴ�
	void DrawCar(CDC* pDC,CRect &rect,float fRotateAng);
	//������ײЧ��
	void DrawCrashFlash(CDC* pDC,CRect &rect);
	//�������ļ�
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


