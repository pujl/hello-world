// ListCtrlCl.cpp : implementation file
//

#include "stdafx.h"
#include "ListCtrlCl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListCtrlCl
struct stColor
{
	int nRow;
	int nCol;
	COLORREF rgb;
};

IMPLEMENT_DYNAMIC(CListCtrlCl, CListCtrl)

CListCtrlCl::CListCtrlCl()
: m_nRowHeight(0)
, m_fontHeight(-9)
, m_fontWith(0)
{
	m_color = TEXTCOLOR;
	m_bUpdate=FALSE;
	m_bSelect=FALSE;

}

CListCtrlCl::~CListCtrlCl()
{
	RemoveAllPtr(&m_ptrListCol);
	RemoveAllPtr(&m_ptrListItem);
	RemoveAllPtr(&m_colTextColor);
	RemoveAllPtr(&m_ItemTextColor);
}


BEGIN_MESSAGE_MAP(CListCtrlCl, CListCtrl)
	//{{AFX_MSG_MAP(CListCtrlCl)
	ON_WM_MEASUREITEM()
	ON_WM_MEASUREITEM_REFLECT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListCtrlCl message handlers



void CListCtrlCl::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	ModifyStyle(0,LVS_OWNERDRAWFIXED);
	CListCtrl::PreSubclassWindow();
	CHeaderCtrl *pHeader = GetHeaderCtrl();
	m_Header.SubclassWindow(pHeader->GetSafeHwnd());
	CListCtrl::PreSubclassWindow();
}

