// CMainView.cpp : CMainView 类的实现
//

#include "stdafx.h"
#include "CPlcCtrl.h"
#include "MakeFileContent.h"
#include "CPlcCtrlDoc.h"
#include "CMainView.h"
#include "MainFrm.h"
#include "ExitInfoDlg.h"
#include "PLCDev.h"
#include <math.h>
#include "DicnaryQry.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainView

IMPLEMENT_DYNCREATE(CMainView, CFormView)

BEGIN_MESSAGE_MAP(CMainView, CFormView)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTNEXIT, &CMainView::OnBtnExit)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BTNEXIT2, &CMainView::OnBtnConnect)
	ON_WM_PAINT()
END_MESSAGE_MAP()

// CMainView 构造/析构

CMainView::CMainView()
	: CFormView(CMainView::IDD)
{
	// TODO: 在此处添加构造代码
	memset(&m_lNormalFont,0,sizeof(m_lNormalFont));
	memset(&m_lCapFont,0,sizeof(m_lCapFont));
	memset(&m_lTimeFont,0,sizeof(m_lTimeFont));
	memset(&m_lShowFont,0,sizeof(m_lShowFont));

	m_PlaySoundFlag = AfxGetApp()->GetProfileInt("Sound","PlaySound",0);
	m_CapFtSize = AfxGetApp()->GetProfileInt("FontSize","Caption",90);
	m_TimeFtSize = AfxGetApp()->GetProfileInt("FontSize","Time",50);
	m_NormalFtSize = AfxGetApp()->GetProfileInt("FontSize","Normal",48);
	m_ShowFtSize = AfxGetApp()->GetProfileInt("FontSize","Show",35);
	m_StrTitle = AfxGetApp()->GetProfileString("Caption","Title","TFTM2淋雨线车体倾斜显示");

	m_lNormalFont.lfHeight = 0-m_NormalFtSize; 
	m_lNormalFont.lfWidth = 0; 
	m_lNormalFont.lfEscapement = 0; 
	m_lNormalFont.lfOrientation = 0; 
	m_lNormalFont.lfWeight = 1200; 
	m_lNormalFont.lfItalic = 0; 
	m_lNormalFont.lfUnderline = 0; 
	m_lNormalFont.lfStrikeOut = 0; 
	m_lNormalFont.lfCharSet = ANSI_CHARSET; 
	m_lNormalFont.lfOutPrecision = OUT_DEFAULT_PRECIS; 
	m_lNormalFont.lfClipPrecision = OUT_DEFAULT_PRECIS; 
	m_lNormalFont.lfQuality = PROOF_QUALITY; 
	m_lNormalFont.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN; 
	memset(m_lNormalFont.lfFaceName,0,sizeof(m_lNormalFont.lfFaceName));
	wsprintf(m_lNormalFont.lfFaceName,"%s",_T("黑体"));

	m_lShowFont.lfHeight = 0-m_ShowFtSize; 
	m_lShowFont.lfWidth = 0; 
	m_lShowFont.lfEscapement = 0; 
	m_lShowFont.lfOrientation = 0; 
	m_lShowFont.lfWeight = FW_HEAVY; 
	m_lShowFont.lfItalic = 0; 
	m_lShowFont.lfUnderline = 0; 
	m_lShowFont.lfStrikeOut = 0; 
	m_lShowFont.lfCharSet = ANSI_CHARSET; 
	m_lShowFont.lfOutPrecision = OUT_DEFAULT_PRECIS; 
	m_lShowFont.lfClipPrecision = OUT_DEFAULT_PRECIS; 
	m_lShowFont.lfQuality = PROOF_QUALITY; 
	m_lShowFont.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN; 
	memset(m_lShowFont.lfFaceName,0,sizeof(m_lShowFont.lfFaceName));
	wsprintf(m_lShowFont.lfFaceName,"%s",_T("黑体"));

	m_lTimeFont.lfHeight = 0-m_TimeFtSize; 
	m_lTimeFont.lfWidth = 0; 
	m_lTimeFont.lfEscapement = 0; 
	m_lTimeFont.lfOrientation = 0; 
	m_lTimeFont.lfWeight = FW_HEAVY; 
	m_lTimeFont.lfItalic = 0; 
	m_lTimeFont.lfUnderline = 0; 
	m_lTimeFont.lfStrikeOut = 0; 
	m_lTimeFont.lfCharSet = ANSI_CHARSET; 
	m_lTimeFont.lfOutPrecision = OUT_DEFAULT_PRECIS; 
	m_lTimeFont.lfClipPrecision = OUT_DEFAULT_PRECIS; 
	m_lTimeFont.lfQuality = PROOF_QUALITY; 
	m_lTimeFont.lfPitchAndFamily = DEFAULT_PITCH | FF_ROMAN; 
	memset(m_lTimeFont.lfFaceName,0,sizeof(m_lTimeFont.lfFaceName));
	wsprintf(m_lTimeFont.lfFaceName,"%s",_T("Arial Narrow"));

	m_lCapFont.lfHeight = 0-m_CapFtSize; 
	m_lCapFont.lfWidth = 0; 
	m_lCapFont.lfEscapement = 0; 
	m_lCapFont.lfOrientation = 0; 
	m_lCapFont.lfWeight = FW_HEAVY; 
	m_lCapFont.lfItalic = 0; 
	m_lCapFont.lfUnderline = 0; 
	m_lCapFont.lfStrikeOut = 0; 
	m_lCapFont.lfCharSet = ANSI_CHARSET; 
	m_lCapFont.lfOutPrecision = OUT_DEFAULT_PRECIS; 
	m_lCapFont.lfClipPrecision = OUT_DEFAULT_PRECIS; 
	m_lCapFont.lfQuality = PROOF_QUALITY; 
	m_lCapFont.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN; 
	memset(m_lCapFont.lfFaceName,0,sizeof(m_lCapFont.lfFaceName));
	wsprintf(m_lCapFont.lfFaceName,"%s","Arial Narrow");

	m_ErrNum = 0;
	m_strRcdFormat = CMakeFileContent::GetPLCInfoFormat();

	penSlow.CreatePen(PS_SOLID,1,SLOWLV);
	penBlack.CreatePen(PS_SOLID,2,RGB(0,0,0));
}

