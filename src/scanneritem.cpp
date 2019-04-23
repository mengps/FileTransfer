#include "scanneritem.h"

#include <QQuickWindow>
#include <QPainter>
#include <QTimer>
#include <QtMath>

ScannerItem::ScannerItem(QQuickItem *parent)
    : QQuickPaintedItem (parent)
{
    qsrand(uint (time(nullptr)));
    m_updateTimer = new QTimer(this);
    m_updateTimer->setInterval(16);
    connect(m_updateTimer, &QTimer::timeout, this, [this](){ update(); });

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this]
    {
        m_points.clear();
        for(int i = 0; i < 5; ++i)
        {
            int alpha = qrand() % 100 + 40;
            int px = qrand() % int(width());
            int py = qrand() % int(height());
            m_points.append(Point(QPoint(px, py), alpha));
        }
    });
    timer->start(1400);
}

ScannerItem::~ScannerItem()
{

}

void ScannerItem::start()
{
    m_drawable = true;
    m_angle = 0;
    m_updateTimer->start();
}

void ScannerItem::stop()
{
    m_drawable = false;
    m_angle = 0;
    m_updateTimer->stop();
    update(); //清除
}

void ScannerItem::paint(QPainter *painter)
{
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(QPen(qRgba(120, 110, 250, 250)));

    //格子
    for(int i = 0; i < width(); i += 20)
        painter->drawLine(i + 0.5, 0, i + 0.5, height());
    for(int j = 0; j < height(); j += 20)
        painter->drawLine(0, j + 0.5, width(), j + 0.5);

    int min = int(qMin(width(), height()));
    QPoint center(int(width() / 2), int(height() / 2));
    painter->drawEllipse(center, min / 2, min / 2);
    painter->drawEllipse(center, min / 3, min / 3);
    painter->drawEllipse(center, min / 6, min / 6);

    if (m_drawable)
    {
        int diff = int(qAbs(width() - height()) / 2);
        QConicalGradient gradient(width() / 2, height() / 2, m_angle + 180);
        gradient.setColorAt(0.1, QColor(15, 45, 188, 200));
        gradient.setColorAt(0.7, QColor(15, 45, 188, 0));
        painter->setBrush(gradient);
        painter->setPen(QPen(Qt::NoPen));
        if (width() > height())
            painter->drawPie(diff, 0, min, min, m_angle * 16, 60 * 16);
        else painter->drawPie(0, diff, min, min, m_angle * 16, 60 * 16);

        for(int i = 0; i < 5; ++i)
        {
            painter->setBrush(QBrush(QColor(15, 45, 188, m_points.at(i).alpha)));
            painter->drawEllipse(m_points.at(i).point, 7, 7);
        }
        m_angle -= 2;
    }
}