void CListCtrlCl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{

	// TODO:  添加您的代码以绘制指定项
	TCHAR lpBuffer[256];

	LV_ITEM lvi;

	lvi.mask = LVIF_TEXT | LVIF_PARAM;
	lvi.iItem = lpDrawItemStruct->itemID; 
	lvi.iSubItem = 0;
	lvi.pszText = lpBuffer;
	lvi.cchTextMax = sizeof(lpBuffer);
	VERIFY(GetItem(&lvi));

	LV_COLUMN lvc, lvcprev;
	::ZeroMemory(&lvc, sizeof(lvc));
	::ZeroMemory(&lvcprev, sizeof(lvcprev));
	lvc.mask = LVCF_WIDTH | LVCF_FMT;
	lvcprev.mask = LVCF_WIDTH | LVCF_FMT;

	CDC* pDC;
	pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
// 	CPen pen;
// 	pen.CreatePen(PS_SOLID, 0, RGB(255,0,0));
// 	pDC->SelectObject(&pen);
	CRect rtClient;
	GetClientRect(&rtClient);
/*	for ( int nCol=0; GetColumn(nCol, &lvc); nCol++)
	{
		if ( nCol > 0 ) 
		{
			// Get Previous Column Width in order to move the next display item
			GetColumn(nCol-1, &lvcprev) ;
			lpDrawItemStruct->rcItem.left += lvcprev.cx ;
			lpDrawItemStruct->rcItem.right += lpDrawItemStruct->rcItem.left; 
		}

		CRect rcItem;   
		if (!GetSubItemRect(lpDrawItemStruct->itemID,nCol,LVIR_LABEL,rcItem))   
			continue;   

		::ZeroMemory(&lvi, sizeof(lvi));
		lvi.iItem = lpDrawItemStruct->itemID;
		lvi.mask = LVIF_TEXT | LVIF_PARAM;
		lvi.iSubItem = nCol;
		lvi.pszText = lpBuffer;
		lvi.cchTextMax = sizeof(lpBuffer);
		VERIFY(GetItem(&lvi));

		CRect rcTemp;
		rcTemp = rcItem;

		if (nCol==0)
		{
			rcTemp.left -=2;
		}

		if ( lpDrawItemStruct->itemState & ODS_SELECTED )
		{
			pDC->FillSolidRect(&rcTemp, GetSysColor(COLOR_HIGHLIGHT)) ;
			pDC->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT)) ;
		}
		else
		{
			COLORREF color;
			color = GetBkColor();
			pDC->FillSolidRect(rcTemp,color);

			if (FindColColor(nCol,color))
			{
				pDC->FillSolidRect(rcTemp,color);
			}
			if (FindItemColor(nCol,lpDrawItemStruct->itemID,color))
			{
				pDC->FillSolidRect(rcTemp,color);
			}
			
			//pDC->SetTextColor(m_color);
		}

		pDC->SelectObject(GetStockObject(DEFAULT_GUI_FONT));

		UINT   uFormat    = DT_CENTER ;
		if (m_Header.m_Format[nCol]=='0')
		{
			uFormat = DT_LEFT;
		}
		else if (m_Header.m_Format[nCol]=='1')
		{
			uFormat = DT_CENTER;
		}
		else if (m_Header.m_Format[nCol]=='2')
		{
			uFormat = DT_RIGHT;
		}
		TEXTMETRIC metric;
		pDC->GetTextMetrics(&metric);
		int ofst;
		ofst = rcItem.Height() - metric.tmHeight;
		rcItem.OffsetRect(0,ofst/2-4);//20100602
		pDC->SetTextColor(m_color);
		COLORREF color;
		if (FindColTextColor(nCol,color))
		{
			pDC->SetTextColor(color);
		}
		if (FindItemTextColor(nCol,lpDrawItemStruct->itemID,color))
		{
			pDC->SetTextColor(color);
		}
		if (strcmp(lpBuffer,"故障")==0)
		{
			pDC->SetTextColor(RGB(255,0,0));
		}
		CFont nFont ,* nOldFont; 
		nFont.CreateFont(m_fontHeight,m_fontWith,0,0,0,FALSE,FALSE,0,0,0,0,0,0,_TEXT("黑体"));//创建字体 
		nOldFont = pDC->SelectObject(&nFont);

		DrawText(lpDrawItemStruct->hDC, lpBuffer, strlen(lpBuffer), 
			&rcItem, uFormat) ;

		pDC->SelectStockObject(SYSTEM_FONT) ;
	}*/
	for ( int nCol= 0; GetColumn(nCol, &lvc); nCol++)
	{
		if ( nCol > 0 ) 
		{
			// Get Previous Column Width in order to move the next display item
			GetColumn(nCol-1, &lvcprev) ;
			lpDrawItemStruct->rcItem.left += lvcprev.cx ;
			lpDrawItemStruct->rcItem.right += lpDrawItemStruct->rcItem.left; 
		}
		CRect rcItem;   
		if (!GetSubItemRect(lpDrawItemStruct->itemID,nCol,LVIR_LABEL,rcItem))   
			continue; 

		
		::ZeroMemory(&lvi, sizeof(lvi));
		lvi.iItem = lpDrawItemStruct->itemID;
		lvi.mask = LVIF_TEXT | LVIF_PARAM;
		lvi.iSubItem = nCol;
		lvi.pszText = lpBuffer;
		lvi.cchTextMax = sizeof(lpBuffer);
		VERIFY(GetItem(&lvi));
/*		if(strcmp(lpBuffer,"")==0)
			continue;*/
		
		CRect rcTemp;
		rcTemp = rcItem;
		CDC MemDC;
		MemDC.CreateCompatibleDC(pDC);
		CBitmap Bitmap, *pOldBitmap=NULL;
		Bitmap.CreateCompatibleBitmap(pDC,rcItem.Width(),rcItem.Height());
		pOldBitmap=MemDC.SelectObject(&Bitmap);

		COLORREF color;
		color = GetBkColor();
		MemDC.FillSolidRect(0,0,rcTemp.Width(),rcTemp.Height(),color);
		
		if (FindColColor(nCol,color))
		{
			MemDC.FillSolidRect(0,0,rcTemp.Width(),rcTemp.Height(),color);
		}
		if (FindItemColor(nCol,lpDrawItemStruct->itemID,color))
		{
			MemDC.FillSolidRect(0,0,rcTemp.Width(),rcTemp.Height(),color);
		}
		
		//pDC->SetTextColor(m_color);

		MemDC.SelectObject(GetStockObject(DEFAULT_GUI_FONT));
		
		UINT   uFormat    = DT_CENTER ;
		if (m_Header.m_Format[nCol]=='0')
		{
			uFormat = DT_LEFT;
		}
		else if (m_Header.m_Format[nCol]=='1')
		{
			uFormat = DT_CENTER;
		}
		else if (m_Header.m_Format[nCol]=='2')
		{
			uFormat = DT_RIGHT;
		}

		MemDC.SetTextColor(m_color);
		if (FindColTextColor(nCol,color))
		{
			MemDC.SetTextColor(color);
		}
		if (FindItemTextColor(nCol,lpDrawItemStruct->itemID,color))
		{
			MemDC.SetTextColor(color);
		}
		/*
		CString str1,str2;
		str1.Format("%s",lpBuffer);
		str2.Format("%s","故障");
		if (str1.Find(str2)!=-1)//有故障信息
		{
			MemDC.SetTextColor(RGB(255,0,0));
		}
		str2.Format("%s","未配置");
		if (str1.Find(str2)!=-1)//有未配置信息
		{
			MemDC.SetTextColor(RGB(255,0,0));
		}
		str2.Format("%s","无效");
		if (str1.Find(str2)!=-1)//无效信息
		{
			MemDC.SetTextColor(RGB(255,0,0));
		}
		str2.Format("%s","超时");
		if (str1.Find(str2)!=-1)
		{
			MemDC.SetTextColor(RGB(255,255,0));
		}
		*/
		if (m_bSelect)
		{
			if ((lpDrawItemStruct->itemState&ODS_SELECTED )==1 )
			{
				MemDC.SetTextColor(RGB(255,255,0));
			}
		}
		
		CFont nFont ,* nOldFont; 
		nFont.CreateFont(m_fontHeight,m_fontWith,0,0,0,FALSE,FALSE,0,0,0,0,0,0,_TEXT("黑体"));//创建字体 
//		nFont.CreateFont(-16,0,0,0,FW_NORMAL,0,0,0,DEFAULT_CHARSET,
//				OUT_CHARACTER_PRECIS,CLIP_CHARACTER_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,_TEXT("黑体"));//创建字体 
		nOldFont = MemDC.SelectObject(&nFont);
		CRect centerRect ,calrect;
		calrect.left=0;
		calrect.top=0;
		calrect.right=rcTemp.Width();
		calrect.bottom=rcTemp.Height();
		centerRect=calrect;
		MemDC.DrawText(lpBuffer, -1, &centerRect, DT_WORDBREAK | DT_CENTER | DT_CALCRECT);
//		calrect.OffsetRect((calrect.Width() - centerRect.Width())/2, (calrect.Height() - centerRect.Height())/2);
		calrect.SetRect((calrect.Width() - centerRect.Width())/2,(calrect.Height() - centerRect.Height())/2,
			(calrect.Width() + centerRect.Width())/2,(calrect.Height() + centerRect.Height())/2);
		MemDC.DrawText(lpBuffer, -1, calrect, DT_WORDBREAK | DT_CENTER);
		MemDC.SelectStockObject(SYSTEM_FONT) ;
		BitBlt(lpDrawItemStruct->hDC,rcItem.left+2,rcItem.top+2,
			rcItem.Width()-4,rcItem.Height()-4,MemDC.GetSafeHdc(),2,2,SRCCOPY);
		MemDC.SelectObject(pOldBitmap);
	}

}

		// 设置表头高度
