// UserSocket.cpp : implementation file
//

#include "stdafx.h"
#include "CommHead.h"
#include "TCPSocket.h"
#include <mmsystem.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTCPSocket
CArray<CTCPSocket * ,CTCPSocket *> m_NetClientArray;
CArray<CTCPSocket * ,CTCPSocket *> m_NetServerArray;
CCriticalSection m_ReadNetCS;        //控制写入管理
//CCriticalSection m_NoNetCS;        //控制写入管理
CCriticalSection m_ConnectNetCS;        //控制写入管理
NETINFO ReadNetInfo;

NETINFO ConnectNetInfo;

const unsigned char START_CHAR1 = 0x80;
const unsigned char START_CHAR2 = 0x00;
const unsigned char START_CHAR3 = 0x07;
const unsigned char START_CHAR4 = 0x00;
const unsigned char START_CHAR5 = 0x22;

CTCPSocket::CTCPSocket()
{
	memset(Read_Buf,0,MAX_TEMP_BUFFER);
	memset(Temp_Buf,0,MAX_TEMP_BUFFER);
	Read_Point=0;
	Write_Point=0;
	m_DevCode = 0;

	memset(m_IniFilePath,0,sizeof(m_IniFilePath));
	SetIniFilePath();
}

CTCPSocket::~CTCPSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CTCPSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CTCPSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CTCPSocket member functions


void CTCPSocket::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class

	memset(tempbuf,0,MAX_TEMP_BUFFER);

	
	int len=Receive((char*)tempbuf,MAX_TEMP_BUFFER);
	int Curlen=(Write_Point+MAX_TEMP_BUFFER-Read_Point)%MAX_TEMP_BUFFER;//获得当前读写长度差
	if ((MAX_TEMP_BUFFER-(unsigned long )Curlen)<=(unsigned long )len)//清除缓冲区的内容
	{
		CAsyncSocket::OnReceive(nErrorCode);
		return ;
	}

	for (int i=0;i<len;i++)
	{
		Temp_Buf[Write_Point]=tempbuf[i];
		Write_Point=(Write_Point+1)%MAX_TEMP_BUFFER;
	}

	/*
	unsigned char buft[200];
	memset(buft,0,200);
	memcpy(buft,tempbuf,200);

 	TRACE("%s len = %d\r\n",m_Name,len);
 	if (len<100)
 	{
 		memcpy(buft,tempbuf,len);
 		CString str,str1;
 		str1.Format("rev= ");
 		str+=str1;
 		for(int j=0;j<len;j++)
 		{
 			if (j!=len-1)
 			{
 				str1.Format("%02X ",buft[j]);
 			}
 			else
 				str1.Format("%02X \r\n",buft[j]);
 			str+=str1;
 		}
 		TRACE(str);
 	}
	*/
	AnalysisOtherDevices();
	
	CAsyncSocket::OnReceive(nErrorCode);
}

