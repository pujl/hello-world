#pragma once
#include "EnumStruct.h"
// CBaseDev ����Ŀ��
const BYTE DEV_LINK_MSG = 0x00;
const BYTE DEV_LINK_FIRST = 0xFE;

class CMainFrame;

class CBaseDev : public CObject
{
public:
	CBaseDev();
	CBaseDev(CString name,CWnd* pWnd);
	virtual ~CBaseDev();
public:
	//�豸����
	inline void SetDevName(CString DevName)	{m_DevName = DevName;}
	//�豸���
	inline void SetDevCode(UCHAR DevCode)	{m_DevCode = DevCode;}
	//��ǰͨ�����,
	//����TCP�豸��ͨ��socket��������ж��Ƿ����ӣ�Ĭ��δ���ӣ�ͨ�����ӣ��Ͽ�����
	//����UPD�豸��ͨ����·�������ж��Ƿ����ӣ�Ĭ��δ���ӣ���·�յ�Ϊ���ӣ�3���ղ���Ϊ�Ͽ�����
	void		SetDevLink(BOOL bLink);
	//����ˢ��ָ��
	void		SetExData(CWnd* pwnd)	{m_lpExtData = pwnd;}
	//�����豸����
	CString		GetDevName();
	//�����豸���
	inline UINT	GetDevCode()		{return m_DevCode;}
	//����link
	inline BOOL GetDevLink()		{return m_bDevLink;}
	//��·������+
	void		LinkCountsPlus();
	//����MainFrame��ָ��
	void		SetFramePoint(CMainFrame* pFrame)		{m_pFrame = pFrame;}
protected:
	virtual void  TranslateMsg(unsigned char* pMsg,unsigned int len) = 0;
	//����Ͽ���Ҫ������
	virtual void  ThingByLinkMsg(bool bLink) = 0;
protected:
	void		UpdateDevInfo();
	//������·��Ϣ
	void		RecvLinkState();
protected:
	CString		m_DevName;
	CWnd*		m_lpExtData;
	UCHAR		m_DevCode;
	UINT		m_UpdateIdenty;	//����ˢ�±�ʶ
	UINT		m_TimerCount;
	BOOL		m_bCheckState;
	CMainFrame	*m_pFrame;
	//��·ʧ�ܴ���
	UINT		m_LinkFailedCnt;
private:
	BOOL		m_bDevLink;
	BOOL		m_bFirstRun;
	
};


