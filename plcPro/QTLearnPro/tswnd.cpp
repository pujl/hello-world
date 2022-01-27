#include "tswnd.h"
#include <QPainter>
#include <QDebug>
#include <QFontMetrics>
#include <QFont>
#include <QRect>
#include "getqstringdic.h"
#include <QPixmap>
#include <QPixmapCache>

TsWnd::TsWnd(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    hide();
    m_bPlanStop = true;
    m_nStripHigh = 80;

    for(int i =0;i<8;i++)
    {
        m_eCarStates[i] = INIT_STATE;
    }
}

void TsWnd::SetCollisionStripsState(CAR_STATE eState, quint8 nId)
{
    if(m_bPlanStop)
    {
        m_bPlanStop = false;
    }
    if(m_eCarStates[nId] != eState)
    {
        m_eCarStates[nId] = eState;
        repaint();
    }
}

void TsWnd::SetIsPlanStop(bool bStop)
{
    if(m_bPlanStop != bStop)
    {
        m_bPlanStop = bStop;
        if(m_bPlanStop)
        {
            for(int i = 0;i<8;i++)
            {
                m_eCarStates[i] = INIT_STATE;
            }
        }
        repaint();
    }
}

void TsWnd::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    //画背景
//    painter.setPen(Qt::NoPen);
//    painter.setBrush(Qt::gray);
//    painter.drawRect(rect());

    //画8个碰撞条。
    uint8_t MidSpace = 10;
    //上下边距
    uint8_t nVSideMargin = 70;
    //左右边距
    uint8_t nHSideSpace = (m_WndWidth - m_nStripWidth*4-MidSpace*3)/2;
    painter.setFont(QFont("黑体",40,QFont::Bold));

    QFontMetrics metrics = painter.fontMetrics();
    QRect rect;
    for(uint8_t i = 0;i<8;i++)
    {
        int x = nHSideSpace + m_nStripWidth * (i%4) + MidSpace * (i%4);
        int y = i>=4?(m_WndHigh-nVSideMargin-m_nStripHigh):nVSideMargin;
        rect.setRect(x,y,m_nStripWidth,m_nStripHigh);

        //painter.setBrush(Qt::white);
        switch(m_eCarStates[i])
        {
        case INIT_STATE:
            painter.setBrush(QBrush(QColor(INIT_CL)));
            painter.setPen(QColor(INIT_CL));
            break;
        case NORAML_STATE:
            painter.setBrush(QBrush(QColor(NORMAL_CL)));
            painter.setPen(QColor(NORMAL_CL));
            break;
        case ERROR_STATE:
            painter.setBrush(QBrush(QColor(ERR_CL)));
            painter.setPen(QColor(ERR_CL));
            break;
        default:
            painter.setBrush(Qt::black);
            break;
        }
        painter.drawRect(rect);
        painter.setPen(QColor(0,0,0));
        //绘制文字
        int nTemp = i<4?0:(y+m_nStripHigh);
        int nTextHigh = i<4?y:(m_WndHigh-y-m_nStripHigh);
        rect.setRect(x,nTemp,m_nStripWidth,nTextHigh);
        int stringHeight = metrics.ascent() + metrics.descent();
//        int stringWidth = metrics.horizontalAdvance("左一");
        int stringWidth = metrics.horizontalAdvance("左一");
        x = rect.x() + (rect.width() - stringWidth)/2;
        y = rect.y() + (rect.height() - stringHeight)/2+metrics.ascent();

        painter.drawText(x,y,getQStringDic::GetDevName(i+DEVID_LEFT1));
    }
    //画车
    QPixmap pic;
    pic.load(":/MyImage/image/car.bmp");
    QPixmap crashTemp;
    crashTemp.load(":/MyImage/image/crash.bmp");
    int pic_w = pic.width();
    int pic_h = pic.height();
    int nCrashSize = 143;
    if(!m_bPlanStop)
    {
        for(int i = 0;i<4;i++)
        {
            //清理缓冲区
            QPixmap temp(pic.size());
            temp.fill(Qt::transparent);
            QPainter pTemp(&temp);
            pTemp.setRenderHint(QPainter::SmoothPixmapTransform, true);
            int x = nHSideSpace + m_nStripWidth * (i%4) + MidSpace * (i%4);
            int y = m_WndHigh/2 - pic_h/2;
            if(m_eCarStates[i] == ERROR_STATE)
            {
                //显示故障效果，向上
                pTemp.translate(pic_w/2,pic_h/2);
                pTemp.rotate(30);
                pTemp.translate(-pic_w/2,-pic_h/2);
                pTemp.drawPixmap(0,0,pic);
                painter.drawPixmap(x+10,y-20,m_nStripWidth-20,m_nStripWidth-20,temp,0,0,pic_w,pic_h);

                y = nVSideMargin;
                painter.drawPixmap(x+40,y-20,nCrashSize,nCrashSize,crashTemp,0,0,crashTemp.width(),crashTemp.height());
            }else if(m_eCarStates[i+4] == ERROR_STATE)
            {
                //显示碰撞，向下
                pTemp.translate(pic_w/2,pic_h/2);
                pTemp.rotate(330);
                pTemp.translate(-pic_w/2,-pic_h/2);
                pTemp.drawPixmap(0,0,pic);
                painter.drawPixmap(x+10,y+20,m_nStripWidth-20,m_nStripWidth-20,temp,0,0,pic_w,pic_h);

                y = m_WndHigh-nVSideMargin-m_nStripHigh;
                painter.drawPixmap(x+40,y-30,nCrashSize,nCrashSize,crashTemp,0,0,crashTemp.width(),crashTemp.height());
            }else
            {
                //正常显示
                pTemp.drawPixmap(0,0,pic);
                painter.drawPixmap(x+10,y,m_nStripWidth-20,m_nStripWidth-20,temp,0,0,pic_w,pic_h);
            }
        }
    }
}

void TsWnd::resizeEvent(QResizeEvent *event)
{
    m_WndHigh = this->geometry().height();
    m_WndWidth = this->geometry().width();
//    qDebug()<<"qint32(int):"<<m_WndHigh<<endl;
//    qDebug()<<"qint32(int):"<<m_WndWidth<<endl;
    m_nStripWidth = m_WndWidth*3/4/4+40;
}