BYTE TCPNetClientSetting(LPCSTR CommName,BYTE &DevCode)//设置
{
	CTCPSocket *pCurrentSocket=NULL;
	CTCPSocket *pOldSocket = NULL;
	pCurrentSocket=new CTCPSocket;

	pCurrentSocket->m_Name.Format("%s",CommName);
	pCurrentSocket->m_DevCode = ::GetPrivateProfileInt("设备配置信息",pCurrentSocket->m_Name,0,pCurrentSocket->GetIniFilePath());
	DevCode = pCurrentSocket->m_DevCode;
	//先断开，再重连
	for (int i=0;i<m_NetClientArray.GetSize();i++)
	{
		pOldSocket=m_NetClientArray.GetAt(i);
		if (pOldSocket->m_DevCode==DevCode)
		{
			pOldSocket->Close();
			delete pOldSocket;
			pOldSocket = NULL;
			m_NetClientArray.RemoveAt(i);
			break;
		}
	}
	//
	CString Portstr;
	Portstr.Format("%sIP地址",CommName);
	::GetPrivateProfileString("网络通信配置",Portstr,NULL,
		pCurrentSocket->m_PeerName.GetBuffer(128),128,pCurrentSocket->GetIniFilePath());
	Portstr.Format("%s监听端口号",CommName);
	pCurrentSocket->m_PeerPort = ::GetPrivateProfileInt("网络通信配置",Portstr,0,pCurrentSocket->GetIniFilePath());
	
	if (!pCurrentSocket->Create())
	{
		delete pCurrentSocket;
		pCurrentSocket = NULL;
		TRACE("套接字创建失败! %d\r\n",GetLastError());
		return 1;
	}
	int revbuf=1024*1024*3+66;
	BOOL bRet=pCurrentSocket->SetSockOpt(SO_RCVBUF,&revbuf,sizeof(int));
	
	pCurrentSocket->Connect(pCurrentSocket->m_PeerName,pCurrentSocket->m_PeerPort);
	
	//清除已断开的连接
	for (int i=0;i<m_NetClientArray.GetSize();i++)
	{
		if (pCurrentSocket->m_Name.Compare(m_NetClientArray.GetAt(i)->m_Name) == 0)
		{
			CTCPSocket *pSocket = m_NetClientArray.GetAt(i);
			delete pSocket;
			pSocket = NULL;
			m_NetClientArray.RemoveAt(i);
			break;
		}
	}

	m_NetClientArray.Add(pCurrentSocket);
	return 0;
}

BYTE TCPNetServiceSetting(LPSTR CommName,BYTE &DevCode)//设置
{
	CTCPSocket *pCurrentSocket=NULL;
	pCurrentSocket=new CTCPSocket;
	UINT m_ServicePort;
	CString Portstr;
	Portstr.Format("%s监听端口号",CommName);

	m_ServicePort = ::GetPrivateProfileInt("网络通信配置",Portstr,0,pCurrentSocket->GetIniFilePath());
	
	pCurrentSocket->m_Name.Format("%s",CommName);
	pCurrentSocket->m_PeerPort = m_ServicePort;
	
	pCurrentSocket->m_DevCode = ::GetPrivateProfileInt("设备配置信息",pCurrentSocket->m_Name,0,pCurrentSocket->GetIniFilePath());
	DevCode = pCurrentSocket->m_DevCode;
	
	if(!pCurrentSocket->Create(m_ServicePort))
	{
		delete pCurrentSocket;
		pCurrentSocket = NULL;
		TRACE("套接字创建失败! %d\r\n",GetLastError());
		return 1;
	}
	if(!pCurrentSocket->Listen())
	{
		delete pCurrentSocket;
		pCurrentSocket = NULL;
		TRACE("套接字端口监听失败! %d\r\n",GetLastError());
		return 2;
	}
	
	m_NetServerArray.Add(pCurrentSocket);
	return 0;
}

BYTE UDPNetSetting(LPCSTR CommName)//设置
{
	CTCPSocket *pCurrentSocket=NULL;
	CTCPSocket *pOldSocket = NULL;
	pCurrentSocket=new CTCPSocket;

	pCurrentSocket->m_Name.Format("%s",CommName);
	pCurrentSocket->m_DevCode = ::GetPrivateProfileInt("设备配置信息",pCurrentSocket->m_Name,0,pCurrentSocket->GetIniFilePath());

	for (int i=0;i<m_NetClientArray.GetSize();i++)
	{
		pOldSocket=m_NetClientArray.GetAt(i);
		if (pOldSocket->m_DevCode== pCurrentSocket->m_DevCode)
		{
			pOldSocket->Close();
			delete pOldSocket;
			pOldSocket = NULL;
			m_NetClientArray.RemoveAt(i);
			break;
		}
	}

	UINT m_ServicePort;
	CString Portstr;
	Portstr.Format("%s监听端口号",CommName);
	m_ServicePort = ::GetPrivateProfileInt("网络通信配置",Portstr,0,pCurrentSocket->GetIniFilePath());
	
	if(!pCurrentSocket->Create(m_ServicePort,SOCK_DGRAM))
	{
		delete pCurrentSocket;
		pCurrentSocket = NULL;
		TRACE("UDP套接字创建失败! %d\r\n",GetLastError());
		return 1;
	}
	Portstr.Format("%s发送IP",CommName);
	::GetPrivateProfileString("网络通信配置",Portstr,NULL,
		pCurrentSocket->m_PeerName.GetBuffer(128),128,pCurrentSocket->GetIniFilePath());
	Portstr.Format("%s发送端口号",CommName);
	pCurrentSocket->m_PeerPort = ::GetPrivateProfileInt("网络通信配置",Portstr,0,pCurrentSocket->GetIniFilePath());
	
	m_NetClientArray.Add(pCurrentSocket);

	return 0;
}