CMainView::~CMainView()
{
	
}

void CMainView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_BTNEXIT,m_ExitBtn);
	DDX_Control(pDX,IDC_BTNEXIT2,m_ConnectBtn);
	DDX_Control(pDX,IDC_TIMESTATIC,m_TimeStatic);
	DDX_Control(pDX,IDC_CAPSTATIC,m_CapStatic);
	DDX_Control(pDX,IDC_STATIC8,m_ShowStatic1);
	DDX_Control(pDX,IDC_STATIC9,m_ShowStatic2);
	DDX_Control(pDX,IDC_STATIC10,m_ShowStatic3);
	DDX_Control(pDX,IDC_STATIC2,m_TxCap);
	DDX_Control(pDX,IDC_STATIC1,m_TxStatic);
	DDX_Control(pDX,IDC_STATIC4,m_ZhCap);
	DDX_Control(pDX,IDC_STATIC3,m_ZhStatic);
	DDX_Control(pDX,IDC_STATIC6,m_LockCap);
	DDX_Control(pDX,IDC_STATIC5,m_LockStatic);
	DDX_Control(pDX,IDC_STATIC7,m_PLCRunCap);
	DDX_Control(pDX,IDC_STATIC11,m_PLCRunStatic);
	DDX_Control(pDX,IDC_MSGLIST,m_MsgList);
	DDX_Control(pDX,IDC_MSGCAP,m_MsgCap);
}

BOOL CMainView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CFormView::PreCreateWindow(cs);
}

