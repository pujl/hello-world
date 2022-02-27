#pragma once
#include <afxcoll.h>
#include  <Afxtempl.h>
#include <malloc.h>
#include <afxmt.h>

#define		NET_DATA_ARRIVE			(WM_USER+105)				//���ݵ�����Ϣ
#define     NET_CONNECT_OK			(WM_USER+106)				//�������ӳɹ�
#define		NET_NO_CONNECT			(WM_USER+107)	


const unsigned long		MAX_TEMP_BUFFER		= 1024*1024;		//���н������ݻ��л������ֽ���

const unsigned long		MAX_SENDNET_BUFFER	= 1024*1024;		//����������ݻ��л������ֽ���

#pragma pack(push,1)

typedef struct tagNETINFO
{
	BOOL busing;
	unsigned long WritePoint;									//дָ��
	unsigned long ReadPoint;									//��ָ��
	unsigned char pdata[MAX_SENDNET_BUFFER];					//��Ϣ�ľ�������
}NETINFO,*PNETINFO;


#pragma pack(pop,1)