void CListCtrlCl::SetHeaderHeight(float Height) //设置表头高度
{
	m_Header.m_Height = Height;
}

void CListCtrlCl::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	
	CListCtrl::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

void CListCtrlCl::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if (m_nRowHeight>0)
	{
		lpMeasureItemStruct->itemHeight = m_nRowHeight;
	}
}

int CListCtrlCl::InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat /* = LVCFMT_LEFT */, int nWidth /* = -1 */, int nSubItem /* = -1 */)
{
	m_Header.m_HChar.Add(lpszColumnHeading);
	if (nFormat==LVCFMT_LEFT)
	{
		m_Header.m_Format = m_Header.m_Format + "0";
	}
	else if (nFormat==LVCFMT_CENTER)
	{
		m_Header.m_Format = m_Header.m_Format + "1";
	}
	else if (nFormat==LVCFMT_RIGHT)
	{
		m_Header.m_Format = m_Header.m_Format + "2";
	}
	else
	{
		m_Header.m_Format = m_Header.m_Format + "1";
	}
	return CListCtrl::InsertColumn(nCol,lpszColumnHeading,nFormat,nWidth,nSubItem);
}
// Gradient - 渐变系数，立体背景用,不用渐变设为0
void CListCtrlCl::SetHeaderBKColor(int R, int G, int B, int Gradient) //设置表头背景色
{
	m_Header.m_R = R;
	m_Header.m_G = G;
	m_Header.m_B = B;
	m_Header.m_Gradient = Gradient;
}

void CListCtrlCl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CListCtrl::OnPaint()
	CRect rect;
	CRect headerRect;
	CDC MenDC;
	CBitmap MemMap;
	GetClientRect(&rect);

	GetDlgItem(0)->GetWindowRect(&headerRect);

	MenDC.CreateCompatibleDC(&dc);
	MemMap.CreateCompatibleBitmap(&dc,rect.Width(),rect.Height());
	MenDC.SelectObject(&MemMap);
	MenDC.FillSolidRect(&rect,RGB(0,0,0));
	DefWindowProc(WM_PAINT,(WPARAM)MenDC.m_hDC,(LPARAM)0);

	//输出
	dc.BitBlt(0,headerRect.Height(),rect.Width(),rect.Height(),&MenDC,0,headerRect.Height(),SRCCOPY);
	MenDC.DeleteDC();
	MemMap.DeleteObject();
}


bool CListCtrlCl::FindColColor(int col,COLORREF &color) //查找列颜色
{
	int flag = 0;
	for (POSITION pos = m_ptrListCol.GetHeadPosition();pos!=NULL;)
	{
		stColor *pColor = (stColor*)m_ptrListCol.GetNext(pos);
		if (pColor->nCol==col)
		{
			flag = 1;
			color = pColor->rgb;
			break;
		}
	}
	if (1==flag)
	{
		return true;
	}
	return false;
}