BOOL INITTCPNET()//网络初始化
{	
	memset(&ReadNetInfo,0,sizeof(ReadNetInfo));
	memset(&ConnectNetInfo,0,sizeof(ConnectNetInfo));
	while(m_NetClientArray.GetSize()!=0)
	{
		CTCPSocket *pdnet1=m_NetClientArray.GetAt(0);
		pdnet1->Close();
		delete pdnet1;
		pdnet1=NULL;
		m_NetClientArray.RemoveAt(0);
	}
	m_NetClientArray.SetSize(0,15);
	
	while(m_NetServerArray.GetSize()!=0)
	{
		CTCPSocket *pdnet2=m_NetServerArray.GetAt(0);
		pdnet2->Close();
		delete pdnet2;
		pdnet2=NULL;
		m_NetServerArray.RemoveAt(0);
	}
	m_NetServerArray.SetSize(0,15);
	return TRUE;
}
BYTE SendNetData(UINT nDevCode,unsigned char *buf,int len)
{
	CTCPSocket *pCurrentSocket=NULL;
	int i=0;

	for(i=0;i<m_NetClientArray.GetSize();i++)
	{
		pCurrentSocket=m_NetClientArray.GetAt(i);
		if(pCurrentSocket->m_DevCode == nDevCode)
		{
			break;
		}
	}

	if((i==m_NetClientArray.GetSize()))
	{
		CString str1;
		str1.Format("No %d Net Connect!\r\n",nDevCode);
//		TRACE(str1);
		return 0;
	}

	pCurrentSocket->WriteNormalPort(buf, len);
	return 1;

}
void ExitNetDll()
{	
	while(m_NetServerArray.GetSize()!=0)
	{	
		CTCPSocket *pdnet2=NULL;
		pdnet2 = m_NetServerArray.GetAt(0);
		
		pdnet2->Close();
		delete pdnet2;
		pdnet2=NULL;
		
		m_NetServerArray.RemoveAt(0);
	}
	
	while(m_NetClientArray.GetSize()!=0)
	{	
		CTCPSocket *pdnet1=NULL;
		pdnet1=m_NetClientArray.GetAt(0);
		
		pdnet1->Close();
		delete pdnet1;
		pdnet1=NULL;
		
		m_NetClientArray.RemoveAt(0);
	}
}

void CTCPSocket::AnalysisOtherDevices() //各个设备
{
	/*
	//直接放，不解析的那种
	unsigned int nlen = 0;
	nlen = (Write_Point - Read_Point+MAX_TEMP_BUFFER)%MAX_TEMP_BUFFER;

	for(unsigned int i = 0;i<nlen;i++)
	{
		Read_Buf[i] = Temp_Buf[Read_Point];
		Read_Point = (Read_Point + 1)%MAX_TEMP_BUFFER;
	}
	ExpandBuffer(Read_Buf,nlen,m_DevCode);
	*/
	unsigned int nlen = 0;
	nlen = (Write_Point - Read_Point+MAX_TEMP_BUFFER)%MAX_TEMP_BUFFER;
	while(nlen>=11)
	{
		if((Temp_Buf[Read_Point] == START_CHAR1)&&
			(Temp_Buf[(Read_Point+1)%MAX_TEMP_BUFFER] == START_CHAR2)&&
			(Temp_Buf[(Read_Point+2)%MAX_TEMP_BUFFER] == START_CHAR3)&&
			(Temp_Buf[(Read_Point+3)%MAX_TEMP_BUFFER] == START_CHAR4)&&
			(Temp_Buf[(Read_Point+4)%MAX_TEMP_BUFFER] == START_CHAR5))
		{
			//find head 
			for(unsigned int i = 0;i<6;i++)
			{
				Read_Buf[i] = Temp_Buf[(i+Read_Point+5)%MAX_TEMP_BUFFER];
			}

			ExpandBuffer(Read_Buf,6,m_DevCode);
			Read_Point = (Read_Point+11)%MAX_TEMP_BUFFER;
			nlen = (Write_Point - Read_Point + MAX_TEMP_BUFFER)%MAX_TEMP_BUFFER;
		}else
		{
			TRACE("头不对\r\n");
			Read_Point = (Read_Point + 1)%MAX_TEMP_BUFFER;
			nlen = (Write_Point - Read_Point + MAX_TEMP_BUFFER)%MAX_TEMP_BUFFER;
		}
	}
}

