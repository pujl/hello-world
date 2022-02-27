// TLog.cpp : 实现文件
//

#include "stdafx.h"
#include "TLog.h"


// CTLog
//日志记录系统，每天记录一条

CTLog::CTLog()
{
	m_hThread = NULL;
	m_TLogPriority = TLP_TIP;
	m_ullMaxFileSize = 1000000;
	m_pFile = NULL;
	m_hMsgRecv[0] = CreateEvent(NULL,TRUE,FALSE,NULL);
	m_hMsgRecv[1] = CreateEvent(NULL,TRUE,FALSE,NULL);
	m_bOpen = FALSE;
}

CTLog::~CTLog()
{
	if(m_hThread != NULL)
	{
		SetEvent(m_hMsgRecv[0]);
		Sleep(200);
	}
	if(m_pFile != NULL)
	{
		delete m_pFile;
		m_pFile = NULL;
	}
}

// CTLog 消息处理程序
DWORD CTLog::TLogThread(LPVOID wParam)
{
	CTLog* pLog = (CTLog*)wParam;
	DWORD dwWait = 0;
	while(dwWait = WaitForMultipleObjects(2,pLog->m_hMsgRecv,FALSE,INFINITE))
	{
		if(WAIT_OBJECT_0 == dwWait)
		{
			break;
		}
		else if((WAIT_OBJECT_0+1)==dwWait)
		{
			pLog->m_csTLog.Lock();
			if(pLog->m_strTLogBuffer.GetLength())
			{
				pLog->m_pFile->SeekToEnd();
				pLog->m_pFile->WriteString(pLog->m_strTLogBuffer);
				pLog->m_pFile->Flush();
				pLog->m_strTLogBuffer.Empty();
			}
			pLog->m_csTLog.Unlock();
			if(pLog->m_pFile->m_hFile != INVALID_HANDLE_VALUE)
			{
				if(pLog->m_pFile->GetLength() >= pLog->m_ullMaxFileSize)
				{
					//日志记录文件太大则更换文件记录，目前未做
					//
				}
			}
			ResetEvent(pLog->m_hMsgRecv[1]);
		}
	}
	pLog->m_csTLog.Lock();
	if(pLog->m_pFile->m_hFile != INVALID_HANDLE_VALUE)
	{
		pLog->m_pFile->WriteString(pLog->m_strTLogBuffer);
		pLog->m_pFile->Flush();
		pLog->m_pFile->Close();
	}
	pLog->m_csTLog.Unlock();

	TRACE("文件日志记录线程退出\r\n");
	return 0;
}

void CTLog::OpenFile()
{
	CTime tm = CTime::GetCurrentTime();
	CString str = tm.Format("%Y-%m-%d");	
	CString strFileName;
	strFileName.Format("%s\\%s[%s].log",m_strTLogFilePath,m_strFileName,str);

	CFileFind findfile;
	BOOL bFind = findfile.FindFile(strFileName,0);
	if(bFind)
	{
		m_bOpen = m_pFile->Open(strFileName,CFile::modeWrite, NULL);	
		m_pFile->SeekToEnd();

		WriteDividLine();
	}else
	{
		m_bOpen = m_pFile->Open(strFileName,CFile::modeCreate|CFile::modeWrite, NULL);	
	}
		
	WriteTitleLine();

// 	str = tm.Format("%Y-%m-%d %H:%M:%S");
// 	CString strNewLine;
// 	strNewLine = _T("-----------------------") + str + _T("-------------------------");
// 	m_pFile->WriteString(strNewLine);
}

void CTLog::WriteDividLine()
{
	CString str = "-----------------------------------------------------------\r\n";
	if(m_bOpen)
	{
		m_pFile->WriteString(str);
	}
}

void CTLog::WriteTitleLine()
{
	if(m_bOpen)
	{
		m_pFile->WriteString(m_TitleInfo);
	}
}

void CTLog::InitLogFile(CString strFileName,CString TitleName,CString sPath)
{
	//得到当前软件目录地址，并打开一个新的log文件记录
	m_strTLogFilePath = sPath + "\\log";
	m_strFileName = strFileName;
	m_TitleInfo = TitleName;

	m_pFile = new CStdioFile;
	//打开文件
	OpenFile();

	if(m_bOpen)
	{
		//打开线程
		DWORD dwID = 0;
		m_hThread = CreateThread(NULL,0,TLogThread,this,0,&dwID);
		CloseHandle(m_hThread);
	}else
	{
		//文件打开失败
		TRACE("文件打开失败\r\n");
	}
}

BOOL CTLog::TLogLine(CString strTLogLine,TLOGPRIORITY tlp)
{
	if(tlp < m_TLogPriority)
	{
		//记录等级太低，不记录
		return FALSE;
	}
	if(strTLogLine.GetLength()>0)
	{
		//CTime tm = CTime::GetCurrentTime();
		//CString str = tm.Format("%Y-%m-%d %H:%M:%S");		

		m_csTLog.Lock();

		//m_strTLogBuffer = m_strTLogBuffer + "\r\n";
		m_strTLogBuffer += strTLogLine;

		m_csTLog.Unlock();
		
		SetEvent(m_hMsgRecv[1]);
		return TRUE;
	}else
	{
		return FALSE;
	}
}