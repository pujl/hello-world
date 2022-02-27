#pragma once

// CTLog
enum TLOGPRIORITY
{
	TLP_DEBUG = 0,	//调试
	TLP_TIP,		//提示
	TLP_WARNING,	//警告
	TLP_ERROR		//故障
};

class CTLog
{
public:
	CTLog();
	virtual ~CTLog();

public:
	static DWORD WINAPI TLogThread(LPVOID wParam);
	void	OpenFile();
	BOOL	TLogLine(CString strTLogLine,TLOGPRIORITY tlp=TLP_TIP);
	//设置文件名和路径信息
	void	InitLogFile(CString strFileName,CString TitleName,CString sPath);
	
private:
	//写分割线
	void	WriteDividLine();
	void	WriteTitleLine();
private:
	CCriticalSection	m_csTLog;
	CString		m_strTLogBuffer;
	CString		m_strTLogFilePath;
	CStdioFile	*m_pFile;
	ULONGLONG	m_ullMaxFileSize;
	TLOGPRIORITY m_TLogPriority;
	HANDLE		m_hMsgRecv[2];
	HANDLE		m_hThread;
	CString		m_strFileName;
	CString		m_TitleInfo;
	BOOL		m_bOpen;
};


