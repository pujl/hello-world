// CPlcCtrlDoc.cpp : CCPlcCtrlDoc ���ʵ��
//

#include "stdafx.h"
#include "CPlcCtrl.h"

#include "CPlcCtrlDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCPlcCtrlDoc

IMPLEMENT_DYNCREATE(CCPlcCtrlDoc, CDocument)

BEGIN_MESSAGE_MAP(CCPlcCtrlDoc, CDocument)
END_MESSAGE_MAP()


// CCPlcCtrlDoc ����/����

CCPlcCtrlDoc::CCPlcCtrlDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CCPlcCtrlDoc::~CCPlcCtrlDoc()
{
}

BOOL CCPlcCtrlDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CCPlcCtrlDoc ���л�

void CCPlcCtrlDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CCPlcCtrlDoc ���

#ifdef _DEBUG
void CCPlcCtrlDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCPlcCtrlDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CCPlcCtrlDoc ����
