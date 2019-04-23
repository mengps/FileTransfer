#ifndef SCANNERITEM_H
#define SCANNERITEM_H

#include <QQuickPaintedItem>

struct Point
{
    QPoint point;
    int alpha;

    Point(const QPoint &p, int a) : point(p) , alpha(a) { }
};

class ScannerItem : public QQuickPaintedItem
{
    Q_OBJECT

public:
    ScannerItem(QQuickItem *parent = nullptr);
    ~ScannerItem();

    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();

protected:
    virtual void paint(QPainter *painter);

private:
    bool m_drawable = false;
    int m_angle = 0;
    QList<Point> m_points;
    QTimer *m_updateTimer;
};

#endif // SCANNERITEM_H
