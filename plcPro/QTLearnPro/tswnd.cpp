#include "tswnd.h"
#include <QPainter>
#include <QDebug>
#include <QFontMetrics>
#include <QFont>
#include <QRect>
#include "getqstringdic.h"

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

void TsWnd::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    //画背景
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::gray);
    painter.drawRect(rect());

    //画8个碰撞条。
    uint8_t MidSpace = 10;
    //上下边距
    uint8_t nVSideMargin = 60;
    //左右边距
    uint8_t nHSideSpace = (m_WndWidth - m_nStripWidth*4-MidSpace*3)/2;
    painter.setFont(QFont("黑体",45,QFont::Bold));
    painter.setPen(QColor(0,0,0));
    QFontMetrics metrics = painter.fontMetrics();
    QRect rect;
    for(uint8_t i = 0;i<8;i++)
    {
        int x = nHSideSpace + m_nStripWidth * (i%4) + MidSpace * (i%4);
        int y = i>=4?(m_WndHigh-nVSideMargin-m_nStripHigh):nVSideMargin;
        rect.setRect(x,y,m_nStripWidth,m_nStripHigh);

        painter.setBrush(Qt::black);
        switch(m_eCarStates[i])
        {
        case INIT_STATE:
            painter.setBrush(QBrush(QColor(INIT_CL)));
            break;
        case NORAML_STATE:
            painter.setBrush(QBrush(QColor(NORMAL_CL)));
            break;
        case ERROR_STATE:
            painter.setBrush(QBrush(QColor(ERROR_STATE)));
            break;
        default:
            painter.setBrush(Qt::black);
            break;
        }

        painter.drawRect(rect);

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


}

void TsWnd::resizeEvent(QResizeEvent *event)
{
    m_WndHigh = this->geometry().height();
    m_WndWidth = this->geometry().width();
//    qDebug()<<"qint32(int):"<<m_WndHigh<<endl;
//    qDebug()<<"qint32(int):"<<m_WndWidth<<endl;
    m_nStripWidth = m_WndWidth*3/4/4+40;
}
