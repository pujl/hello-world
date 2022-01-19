#ifndef BASEDEV_H
#define BASEDEV_H

#include <QObject>
#include "commen.h"

//连接消息
const quint8 DEV_LINK_MSG = 0xFF;


class baseDev : public QObject
{
    Q_OBJECT
public:
    explicit baseDev(QObject *parent = 0);

signals:
    //发送事件
    void updateDevMsg(quint8 updateId,quint8 devCode);
public slots:

public:
    //设备名称
    void SetDevName(QString DevName)	{m_DevName = DevName;}
    //设备编号
    void SetDevCode(quint8 DevCode)     {m_DevCode = DevCode;}
    //当前通信情况,
    //对于TCP设备，通过socket连接情况判断是否连接，默认未连接，通信连接，断开连接
    //对于UPD设备，通过链路命令来判断是否连接，默认未连接，链路收到为连接，3包收不到为断开连接
    void		SetDevLink(DEV_LINK_STATE eLink);
    //返回设备名称
    QString		GetDevName();
    //返回设备编号
    quint8     GetDevCode()            {return m_DevCode;}
    //返回link
    DEV_LINK_STATE      GetDevLink()    {return m_eDevLink;}
    //链路计数器+
    void		LinkCountsPlus();
protected:
    virtual void    TranslateMsg(quint8* pMsg,quint32 len) = 0;
    //程序断开需要做的事
    virtual void    ThingByLinkMsg(DEV_LINK_STATE eLink) = 0;
protected:
    //接收链路信息
    void            RecvLinkState();
protected:
    QString             m_DevName;
    quint8              m_DevCode;
    //链路失败次数
    quint32             m_LinkFailedCnt;
private:
    DEV_LINK_STATE		m_eDevLink;
};

#endif // BASEDEV_H
