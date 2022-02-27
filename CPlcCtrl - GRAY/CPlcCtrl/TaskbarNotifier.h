// CTaskbarNotifier Header file
// By John O'Byrne - 05 July 2002

#pragma once

#define WM_TASKBARNOTIFIERCLICKED	WM_USER+123
#define TN_TEXT_NORMAL			0x0000
#define TN_TEXT_BOLD			0x0001
#define TN_TEXT_ITALIC			0x0002
#define TN_TEXT_UNDERLINE		0x0004

// CTaskbarNotifier

//##ModelId=487D6C8E02DA
class CTaskbarNotifier : public CWnd
{
	DECLARE_DYNAMIC(CTaskbarNotifier)

public:
	//##ModelId=487D6C8E02DC
	CTaskbarNotifier();
	//##ModelId=487D6C8E02E4
	virtual ~CTaskbarNotifier();

	//##ModelId=487D6C8E02E6
	int Create(CWnd *pWndParent);
	//##ModelId=487D6C8E02EE
	void Show(LPCTSTR szCaption,DWORD dwTimeToShow=500,DWORD dwTimeToStay=3000,DWORD dwTimeToHide=500,int nIncrement=1);
	//##ModelId=487D6C8E02F9
	void Hide();
	//##ModelId=487D6C8E02FA
	BOOL SetSkin(UINT nBitmapID,short red=-1,short green=-1,short blue=-1);
	//##ModelId=487D6C8E0304
	BOOL SetSkin(LPCTSTR szFileName,short red=-1,short green=-1,short blue=-1);
	//##ModelId=487D6C8E030D
	void SetTextFont(LPCTSTR szFont,int nSize,int nNormalStyle,int nSelectedStyle);
	//##ModelId=487D6C8E0316
	void SetTextColor(COLORREF crNormalTextColor,COLORREF crSelectedTextColor);
	//##ModelId=487D6C8E0319
	void SetTextRect(RECT rcText);
	
	//##ModelId=487D6C8E0322
	CWnd *m_pWndParent;
	
	//##ModelId=487D6C8E032B
	CFont m_myNormalFont;
	//##ModelId=487D6C8E0330
	CFont m_mySelectedFont;
	//##ModelId=487D6C8E0334
	COLORREF m_crNormalTextColor;
	//##ModelId=487D6C8E0335
	COLORREF m_crSelectedTextColor;
	//##ModelId=487D6C8E0336
	HCURSOR m_hCursor;
	
	//##ModelId=487D6C8E033F
	CBitmap m_biSkinBackground;
	//##ModelId=487D6C8E0343
	HRGN m_hSkinRegion;
	//##ModelId=487D6C8E0348
	CRect m_rcText;
	//##ModelId=487D6C8E0349
	int m_nSkinWidth;
	//##ModelId=487D6C8E034A
	int m_nSkinHeight;

	//##ModelId=487D6C8E0352
	CString m_strCaption;
	//##ModelId=487D6C8E0353
	BOOL m_bMouseIsOver;
	//##ModelId=487D6C8E035C
	int m_nAnimStatus;

	//##ModelId=487D6C8E0366
 	DWORD m_dwTimeToShow;
	//##ModelId=487D6C8E0367
	DWORD m_dwTimeToLive;
	//##ModelId=487D6C8E0368
	DWORD m_dwTimeToHide;
	//##ModelId=487D6C8E0370
	DWORD m_dwDelayBetweenShowEvents;
	//##ModelId=487D6C8E0371
	DWORD m_dwDelayBetweenHideEvents;
	//##ModelId=487D6C8E037A
	int m_nStartPosX;
	//##ModelId=487D6C8E037B
	int m_nStartPosY;
	//##ModelId=487D6C8E037C
	int m_nCurrentPosX;
	//##ModelId=487D6C8E0384
	int m_nCurrentPosY;
	//##ModelId=487D6C8E0385
	int m_nIncrement;
	
protected:
	//##ModelId=487D6C8E0386
	BYTE* Get24BitPixels(HBITMAP pBitmap, WORD *pwWidth, WORD *pwHeight);
	//##ModelId=487D6C8E0391
	HRGN GenerateRegion(HBITMAP hBitmap, BYTE red, BYTE green, BYTE blue);

protected:
	DECLARE_MESSAGE_MAP()
public:
	//##ModelId=487D6C8E03A2
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//##ModelId=487D6C8E03A5
	afx_msg void OnDestroy();
	//##ModelId=487D6C8E03A7
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//##ModelId=487D6C8E03B0
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//##ModelId=487D6C8E03B9
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//##ModelId=487D6C8E03C1
	afx_msg LRESULT OnMouseHover(WPARAM w, LPARAM l);
	//##ModelId=487D6C8E03C5
	afx_msg LRESULT OnMouseLeave(WPARAM w, LPARAM l);
	//##ModelId=487D6C8E03CD
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//##ModelId=487D6C8E03D4
	afx_msg void OnPaint();
	//##ModelId=487D6C8E03D6
	afx_msg void OnTimer(UINT nIDEvent);
};