void CMainView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	HINSTANCE hInstance = AfxGetInstanceHandle();
	m_GrayBMP = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_BITGRAY));
	m_GreenBMP = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_BITGREEN));
	m_OrangeBMP = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_ORAGLE));
	m_RedBMP = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_BITRED));
	m_YellowBMP = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_BITYELLOW));

	m_ExitBtn.SetBitmaps(IDB_BITMAP1,IDB_BITMAP2,IDB_BITMAP3);
	m_ExitBtn.SetBtnId(BTNID_EXIT_SYS);
	m_ExitBtn.SetPressDown(FALSE);

	m_ConnectBtn.SetBitmaps(IDB_BITMAP1,IDB_BITMAP2,IDB_BITMAP3);
	m_ConnectBtn.SetBtnId(BTNID_CONN_SYS);
	m_ConnectBtn.SetPressDown(FALSE);
	
	m_TimeStatic.SetBackColor(BACKCOLOR);
	m_TimeStatic.SetTextColor(TEXTCOLOR);
	m_TimeStatic.SetTextFont(m_lTimeFont);
	m_TimeStatic.SetAlignment(DT_CENTER);

	SYSTEMTIME time;
	GetLocalTime(&time);
	CString str;
	str.Format("%04d-%02d-%02d %02d:%02d:%02d",time.wYear,time.wMonth,time.wDay,time.wHour,time.wMinute,time.wSecond);
	m_TimeStatic.SetCaption(str);
	SetTimer(1,1000,NULL);

	m_CapStatic.SetBackColor(BACKCOLOR);
	m_CapStatic.SetTextColor(TEXTCOLOR);
	m_CapStatic.SetTextFont(m_lCapFont);
	m_CapStatic.SetAlignment(DT_CENTER);
	m_CapStatic.SetCaption(m_StrTitle);

	m_ShowStatic1.SetBackColor(BACKCOLOR);
	m_ShowStatic1.SetTextColor(TEXTCOLOR);
	m_ShowStatic1.SetTextFont(m_lShowFont);
	m_ShowStatic1.SetAlignment(DT_LEFT);
	m_ShowStatic1.SetCaption("计划停止");

	m_ShowStatic2.SetBackColor(BACKCOLOR);
	m_ShowStatic2.SetTextColor(TEXTCOLOR);
	m_ShowStatic2.SetTextFont(m_lShowFont);
	m_ShowStatic2.SetAlignment(DT_LEFT);
	m_ShowStatic2.SetCaption("正常");

	m_ShowStatic3.SetBackColor(BACKCOLOR);
	m_ShowStatic3.SetTextColor(TEXTCOLOR);
	m_ShowStatic3.SetTextFont(m_lShowFont);
	m_ShowStatic3.SetAlignment(DT_LEFT);
	m_ShowStatic3.SetCaption("异常");

	m_TxCap.SetBackColor(BACKCOLOR);
	m_TxCap.SetTextColor(TEXTCOLOR);
	m_TxCap.SetTextFont(m_lNormalFont);
	m_TxCap.SetAlignment(DT_LEFT);

	m_ZhCap.SetBackColor(BACKCOLOR);
	m_ZhCap.SetTextColor(TEXTCOLOR);
	m_ZhCap.SetTextFont(m_lNormalFont);
	m_ZhCap.SetAlignment(DT_LEFT);

	m_LockCap.SetBackColor(BACKCOLOR);
	m_LockCap.SetTextColor(TEXTCOLOR);
	m_LockCap.SetTextFont(m_lNormalFont);
	m_LockCap.SetAlignment(DT_LEFT);

	m_PLCRunCap.SetBackColor(BACKCOLOR);
	m_PLCRunCap.SetTextColor(TEXTCOLOR);
	m_PLCRunCap.SetTextFont(m_lNormalFont);
	m_PLCRunCap.SetAlignment(DT_LEFT);