bool CListCtrlCl::FindItemColor(int col,int row,COLORREF &color) //查找颜色
{
	int flag = 0;
	for (POSITION pos = m_ptrListItem.GetHeadPosition();pos!=NULL;)
	{
		stColor *pColor = (stColor*)m_ptrListItem.GetNext(pos);
		if (pColor->nCol==col&&pColor->nRow==row)
		{
			flag = 1;
			color = pColor->rgb;
			break;
		}
	}
	if (1==flag)
	{
		return true;
	}
	return false;
}

void CListCtrlCl::SetColColor(int col,COLORREF color) //设置列颜色
{
	stColor *pColor  = new stColor;
	pColor->nCol = col;
	pColor->rgb = color;
	m_ptrListCol.AddTail(pColor);
}

void CListCtrlCl::SetItemColor(int col,int row,COLORREF color) //设置格子颜色
{
	stColor *pColor  = new stColor;
	pColor->nCol = col;
	pColor->nRow = row;
	pColor->rgb = color;
	m_ptrListItem.AddTail(pColor);
}

void CListCtrlCl::SetRowHeigt(int nHeight) //高置行高
{
	m_nRowHeight = nHeight;

	CRect rcWin;
	GetWindowRect(&rcWin);
	WINDOWPOS wp;
	wp.hwnd = m_hWnd;
	wp.cx = rcWin.Width();
	wp.cy = rcWin.Height();
	wp.flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;
	SendMessage(WM_WINDOWPOSCHANGED, 0, (LPARAM)&wp);
}

void CListCtrlCl::SetHeaderFontHW(int nHeight,int nWith) //设置头部字体宽和高
{
	m_Header.m_fontHeight = nHeight;
	m_Header.m_fontWith = nWith;
}

void CListCtrlCl::SetHeaderTextColor(COLORREF color) //设置头部字体颜色
{
	m_Header.m_color = color;
}

BOOL CListCtrlCl::SetTextColor(COLORREF cr)  //设置字体颜色
{
	m_color = cr;
	return TRUE;
}

void CListCtrlCl::SetFontHW(int nHeight,int nWith) //设置字体高和宽
{
	m_fontHeight = nHeight;
	m_fontWith = nWith;
}

void CListCtrlCl::SetColTextColor(int col,COLORREF color)
{
	stColor *pColor = new stColor;
	pColor->nCol = col;
	pColor->rgb = color;
	m_colTextColor.AddTail(pColor);
}

bool CListCtrlCl::FindColTextColor(int col,COLORREF &color)
{
	int flag = 0;
	for (POSITION pos = m_colTextColor.GetHeadPosition();pos!=NULL;)
	{
		stColor *pColor = (stColor*)m_colTextColor.GetNext(pos);
		if (pColor->nCol==col)
		{
			flag = 1;
			color = pColor->rgb;
			break;
		}
	}
	if (1==flag)
	{
		return true;
	}
	return false;
}

bool CListCtrlCl::FindItemTextColor(int col,int row,COLORREF &color)
{
	int flag = 0;
	for (POSITION pos = m_ItemTextColor.GetHeadPosition();pos!=NULL;)
	{
		stColor *pColor = (stColor*)m_ItemTextColor.GetNext(pos);
		if (pColor->nCol==col&&pColor->nRow==row)
		{
			flag = 1;
			color = pColor->rgb;
			break;
		}
	}
	if (1==flag)
	{
		return true;
	}
	return false;
}

void CListCtrlCl::SetItemTextColor(int col,int row,COLORREF color)
{
	stColor *pColor = new stColor;
	pColor->nCol = col;
	pColor->nRow = row;
	pColor->rgb = color;
	m_ItemTextColor.AddTail(pColor);
}

void CListCtrlCl::RemoveAllPtr(CPtrList *pList)
{
/*	for (POSITION pos = pList->GetHeadPosition();pos!=NULL;)
	{
		stColor *pColor = (stColor *)pList->GetNext(pos);
		delete pColor;
		pColor = NULL;
		
	}
	pList->RemoveAll();
	TRACE ("%d \r\n",pList->GetCount());*/

	while(!pList->IsEmpty())
	{
		POSITION pos = pList->GetHeadPosition();
		stColor *pColor = (stColor*)pList->GetAt(pos);
		delete pColor;
		pColor = NULL;
		pList->RemoveAt(pos);
	}
}

BOOL CListCtrlCl::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	if (!m_bUpdate)
	{
		return TRUE;
	}
	
	return CListCtrl::OnEraseBkgnd(pDC);
}

void CListCtrlCl::SetListUpdate(BOOL bUpdate)
{
	m_bUpdate=bUpdate;
}

void CListCtrlCl::SetListSelect(BOOL bSelect)
{
	m_bSelect=bSelect;
}
