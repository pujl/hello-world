// CsutomBtn.cpp : implementation file
//

#include "stdafx.h"
#include "CsutomBtn.h"
#include "resource.h"
#include "DicnaryQry.h"
/////////////////////////////////////////////////////////////////////////////
// CCsutomBtn

CCsutomBtn::CCsutomBtn()
{
	m_bPress=FALSE;
	m_bDown=FALSE;
	m_BtnId = 0;
}

CCsutomBtn::~CCsutomBtn()
{
}


BEGIN_MESSAGE_MAP(CCsutomBtn, CButton)
	//{{AFX_MSG_MAP(CCsutomBtn)
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCsutomBtn message handlers

void CCsutomBtn::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	m_bPress=TRUE;
	//发送按下消息，某些按钮需要处理该消息
//	Invalidate();
	CButton::OnLButtonDown(nFlags, point);
}

void CCsutomBtn::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (!m_bDown)
	{
		m_bPress = FALSE;
	}
	
	//	Invalidate();
	CButton::OnLButtonUp(nFlags, point);
}

void CCsutomBtn::SetBitmaps(UINT nBitmapIn, UINT nBitmapOut ,UINT nBitmapDisable)
{
	m_Bitmap[0].LoadBitmap(nBitmapIn);
	m_Bitmap[1].LoadBitmap(nBitmapOut);
	m_Bitmap[2].LoadBitmap(nBitmapDisable);
	BITMAP csBitmapSize;
	m_Bitmap->GetBitmap(&csBitmapSize);
	CRect rect1;
	GetParent()->GetWindowRect(&rect1);
	CRect rect;
	GetWindowRect(&rect);
	rect.OffsetRect(-1*rect1.left,-1*rect1.top);
	rect.right=rect.left+csBitmapSize.bmWidth;
	rect.bottom=rect.top+csBitmapSize.bmHeight;
	MoveWindow(rect);
	GetWindowRect(&rect);
}


void CCsutomBtn::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CButton::PreSubclassWindow();
	ModifyStyle(0, BS_OWNERDRAW);
}

void CCsutomBtn::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
/*	static bool bfirst=true;
	if (bfirst==true)
	{*/
		BOOL m_bIsDisabled = (lpDrawItemStruct->itemState & ODS_DISABLED);
		CRect rect =  lpDrawItemStruct->rcItem;
		CDC *pDC=CDC::FromHandle(lpDrawItemStruct->hDC);
		CDC MemDC;
		CBitmap *poldBitmap=NULL;
		MemDC.CreateCompatibleDC(pDC);
		UINT fontw=18,fonth=0;
		if (m_bIsDisabled)
		{
			poldBitmap=MemDC.SelectObject(&m_Bitmap[2]);
//			pDC->BitBlt(0,0,rect.Width(),rect.Height(),&MemDC,0,0,SRCCOPY);
		//	pDC->SelectStockObject(SYSTEM_FONT) ;
		}
		else
		{
			if (!m_bPress)
			{
				poldBitmap=MemDC.SelectObject(&m_Bitmap[0]);
			}
			else
				poldBitmap=MemDC.SelectObject(&m_Bitmap[1]);
//			pDC->BitBlt(0,0,rect.Width(),rect.Height(),&MemDC,0,0,SRCCOPY);
//			MemDC.SelectObject(poldBitmap);	
		}
		pDC->BitBlt(0,0,rect.Width(),rect.Height(),&MemDC,0,0,SRCCOPY);
		MemDC.SelectObject(poldBitmap);	
		pDC->SetBkMode(TRANSPARENT);
		if (!m_bPress)
		{
			pDC->SetTextColor(WHITE);
		}
		else
			pDC->SetTextColor(RGB(255,0,255));
		CString sTitle;
		if(m_BtnId == 0)
		{
			GetWindowText(sTitle);
		}else
		{
			sTitle= CDicnaryQry::GetBtnName(m_BtnId);
		}

		CFont nFont ,* nOldFont; 
		if(GetBtnType() == NUM_BTN)
		{
			if(GetCmdId() == 0x0D || GetCmdId() == 0x0E)
			nFont.CreateFont(-20,fonth,0,0,FW_NORMAL,0,0,0,DEFAULT_CHARSET,
				OUT_CHARACTER_PRECIS,CLIP_CHARACTER_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,_TEXT("黑体"));//创建字体 
			else
			{
				nFont.CreateFont(-25,fonth,0,0,FW_NORMAL,0,0,0,DEFAULT_CHARSET,
					OUT_CHARACTER_PRECIS,CLIP_CHARACTER_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,_TEXT("黑体"));//创建字体 
			}
		}
		else if (m_bPress)
		{
			nFont.CreateFont(-1*fontw,fonth,0,0,FW_NORMAL,0,0,0,DEFAULT_CHARSET,
				OUT_CHARACTER_PRECIS,CLIP_CHARACTER_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,_TEXT("黑体"));//创建字体 
		}
		else
		{ 
			nFont.CreateFont(-1*fontw,fonth,0,0,FW_NORMAL,0,0,0,DEFAULT_CHARSET,
				OUT_CHARACTER_PRECIS,CLIP_CHARACTER_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,_TEXT("黑体"));//创建字体 
		}

			
		nOldFont = pDC->SelectObject(&nFont);
		CRect centerRect = rect;
		pDC->DrawText(sTitle, -1, centerRect, DT_WORDBREAK | DT_CENTER | DT_CALCRECT);
		centerRect.SetRect((rect.Width() - centerRect.Width())/2,(rect.Height() - centerRect.Height())/2,
			(rect.Width() + centerRect.Width())/2,(rect.Height() + centerRect.Height())/2);
		pDC->DrawText(sTitle, -1, centerRect, DT_WORDBREAK | DT_CENTER);
		pDC->BitBlt(0,0,rect.Width(),rect.Height(),&MemDC,0,0,SRCCOPY);
		pDC->SelectObject(nOldFont);
}

