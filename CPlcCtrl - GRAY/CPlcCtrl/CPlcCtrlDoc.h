// CPlcCtrlDoc.h : CCPlcCtrlDoc ��Ľӿ�
//


#pragma once


class CCPlcCtrlDoc : public CDocument
{
protected: // �������л�����
	CCPlcCtrlDoc();
	DECLARE_DYNCREATE(CCPlcCtrlDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CCPlcCtrlDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


