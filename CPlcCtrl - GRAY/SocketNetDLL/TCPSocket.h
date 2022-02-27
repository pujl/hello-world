#if !defined(AFX_USERSOCKET_H__7DEF6A3E_2306_4EC6_B682_7BE8256E2905__INCLUDED_)
#define AFX_USERSOCKET_H__7DEF6A3E_2306_4EC6_B682_7BE8256E2905__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UserSocket.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CTCPSocket command target

class CTCPSocket : public CAsyncSocket
{
// Attributes
public:

// Operations
public:
	CTCPSocket();
	virtual ~CTCPSocket();

// Overrides
public:
	void WriteNormalPort(BYTE DataArray[], int nSize);
	void AnalysisOtherDevices();//����һ��Э����豸
	void ExpandBuffer(unsigned char *buffer, unsigned long len, unsigned char port);

public:
	void SetIniFilePath();

	//��������ļ�����·��
	inline char *GetIniFilePath(){return m_IniFilePath;}
	unsigned char GetJYXOR(unsigned char* lpData,unsigned int nlen);
	unsigned char GetJYSum(unsigned char* lpData, unsigned int nLen);
#pragma pack(push,1)
	//���ջ�����
	unsigned char tempbuf[MAX_TEMP_BUFFER];
	//һ��������
	unsigned char Read_Buf[MAX_TEMP_BUFFER];
	//���λ�����
	unsigned char Temp_Buf[MAX_TEMP_BUFFER];
	unsigned long Read_Point;
	unsigned long Write_Point;

	CString m_Name;//�豸����
	BYTE m_DevCode;//�豸��ʶ��

//	BOOL Find_End;//�ҵ���Ϣβ
#pragma pack(pop,1)
	CString m_PeerName;//Զ��IP��ַ
	UINT	m_PeerPort;//Զ�˶˿ں�

private:
	char m_IniFilePath[300];
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTCPSocket)
	public:
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnAccept(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CTCPSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USERSOCKET_H__7DEF6A3E_2306_4EC6_B682_7BE8256E2905__INCLUDED_)