//  	CFont font;
//  	font.CreateFontIndirect(&m_lShowFont);
//  	GetDlgItem(IDC_GRPSHOW)->SetFont(&font);

	m_ShowWnd[0].SetCarState(INIT_STATE);	
	m_ShowWnd[1].SetCarState(NORAML_STATE);
	m_ShowWnd[2].SetFlashCtrl(FALSE);
	m_ShowWnd[2].SetCarState(ERROR_STATE);

	m_MsgCap.SetBackColor(BACKCOLOR);
	m_MsgCap.SetTextColor(TEXTCOLOR);
	m_MsgCap.SetTextFont(m_lShowFont);
	m_MsgCap.SetAlignment(DT_CENTER);
	m_MsgCap.SetCaption("异常日志");

	m_MsgList.SetListUpdate(TRUE);
	m_MsgList.SetBkColor(BACKCOLOR);        //设置背景色
	m_MsgList.SetRowHeigt(25);               //设置行高度
	m_MsgList.SetHeaderHeight(1.5);          //设置头部高度
	m_MsgList.SetHeaderTextColor(RGB(255,255,255)); //设置头部字体颜色
	m_MsgList.SetHeaderBKColor(0,0,0,0.5);
	m_MsgList.SetTextColor(RGB(255,0,0)); //设置字体颜色
	DWORD dwStyle = m_MsgList.GetExtendedStyle();
	m_MsgList.SetExtendedStyle(dwStyle|LVS_EX_GRIDLINES);
	m_MsgList.SetFontHW(16,8);               //设置字体高度，和宽度,0表示缺省宽度
	m_MsgList.InsertColumn(0,_T("备用"),LVCFMT_CENTER);
	m_MsgList.InsertColumn(1,_T("序号"),LVCFMT_CENTER);
	m_MsgList.InsertColumn(2,_T("设备名称"),LVCFMT_CENTER);
	m_MsgList.InsertColumn(3,_T("异常时间"),LVCFMT_CENTER);
	m_MsgList.InsertColumn(4,_T("恢复时间"),LVCFMT_CENTER);
	m_MsgList.ShowScrollBar(SB_VERT,TRUE);
}


// CMainView 诊断

#ifdef _DEBUG
void CMainView::AssertValid() const
{
	CFormView::AssertValid();
}

void CMainView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CCPlcCtrlDoc* CMainView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCPlcCtrlDoc)));
	return (CCPlcCtrlDoc*)m_pDocument;
}
#endif //_DEBUG

// CMainView 消息处理程序

HBRUSH CMainView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性
	CMainFrame *pFrame =(CMainFrame *)AfxGetMainWnd();
// 	if(nCtlColor == CTLCOLOR_DLG)
// 	{
// 		return (HBRUSH)pFrame->GetBackBrush().GetSafeHandle();
// 	}
// 
// 	CMainFrame *pFrame =(CMainFrame *)AfxGetMainWnd();
// 	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if(nCtlColor == CTLCOLOR_DLG|CTLCOLOR_STATIC|CTLCOLOR_EDIT)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(TEXTCOLOR);
		CFont font;
		font.CreateFontIndirect(&m_lShowFont);
		pDC->SelectObject(&font);
		return (HBRUSH)pFrame->GetBackBrush().GetSafeHandle();
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

void CMainView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	if(pFrame == NULL)
	{
		return;
	}
	UINT wndHigh = pFrame->GetSwMetrics(SW_CYVALUE);
	UINT wndWidth = pFrame->GetSwMetrics(SW_CXVALUE);
	if(m_ExitBtn.GetSafeHwnd())
	{
		m_ExitBtn.MoveWindow(wndWidth-59,wndHigh-59,60,60);
		m_ConnectBtn.MoveWindow(wndWidth-59,wndHigh-129,60,60);
	}
	if(m_TimeStatic.GetSafeHwnd())
	{
		m_TimeStatic.MoveWindow(wndWidth-m_TimeWidth,30,m_TimeWidth,70);
	}
	if(m_CapStatic.GetSafeHwnd())
	{
		m_CapStatic.MoveWindow(wndWidth/2-m_TitleWidth/2,10,m_TitleWidth,120);
	}

	UINT TopSpace = wndHigh/6;
	if(m_TsWnd.GetSafeHwnd())
	{
		m_TsWnd.MoveWindow(0,TopSpace,wndWidth,TopSpace*3);
	}

	for(int i =0;i<3;i++)
	{
		if(m_ShowWnd[i].GetSafeHwnd())
		{
			m_ShowWnd[i].MoveWindow(200,TopSpace*4+100+80*i,200,50);
		}
	}

