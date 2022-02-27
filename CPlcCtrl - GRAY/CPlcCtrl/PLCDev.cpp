#include "StdAfx.h"
#include "PLCDev.h"
#include "DicnaryQry.h"
#include "MainFrm.h"

CPLCDev::CPLCDev(CString name,CWnd* pWnd)
	:CBaseDev(name,pWnd)
{
	for(int i =0;i<8;i++)
	{
		m_eCarState[i] = INIT_STATE;
		m_Record[i].bChange = FALSE;
		m_Record[i].m_eCurState = INIT_STATE;
	}
	m_bInit = TRUE;
	m_bAllErr = FALSE;
	m_bLock = FALSE;
	m_bPLCRun = FALSE;
}

CPLCDev::~CPLCDev(void)
{
}

void CPLCDev::TranslateMsg(unsigned char* pMsg,unsigned int len)
{
	//��ֵ
	unsigned short nIdx = 0x01;
	USHORT nTemp = 0;
	if(len == 6)
	{
		memcpy((UCHAR*)&m_PLCState,pMsg,len);

		//�ж��Ƿ�ƻ�ֹͣ,ֹͣ���ж���������
		if((m_PLCState._D800 & nIdx) == nIdx)
		{
			//if(!m_bInit)
			{
				for(int i =0;i<8;i++)
				{
					m_eCarState[i] = INIT_STATE;
				}
				//m_bInit = TRUE;

				//ˢ��
				m_UpdateIdenty = PLC_MSGINIT;
				UpdateDevInfo();
			}
		}else
		{
			//�ж��쳣
			BOOL bReport = FALSE;
			for(int i =0;i<8;i++)
			{
				nTemp = nIdx<<(i+2);
				if((m_PLCState._D800 & nTemp) == nTemp)
				{
					BOOL bChange = JudgePLCStateChange(i,ERROR_STATE);
						
					if(bChange)
					{
						//�ϱ�״̬
						bReport = TRUE;
					}
				}
			}
			for(int i =0;i<8;i++)
			{
				nTemp = nIdx<<(i+2);
				if((m_PLCState._D801 & nTemp) == nTemp)
				{
					BOOL bChange = JudgePLCStateChange(i,NORAML_STATE);

					//�ϱ�״̬

					if(bChange)
					{
						//�ϱ�״̬
						bReport = TRUE;
					}
				}
			}
			if(bReport)
			{
				m_UpdateIdenty = PLC_MSGCHANGE;
				UpdateDevInfo();
			}
		}
		//�жϻ�����
		nTemp = nIdx<<1;
		if((m_PLCState._D800 & nTemp) == nTemp)
		{
			if(!m_bLock)
			{
				m_bLock = TRUE;
				m_UpdateIdenty = PLC_MSGRLOCK;
				UpdateDevInfo();
			}
		}else
		{
			if(m_bLock)
			{
				m_bLock = FALSE;
				m_UpdateIdenty = PLC_MSGRLOCK;
				UpdateDevInfo();
			}
		}
		//�ж��ۺ��쳣��һ��ֻ�����豸�쳣���ͻ��쳣��������һ��ָʾ��
		nTemp = nIdx<<0x0A;
		if((m_PLCState._D800 & nTemp) == nTemp)
		{
			if(!m_bAllErr)
			{
				m_bAllErr = TRUE;
				m_UpdateIdenty = PLC_MSGRALLERR;
				UpdateDevInfo();
			}
		}else
		{
			if(m_bAllErr)
			{
				m_bAllErr = FALSE;
				m_UpdateIdenty = PLC_MSGRALLERR;
				UpdateDevInfo();
			}
		}
		//�ж�PLC����
		nTemp = nIdx<<0x0B;
		if((m_PLCState._D800 & nTemp) == nTemp)
		{
			if(!m_bPLCRun)
			{
				m_bPLCRun = TRUE;
				m_UpdateIdenty = PLC_MSGRUN;
				UpdateDevInfo();
			}
		}else
		{
			if(m_bPLCRun)
			{
				m_bPLCRun = FALSE;
				m_UpdateIdenty = PLC_MSGRUN;
				UpdateDevInfo();
			}
		}
	}
}

void CPLCDev::ThingByLinkMsg(bool bLink)
{
	//�豸�Ͽ�ʱ�����н�����ָ���ʼ״̬�������������ȵȣ�����
	if(!bLink)
	{
		for(int i =0;i<8;i++)
		{
			m_eCarState[i] = INIT_STATE;
		}
		//m_bInit = FALSE;

		//ˢ��
		m_UpdateIdenty = PLC_MSGINIT;
		UpdateDevInfo();
	}
}

BOOL CPLCDev::JudgePLCStateChange(BYTE nId,CAR_STATE eState)
{
	//��һ��Ӧ7 �����Ӧ6 ������Ӧ����5 ���Ķ�Ӧ����4 ��һ��Ӧ����3������
	//����һ�¶�Ӧ��ϵ
	nId = 7-nId;
	if(m_eCarState[nId] != eState)
	{
		//״̬�л�
		m_eCarState[nId] = eState;
		m_Record[nId].m_eCurState = eState;

		if(eState == ERROR_STATE)
		{
			m_Record[nId].bChange = TRUE;

			GetLocalTime(&m_Record[nId].m_ErrTime);
		}else
		{
			m_Record[nId].bChange = TRUE;

			GetLocalTime(&m_Record[nId].m_RecoverTime);
		}
		return TRUE;
	}else
	{
		m_Record[nId].bChange = FALSE;
		return FALSE;
	}
}

void CPLCDev::SendQryCmd()
{
	if(GetDevLink())
	{
		unsigned char buf[11];
		buf[0] = 0x00;
		buf[1] = 0x00;
		buf[2] = 0x07;
		buf[3] = 0x00;
		buf[4] = 0x22;
		buf[5] = 0x00;
		buf[6] = 0x18;
		buf[7] = 0x01;
		buf[8] = 0x18;
		buf[9] = 0x02;
		buf[10] = 0x18;
		SendNetData(0,buf,11);
	}
}