void CTCPSocket::ExpandBuffer(unsigned char *buffer, unsigned long len, unsigned char port)
{
	CWnd *pMain=AfxGetApp()->GetMainWnd();
	HWND hWnd=pMain->GetSafeHwnd();
	m_ReadNetCS.Lock();
	unsigned long Curlen=(ReadNetInfo.WritePoint+MAX_SENDNET_BUFFER-ReadNetInfo.ReadPoint)%MAX_SENDNET_BUFFER;
	if ((MAX_SENDNET_BUFFER-Curlen)<=(len+5))
	{
		m_ReadNetCS.Unlock();
		return;
	}
	unsigned char *tempbuf=NULL;
	tempbuf=new unsigned char [len+5];
	memset(tempbuf,0,len+5);
	tempbuf[0] = port;
	memcpy(&tempbuf[1],&len,sizeof(unsigned long));
	memcpy(&tempbuf[5],buffer,len);
	unsigned long tempwrite=ReadNetInfo.WritePoint;
	for (unsigned long i=0;i<len+5;i++)
	{
		ReadNetInfo.pdata[tempwrite]=tempbuf[i];
		tempwrite=(tempwrite+1)%MAX_SENDNET_BUFFER;
	}
	ReadNetInfo.WritePoint=tempwrite;
	if(tempbuf)
	{
		delete []tempbuf;
		tempbuf=NULL;
	}
	::PostMessage(hWnd,NET_DATA_ARRIVE,(WPARAM)&ReadNetInfo,(LPARAM)NULL);//
	m_ReadNetCS.Unlock();

}

void CTCPSocket::OnClose(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	CWnd *pMain=AfxGetApp()->GetMainWnd();
	HWND hWnd=pMain->GetSafeHwnd();
	m_ConnectNetCS.Lock();
	unsigned long len;
	
	int type=0,b_Connect=0;
	len = sizeof(type);
	len += sizeof(b_Connect);
	
	type = m_DevCode;
	b_Connect = 0;
	
	unsigned char port =1;
	unsigned long Curlen=(ConnectNetInfo.WritePoint+MAX_SENDNET_BUFFER-ConnectNetInfo.ReadPoint)%MAX_SENDNET_BUFFER;
	if ((MAX_SENDNET_BUFFER-Curlen)<=(len+5))
	{
		m_ConnectNetCS.Unlock();
		return;
	}
	unsigned char *tempbuf=NULL;
	tempbuf=new unsigned char [len+5];
	memset(tempbuf,0,len+5);
	memcpy(tempbuf,&len,sizeof(unsigned long));
	memcpy(&tempbuf[4],&port,1);
	memcpy(&tempbuf[5],&type,sizeof(type));
	memcpy(&tempbuf[5+sizeof(type)],&b_Connect,sizeof(b_Connect));
	unsigned long tempwrite=ConnectNetInfo.WritePoint;
	for (unsigned long i=0;i<len+5;i++)
	{
		ConnectNetInfo.pdata[tempwrite]=tempbuf[i];
		tempwrite=(tempwrite+1)%MAX_SENDNET_BUFFER;
	}
	ConnectNetInfo.WritePoint=tempwrite;
	if(tempbuf)
	{
		delete []tempbuf;
		tempbuf=NULL;
	}
	::PostMessage(hWnd,NET_CONNECT_OK,(WPARAM)&ConnectNetInfo,(LPARAM)NULL);

	m_ConnectNetCS.Unlock();

	CAsyncSocket::OnClose(nErrorCode);
}

