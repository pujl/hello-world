#pragma once

// CTLog
enum TLOGPRIORITY
{
	TLP_DEBUG = 0,	//����
	TLP_TIP,		//��ʾ
	TLP_WARNING,	//����
	TLP_ERROR		//����
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
	//�����ļ�����·����Ϣ
	void	InitLogFile(CString strFileName,CString TitleName,CString sPath);
	
private:
	//д�ָ���
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


