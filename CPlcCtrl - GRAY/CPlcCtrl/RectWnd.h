#pragma once
#include "EnumStruct.h"

class CRectWnd : public CWnd
{
	DECLARE_DYNAMIC(CRectWnd)

public:
	CRectWnd();
	virtual ~CRectWnd();

public:
	void SetCarState(CAR_STATE state);
	void SetFlashCtrl(BOOL bFlash);
private:
	void DrawCS(CDC* pDC);
	
private:
	COLORREF		m_rBkCl;
	COLORREF		m_rFrameCl;
	// «∑Ò…¡À∏
	BOOL			m_bIsFlash;
	CAR_STATE		m_CurState;
	BOOL			m_bFlashCtrl;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