void CTCPSocket::OnConnect(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	CWnd *pMain=AfxGetApp()->GetMainWnd();
	HWND hWnd=pMain->GetSafeHwnd();
	m_ConnectNetCS.Lock();
	unsigned long len;
	
	int type=0,b_Connect=0;
	len = sizeof(type);
	len += sizeof(b_Connect);

	type = m_DevCode;


	if (nErrorCode == 0)
	{
		b_Connect = 1;
	}
	else
	{
		b_Connect = 0;
	}

	unsigned char port =1;
	unsigned long Curlen=(ConnectNetInfo.WritePoint+MAX_SENDNET_BUFFER-ConnectNetInfo.ReadPoint)%MAX_SENDNET_BUFFER;
	if ((MAX_SENDNET_BUFFER-Curlen)<=(len+5))
	{
		m_ConnectNetCS.Unlock();
		return;
	}
	unsigned char *tempbuf=NULL;
	tempbuf=new unsigned char [len+5];
	memset(tempbuf,0,len+5);
	memcpy(tempbuf,&len,sizeof(unsigned long));
	memcpy(&tempbuf[4],&port,1);
	memcpy(&tempbuf[5],&type,sizeof(type));
	memcpy(&tempbuf[5+sizeof(type)],&b_Connect,sizeof(b_Connect));
	unsigned long tempwrite=ConnectNetInfo.WritePoint;
	for (unsigned long i=0;i<len+5;i++)
	{
		ConnectNetInfo.pdata[tempwrite]=tempbuf[i];
		tempwrite=(tempwrite+1)%MAX_SENDNET_BUFFER;
	}
	ConnectNetInfo.WritePoint=tempwrite;
	if(tempbuf)
	{
		delete []tempbuf;
		tempbuf=NULL;
	}
	::PostMessage(hWnd,NET_CONNECT_OK,(WPARAM)&ConnectNetInfo,(LPARAM)NULL);//
	m_ConnectNetCS.Unlock();

	CAsyncSocket::OnConnect(nErrorCode);
}

void CTCPSocket::OnAccept(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	CTCPSocket *pCurrentSocket=NULL;

	for (int i=0;i<m_NetClientArray.GetSize();i++)
	{
		pCurrentSocket=m_NetClientArray.GetAt(i);
		if (pCurrentSocket->m_DevCode==m_DevCode)
		{
			pCurrentSocket->Close();
			delete pCurrentSocket;
			pCurrentSocket = NULL;
			m_NetClientArray.RemoveAt(i);
			break;
		}
		TRACE("OnAccept Delete\r\n");
	}	
	pCurrentSocket=new CTCPSocket;
	pCurrentSocket->m_DevCode=m_DevCode;
	pCurrentSocket->m_Name.Format("%s",m_Name);
	Accept(*pCurrentSocket);
	m_NetClientArray.Add(pCurrentSocket);

	CWnd *pMain=AfxGetApp()->GetMainWnd();
	HWND hWnd=pMain->GetSafeHwnd();
	m_ConnectNetCS.Lock();
	unsigned long len;

	int type=0,b_Connect=0;
	len = sizeof(type);
	len += sizeof(b_Connect);
	type = pCurrentSocket->m_DevCode;
	b_Connect = 1;
	unsigned char port =1;
	unsigned long Curlen=(ConnectNetInfo.WritePoint+MAX_SENDNET_BUFFER-ConnectNetInfo.ReadPoint)%MAX_SENDNET_BUFFER;
	if ((MAX_SENDNET_BUFFER-Curlen)<=(len+5))
	{
		m_ConnectNetCS.Unlock();
		return;
	}
	unsigned char *tempbuf=NULL;
	tempbuf=new unsigned char [len+5];
	memset(tempbuf,0,len+5);
	memcpy(tempbuf,&len,sizeof(unsigned long));
	memcpy(&tempbuf[4],&port,1);
	memcpy(&tempbuf[5],&type,sizeof(type));
	memcpy(&tempbuf[5+sizeof(type)],&b_Connect,sizeof(b_Connect));
	unsigned long tempwrite=ConnectNetInfo.WritePoint;
	for (unsigned long j=0;j<len+5;j++)
	{
		ConnectNetInfo.pdata[tempwrite]=tempbuf[j];
		tempwrite=(tempwrite+1)%MAX_SENDNET_BUFFER;
	}
	ConnectNetInfo.WritePoint=tempwrite;
	if(tempbuf)
	{
		delete []tempbuf;
		tempbuf=NULL;
	}
	::PostMessage(hWnd,NET_CONNECT_OK,(WPARAM)&ConnectNetInfo,(LPARAM)NULL);//
	m_ConnectNetCS.Unlock();

	CAsyncSocket::OnAccept(nErrorCode);
}

