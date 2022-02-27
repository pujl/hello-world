// RectWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "CPlcCtrl.h"
#include "RectWnd.h"


// CRectWnd

IMPLEMENT_DYNAMIC(CRectWnd, CWnd)

CRectWnd::CRectWnd()
{
	m_rBkCl = INIT_CL;
	m_rFrameCl = m_rBkCl;
	m_CurState = INIT_STATE;
	m_bFlashCtrl = TRUE;

	UINT  nFlash;
	nFlash = AfxGetApp()->GetProfileInt("Flash","FlashFlag",0);
	if(nFlash == 0)
	{
		m_bIsFlash = FALSE;
	}else
	{
		m_bIsFlash = TRUE;
	}
}

CRectWnd::~CRectWnd()
{

}


BEGIN_MESSAGE_MAP(CRectWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()



// CRectWnd 消息处理程序
void CRectWnd::DrawCS(CDC* pDC)
{
// 	pDC->SelectStockObject(GRAY_BRUSH);
// 	pDC->SetBkMode(TRANSPARENT);
// 	pDC->SetBkColor(RGB(0,0,0));
	RECT rect;
	GetClientRect(&rect);
	RECT rect2;
	rect2.left = rect.left + 2;
	rect2.right = rect.right - 2;
	rect2.top = rect.top + 2;
	rect2.bottom = rect.bottom - 2;

	pDC->FillSolidRect(&rect,m_rFrameCl);
	pDC->FillSolidRect(&rect2,m_rBkCl);
}


void CRectWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CWnd::OnPaint()
	DrawCS(&dc);
}

void CRectWnd::SetCarState(CAR_STATE state)
{
	m_CurState =  state;
	KillTimer(1);
	switch(state)
	{
	case INIT_STATE:
		m_rBkCl = INIT_CL;
		break;
	case NORAML_STATE:
		m_rBkCl = NORAL_CL;
		break;
	case ERROR_STATE:
		if(m_bIsFlash)
		{
			SetTimer(1,200,NULL);
		}
		m_rBkCl = ERR_CL;
		break;
	default:
		m_rBkCl = BLACK;
		break;
	}
	m_rFrameCl = m_rBkCl;
	Invalidate();
}

void CRectWnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//闪烁
	if(nIDEvent == 1)
	{
		if(m_bFlashCtrl)
		{
			m_rBkCl = ERR_CL;
		}else
		{
			m_rBkCl = WHITE;
		}
		m_bFlashCtrl = !m_bFlashCtrl;
		Invalidate();
	}
	
	CWnd::OnTimer(nIDEvent);
}

void CRectWnd::SetFlashCtrl(BOOL bFlash)
{
	m_bIsFlash = bFlash;
}