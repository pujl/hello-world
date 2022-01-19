#include "basedev.h"

const quint32 LINK_COUNTOUT = 5;

baseDev::baseDev(QObject *parent) : QObject(parent)
{
    m_eDevLink = NO_LINK;
    m_LinkFailedCnt = 0;
}

// CBaseDev 成员函数
void baseDev::SetDevLink(DEV_LINK_STATE eLink)
{
    if(m_eDevLink != eLink)
    {
        m_eDevLink = eLink;
        //发送链路消息
        updateDevMsg(DEV_LINK_MSG,m_DevCode);

        //设备断开或连接后初始化参数
        ThingByLinkMsg(eLink);
    }
}

QString	baseDev::GetDevName()
{
    return m_DevName;
}

void baseDev::RecvLinkState()
{
    //收到后重置计数器
    m_LinkFailedCnt = 0;
    SetDevLink(LINK_SUCC);
}

void baseDev::LinkCountsPlus()
{
    if(m_LinkFailedCnt < LINK_COUNTOUT)
    {
        m_LinkFailedCnt ++;
        if(m_LinkFailedCnt >= LINK_COUNTOUT)
        {
            SetDevLink(LINK_FAILED);
        }
    }
}