void CTCPSocket::WriteNormalPort(BYTE DataArray[], int nSize)
{
	unsigned char ProcessData[200];
	unsigned long infolen=0;
	memset(ProcessData,0,sizeof(ProcessData));
	int i=0,j=0;

	memcpy(&ProcessData[infolen],DataArray,nSize);
	infolen += nSize;

	unsigned char JYdata = 0;

	if(m_DevCode <= 5)
	{
		if(Send(ProcessData,infolen)==SOCKET_ERROR)
		{
			CString str2;
			str2.Format(" %s网络数据发送故障,故障代码是%d\r\n",m_Name,GetLastError( ));
			TRACE(str2);
		}
	}else
	{
		//udp发送
		if(SendTo(ProcessData,infolen,m_PeerPort,m_PeerName)==SOCKET_ERROR)
		{
			CString str2;
			str2.Format(" %s网络数据发送故障,故障代码是%d\r\n",m_Name,GetLastError( ));
			TRACE(str2);
		}
	}

	/*
	if (infolen<200)
	{
		BYTE buft[200];
		memset(buft,0,200);
		memcpy(buft,ProcessData,infolen);
		CString str,str1;
		str1.Format("send= %s ",m_Name);
		str+=str1;
		for(unsigned long j=0;j<infolen;j++)
		{
			if (j!=infolen-1)
			{
				str1.Format("%02X ",buft[j]);
			}
			else
				str1.Format("%02X \r\n",buft[j]);
			str+=str1;
		}
		TRACE(str);
	}
	*/
}

void CTCPSocket::SetIniFilePath()
{
	TCHAR exeFullPath[MAX_PATH]="\0"; // MAX_PATH在API中有定义，为128
	int len=GetModuleFileName(NULL,
		exeFullPath, //应用程序的全路径存放地址
		MAX_PATH);

	CString pathname;
	pathname.Format("%s",exeFullPath);

	int end=0;
	end=pathname.ReverseFind('\\');
	pathname.Delete(end+1,pathname.GetLength()-end-1);
	wsprintf(m_IniFilePath,"%s%s",pathname,"通信配置.ini");
}

unsigned char CTCPSocket::GetJYXOR(unsigned char* lpData,unsigned int nlen)
{
	unsigned int i=0;
	unsigned char Xor=0;
	if(NULL!=lpData && nlen!=0)
	{
		for(i=0;i<nlen-1;i++)
			Xor^=lpData[i];
	}
	return Xor;
}

unsigned char CTCPSocket::GetJYSum(unsigned char* lpData, unsigned int nLen)   
{
	unsigned int i=0;
	unsigned char Sum=0;
	if(NULL!=lpData && nLen!=0)
	{
		for(i=0;i<nLen-1;i++)
			Sum+=lpData[i];
	}
	return Sum;
}