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
	void AnalysisOtherDevices();//解析一般协议的设备
	void ExpandBuffer(unsigned char *buffer, unsigned long len, unsigned char port);

public:
	void SetIniFilePath();

	//获得配置文件完整路径
	inline char *GetIniFilePath(){return m_IniFilePath;}
	unsigned char GetJYXOR(unsigned char* lpData,unsigned int nlen);
	unsigned char GetJYSum(unsigned char* lpData, unsigned int nLen);
#pragma pack(push,1)
	//接收缓冲区
	unsigned char tempbuf[MAX_TEMP_BUFFER];
	//一包缓冲区
	unsigned char Read_Buf[MAX_TEMP_BUFFER];
	//环形缓冲区
	unsigned char Temp_Buf[MAX_TEMP_BUFFER];
	unsigned long Read_Point;
	unsigned long Write_Point;

	CString m_Name;//设备名称
	BYTE m_DevCode;//设备标识号

//	BOOL Find_End;//找到信息尾
#pragma pack(pop,1)
	CString m_PeerName;//远端IP地址
	UINT	m_PeerPort;//远端端口号

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
