#ifndef COMMEN_H
#define COMMEN_H

#include <QRgb>
#include <QDateTime>
#include "enumStruct.h"


const  QString          DEVNAME_PLC	= "PLC";
const  quint32			DEVID_PLC	= 0;


const unsigned int		DEVID_LEFT1 = 0x01;
const unsigned int		DEVID_LEFT2 = 0x02;
const unsigned int		DEVID_LEFT3 = 0x03;
const unsigned int		DEVID_LEFT4 = 0x04;

const unsigned int		DEVID_RIGHT1 = 0x05;
const unsigned int		DEVID_RIGHT2 = 0x06;
const unsigned int		DEVID_RIGHT3 = 0x07;
const unsigned int		DEVID_RIGHT4 = 0x08;

const QRgb INIT_CL      = 0x123456;
const QRgb ERR_CL       = 0x111111;
const QRgb NORMAL_CL    = 0x125634;

#pragma pack(push)
#pragma pack(1)

typedef struct PLC_STATE{
    unsigned short	_D800;
    unsigned short	_D801;
    unsigned short	_D802;
}PLC_STATE;

typedef struct PLC_RECORD{
    bool  bChange;
    CAR_STATE   m_eCurState;
    QDateTime	m_ErrTime;
    QDateTime  m_RecoverTime;
}PLC_RECORD,*lpPLC_RECORD;

#pragma pack(pop)

#endif // COMMEN_H