// 	if(GetDlgItem(IDC_GRPSHOW)->GetSafeHwnd())
// 	{
// 		GetDlgItem(IDC_GRPSHOW)->MoveWindow(200,TopSpace*4+50,380,250);
// 	}
	if(m_ShowStatic1.GetSafeHwnd())
	{
		m_ShowStatic1.MoveWindow(430,TopSpace*4+95,160,60);
	}
	if(m_ShowStatic2.GetSafeHwnd())
	{
		m_ShowStatic2.MoveWindow(430,TopSpace*4+175,100,60);
	}
	if(m_ShowStatic3.GetSafeHwnd())
	{
		m_ShowStatic3.MoveWindow(430,TopSpace*4+255,100,60);
	}

	UINT nSubWd = 200;
	UINT nBmpSize = 60;
	//设置一个位置基准
	UINT x_Base = 750;
	UINT y_Base = TopSpace*4+50;
	if(m_TxCap.GetSafeHwnd())
	{
		m_TxCap.MoveWindow(x_Base+5,y_Base,nSubWd,60);
	}
	if(m_TxStatic.GetSafeHwnd())
	{
		m_TxStatic.MoveWindow(x_Base+nSubWd+70,y_Base,nBmpSize,nBmpSize);
	}
	if(m_ZhCap.GetSafeHwnd())
	{
		m_ZhCap.MoveWindow(x_Base+5,y_Base+75,nSubWd,60);
	}
	if(m_ZhStatic.GetSafeHwnd())
	{
		m_ZhStatic.MoveWindow(x_Base+nSubWd+70,y_Base+75,nBmpSize,nBmpSize);
	}
	if(m_LockCap.GetSafeHwnd())
	{
		m_LockCap.MoveWindow(x_Base+5,y_Base+150,nSubWd,60);
	}
	if(m_LockStatic.GetSafeHwnd())
	{
		m_LockStatic.MoveWindow(x_Base+nSubWd+70,y_Base+150,nBmpSize,nBmpSize);
	}
	if(m_PLCRunCap.GetSafeHwnd())
	{
		m_PLCRunCap.MoveWindow(x_Base+10,y_Base+225,nSubWd,60);
	}
	if(m_PLCRunStatic.GetSafeHwnd())
	{
		m_PLCRunStatic.MoveWindow(x_Base+nSubWd+70,y_Base+225,nBmpSize,nBmpSize);
	}

	CRect rect;
	if(m_MsgList.GetSafeHwnd())
	{
		m_MsgList.MoveWindow(wndWidth-700,TopSpace*4+40,500,wndHigh-TopSpace*4-53);
		m_MsgList.GetClientRect(&rect);
		m_MsgList.SetColumnWidth(0,0);
		m_MsgList.SetColumnWidth(1,rect.Width()*6/38);
		m_MsgList.SetColumnWidth(2,rect.Width()*8/38);
		m_MsgList.SetColumnWidth(3,rect.Width()*12/38);
		m_MsgList.SetColumnWidth(4,rect.Width()*12/38);
	}
	if(m_MsgCap.GetSafeHwnd())
	{
		m_MsgCap.MoveWindow(wndWidth-700,TopSpace*4,500,40);
	}
}

void CMainView::OnBtnExit()
{
	// TODO: 在此添加控件通知处理程序代码
	CExitInfoDlg exitInfoDlg;
	exitInfoDlg.SetPromptStr("是否退出PLC控制软件？");
	exitInfoDlg.SetCaptionStr("系统退出提示消息");
	exitInfoDlg.SetDlgType(EXIT_DLG);
	CMainFrame *pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	if(exitInfoDlg.DoModal() == IDOK)
	{
		ExitNetDll();
		::PostMessage(pFrame->GetSafeHwnd(),WM_CLOSE,NULL,NULL);
	}
}

void CMainView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nIDEvent == 1)
	{
		SYSTEMTIME time;
		GetLocalTime(&time);
		CString str;
		str.Format("%04d-%02d-%02d %02d:%02d:%02d",time.wYear,time.wMonth,time.wDay,time.wHour,time.wMinute,time.wSecond);
		m_TimeStatic.SetCaption(str);
	}
	CFormView::OnTimer(nIDEvent);
}

