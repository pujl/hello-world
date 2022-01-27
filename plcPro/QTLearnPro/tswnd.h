#ifndef TSWND_H
#define TSWND_H

#include <QWidget>
#include "enumStruct.h"

class TsWnd : public QWidget
{
    Q_OBJECT
public:
    explicit TsWnd(QWidget *parent = nullptr);

public:
    void SetCollisionStripsState(CAR_STATE eState,quint8 nId);
    void SetIsPlanStop(bool bStop);
protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
signals:


private:
    int     m_WndHigh;
    int     m_WndWidth;
    int     m_nStripWidth;
    int     m_nStripHigh;
    bool	m_bPlanStop;
    CAR_STATE       m_eCarStates[8];
};
#endif // TSWND_H