BOOL CCsutomBtn::OnEraseBkgnd(CDC* pDC) 
{
// TODO: Add your message handler code here and/or call default
//	return TRUE;
	return CButton::OnEraseBkgnd(pDC);
}

void CCsutomBtn::SetBtnId(UINT nId)
{
	EnableWindow(TRUE);
//	ShowWindow(SW_SHOW);

	m_BtnId = nId;
	Invalidate();
}

UINT CCsutomBtn::GetBtnId()
{
	return m_BtnId;
}

UINT CCsutomBtn::GetCmdId()
{
	return (m_BtnId&BTN_CMD_MASK);
}

BTN_STYLE CCsutomBtn::GetBtnType()
{
	BTN_STYLE btnstyle = DEFAULT_BTN;
	switch(m_BtnId&BTN_CATEGORY_MASK)
	{
	case BTNTY_NUM_KEYBOARD:
		btnstyle = NUM_BTN;
		break;
	case BTNTY_NORMAL_STYLE:
		btnstyle = NORMAL_BTN;
		break;
	case BTNTY_NUM_GUIDE:
		btnstyle = NUM_GUIDE_BTN;
		break;
	default:
		break;
	}
	return btnstyle;
}

void CCsutomBtn::SetPressDown(BOOL AlwaysDown)
{
	m_bDown=AlwaysDown;
	if (m_bDown)
	{
		m_bPress=TRUE;
	}
	else
		m_bPress=FALSE;
	InvalidateRect(NULL, FALSE);  //让按钮重画

}

void CCsutomBtn::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(!m_bDown)
	{
		if (m_bPress)
		{
			TRACKMOUSEEVENT tme;
			tme.cbSize = sizeof(tme);
			tme.hwndTrack = m_hWnd;
			tme.dwFlags = TME_LEAVE | TME_HOVER;
			tme.dwHoverTime = 1;
			m_bPress = _TrackMouseEvent(&tme);
		}

	}
	CButton::OnMouseMove(nFlags, point);
}
LRESULT CCsutomBtn::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	if (m_bDown)
	{
		m_bPress = TRUE;
	}
	else 
		m_bPress = FALSE; 
	InvalidateRect(NULL, FALSE);  //让按钮重画
	return 0;
}

LRESULT CCsutomBtn::OnMouseHover(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

BOOL CCsutomBtn::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_LBUTTONDBLCLK)
	{
		pMsg->message=0;
		return FALSE;        
	}
	return CButton::PreTranslateMessage(pMsg);
}

BOOL CCsutomBtn::GetPressDown()
{
	return m_bDown;
}