int CMainView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	for(int i =0;i<3;i++)
	{
		m_ShowWnd[i].Create(NULL,NULL,WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,0);
	}
	m_TsWnd.Create(NULL,NULL,WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,0);

	//获取字符宽度大小
	HDC hdc = ::GetDC(GetSafeHwnd());
	HFONT hfont = CreateFontIndirect(&m_lCapFont);
	HFONT hold = (HFONT)SelectObject(hdc,hfont);
	SIZE sz;
	GetTextExtentPoint32(hdc,m_StrTitle,strlen(m_StrTitle),&sz);
	m_TitleWidth = sz.cx+20;

	hfont = CreateFontIndirect(&m_lTimeFont);
	hold = (HFONT)SelectObject(hdc,hfont);
	CString str = "2022-12-12 12-12-12";
	GetTextExtentPoint32(hdc,str,strlen(str),&sz);
	m_TimeWidth = sz.cx+20;

	::DeleteObject(SelectObject(hdc,hold));
	::ReleaseDC(GetSafeHwnd(),hdc);
	return 0;
}

void CMainView::OnBtnConnect()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame *pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	pFrame->ConnectPLC();
}

void CMainView::DevUpdate(UINT nDevCode,UINT nUpdateId)
{
	//如果nUpdateId是通信状态的话，只在主界面处理
	//如果nUpdateId是设备状态的话，在主界面和分界面都处理
	//其余信息都在分界面处理
	
	CMainFrame *pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	if(pFrame == NULL)
	{
		return;
	}
	switch(nUpdateId)
	{
	case DEV_LINK_MSG:
		{
			switch(nDevCode)
			{
			case DEVID_PLC:
				{
					if(pFrame->m_pLCDev->GetDevLink())
					{
						//"通信连接" 干一些初始化的事
						//颜色变化  
						pFrame->SetPromptNotifier("PLC设备连接成功！");
						m_TxStatic.SetBitmap(m_GreenBMP);
						m_LockStatic.SetBitmap(m_GrayBMP);
						m_ZhStatic.SetBitmap(m_GrayBMP);
						m_PLCRunStatic.SetBitmap(m_GrayBMP);
					}else
					{
						//"未连接");
						pFrame->SetPromptNotifier("PLC设备断开连接！");
						m_TxStatic.SetBitmap(m_RedBMP);
						m_ZhStatic.SetBitmap(m_GrayBMP);
						m_LockStatic.SetBitmap(m_GrayBMP);
						m_PLCRunStatic.SetBitmap(m_GrayBMP);

						m_MsgList.DeleteAllItems();
					}
				}
				break;
			default:
				break;
			}
		}
		break;
	case DEV_LINK_FIRST:
		{
			switch(nDevCode)
			{
			case DEVID_PLC:
				{
					if(pFrame->m_pLCDev->GetDevLink())
					{
						//"通信连接" 干一些初始化的事
						//颜色变化
						pFrame->SetPromptNotifier("PLC设备连接成功！");
						m_TxStatic.SetBitmap(m_GreenBMP);
						m_LockStatic.SetBitmap(m_GrayBMP);
						m_ZhStatic.SetBitmap(m_GrayBMP);
						m_PLCRunStatic.SetBitmap(m_GrayBMP);
					}else
					{
						//"未连接");
						m_TxStatic.SetBitmap(m_RedBMP);
						m_ZhStatic.SetBitmap(m_GrayBMP);
						m_LockStatic.SetBitmap(m_GrayBMP);
						m_PLCRunStatic.SetBitmap(m_GrayBMP);
					}
				}
				break;
			default:
				break;
			}
		}
		break;
	case PLC_MSGINIT:
		{
			m_TsWnd.SetIsPlanStop(TRUE);
		}
		break;
	case PLC_MSGCHANGE:
		{
			lpPLC_RECORD pRcd = pFrame->m_pLCDev->GetPlcRcd();
			for(int i =0;i<8;i++)
			{
				if(pRcd[i].bChange)
				{
					int nCount = m_MsgList.GetItemCount();
					CString str;
					if(pRcd[i].m_eCurState == ERROR_STATE)
					{
						//m_RectWnd[i].SetCarState(ERROR_STATE);
						if(m_PlaySoundFlag == 1)
						{
							pFrame->PlayAlarmVoice();
						}
						//显示表格,直接插入
						str.Format("%d",nCount+1);
						m_MsgList.InsertItem(nCount,"");
						m_MsgList.SetItemText(nCount,1,str);
						m_MsgList.SetItemData(nCount,i);
						m_MsgList.SetItemText(nCount,2,CDicnaryQry::GetDevName(i+0xFF01));
						str.Format("%02d:%02d:%02d",pRcd[i].m_ErrTime.wHour,pRcd[i].m_ErrTime.wMinute,pRcd[i].m_ErrTime.wSecond);
						m_MsgList.SetItemText(nCount,3,str);
						for(int i =1;i<5;i++)
						{
							m_MsgList.SetItemColor(i,nCount,RGB(252,255,0));
						}

						m_MsgList.SendMessage(WM_VSCROLL,SB_BOTTOM,NULL);
					}else
					{
						//m_RectWnd[i].SetCarState(NORAML_STATE);
						int j = 0;
						for(j = nCount-1;j>=0;j--)
						{
							if(i == m_MsgList.GetItemData(j))
							{
								break;
							}
						}
						if(j!=-1)
						{
							str.Format("%02d:%02d:%02d",pRcd[i].m_RecoverTime.wHour,pRcd[i].m_RecoverTime.wMinute,pRcd[i].m_RecoverTime.wSecond);
							m_MsgList.SetItemText(j,4,str);

							//整个状态切换恢复后则插入一条 
							m_ErrNum += 1;
							str.Format(m_strRcdFormat,m_ErrNum,CDicnaryQry::GetDevName(i+0xFF01),pRcd[i].m_ErrTime.wHour,
								pRcd[i].m_ErrTime.wMinute,pRcd[i].m_ErrTime.wSecond,pRcd[i].m_RecoverTime.wHour,
								pRcd[i].m_RecoverTime.wMinute,pRcd[i].m_RecoverTime.wSecond);
							if(pFrame != NULL)
							{
								pFrame->m_PLCLog.TLogLine(str);	
							}
						}
					}

				}
				
				CAR_STATE eState = pFrame->m_pLCDev->QryCarState(i);

				m_TsWnd.SetCollisionStripsState(i,eState);
			}
		}
		break;
	case PLC_MSGRLOCK:
		{
			if(pFrame->m_pLCDev->GetLockState())
			{
				m_LockStatic.SetBitmap(m_RedBMP);
			}else
			{
				m_LockStatic.SetBitmap(m_GrayBMP);
			}
		}
		break;
	case PLC_MSGRALLERR:
		{
			if(pFrame->m_pLCDev->GetAllErr())
			{
				m_ZhStatic.SetBitmap(m_RedBMP);
			}else
			{
				m_ZhStatic.SetBitmap(m_GreenBMP);
			}
		}
		break;
	case PLC_MSGRUN:
		{
			if(pFrame->m_pLCDev->GetPLCRun())
			{
				m_PLCRunStatic.SetBitmap(m_GreenBMP);
			}else
			{
				m_PLCRunStatic.SetBitmap(m_GrayBMP);
			}
		}
		break;
	default:
		break;
	}
}

