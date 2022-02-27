#pragma once
#include <afxcoll.h>
#include  <Afxtempl.h>
#include <malloc.h>
#include <afxmt.h>

#define		NET_DATA_ARRIVE			(WM_USER+105)				//数据到达消息
#define     NET_CONNECT_OK			(WM_USER+106)				//网络连接成功
#define		NET_NO_CONNECT			(WM_USER+107)	


const unsigned long		MAX_TEMP_BUFFER		= 1024*1024;		//串行接收数据环行缓冲区字节数

const unsigned long		MAX_SENDNET_BUFFER	= 1024*1024;		//网络接收数据环行缓冲区字节数

#pragma pack(push,1)

typedef struct tagNETINFO
{
	BOOL busing;
	unsigned long WritePoint;									//写指针
	unsigned long ReadPoint;									//读指针
	unsigned char pdata[MAX_SENDNET_BUFFER];					//信息的具体内容
}NETINFO,*PNETINFO;


#pragma pack(pop,1)