void CMainView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CFormView::OnPaint()
	CMainFrame *pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);

	//dc.SelectStockObject(BLACK_BRUSH);
	dc.SelectObject(&penBlack);
	UINT x_Base = 700;
	UINT wndHigh = pFrame->GetSwMetrics(SW_CYVALUE);
	UINT TopSpace = wndHigh/6;
	UINT y_Base = TopSpace*4+40;
	dc.MoveTo(x_Base,y_Base);
	dc.LineTo(x_Base,wndHigh-13);
	dc.LineTo(x_Base+450,wndHigh-13);
	dc.LineTo(x_Base+450,y_Base);
	dc.LineTo(x_Base,y_Base);
	
	x_Base = 150;
	dc.MoveTo(x_Base,y_Base);
	dc.LineTo(x_Base,wndHigh-13);
	dc.LineTo(x_Base+450,wndHigh-13);
	dc.LineTo(x_Base+450,y_Base);
	dc.LineTo(x_Base+270,y_Base);
	dc.MoveTo(x_Base,y_Base);
	dc.LineTo(x_Base+160,y_Base);

	CFont font;
	font.CreateFontIndirect(&m_lNormalFont);
	dc.SelectObject(&font);
	dc.TextOut(x_Base+165,y_Base-25,"示例");
}	
