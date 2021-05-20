#include "renderarea.h"
#include "beziercurve.h"

#include <QPainter>
#include <QPainterPath>

RenderArea::RenderArea(QWidget *parent)
    : QWidget(parent)
{
    shape = Polygon;

    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}

QSize RenderArea::minimumSizeHint() const
{
    return QSize(100, 100);
}

QSize RenderArea::sizeHint() const
{
    return QSize(400, 200);
}

void RenderArea::setPen(const QPen &pen)
{
    this->pen = pen;
    update();
}

void RenderArea::paintEvent(QPaintEvent * /* event */)
{
    static const QPoint points[6] = {
        QPoint(0, 90),
        QPoint(10, 80),
        QPoint(20, 10),
        QPoint(80, 30),
        QPoint(90, 70),
        QPoint(100, 85)
    };

    static const QPoint handlers[5] = {
        QPoint(5, 50),
        QPoint(15, 90),
        QPoint(40, 30),
        QPoint(85, 10),
        QPoint(95, 100)
    };

    static const QPoint bar[2] = {
        QPoint(0, 0),
        QPoint(0, 100)
    };

    const int offset = 10;

    QRect rect(10, 20, 80, 60);

    QPainter painter(this);
    painter.setPen(pen);

    int queryPointX = 50;
    int queryPointY;
    int lastX = points[0].x();
    int lastY = points[0].y();
    BezierCurve bezierCurve(&painter, points, handlers);
    for(int x = 0; x <= 100; x++){
        painter.save();
        painter.translate(offset, offset);
        painter.drawLine(lastX, lastY, x, bezierCurve.GetCurvePoint(x));

        lastX = x;
        lastY = bezierCurve.GetCurvePoint(x);
        if(queryPointX == x){
            queryPointY = lastY;
        }
        painter.restore();
    }

    // paint side bar
    painter.save();
    painter.translate(offset + 150, offset + 0);
    painter.drawLine(bar[0], bar[1]);
    painter.restore();

    Qt::PenCapStyle cap = Qt::PenCapStyle(Qt::RoundCap);
    Qt::PenStyle style = Qt::PenStyle(Qt::SolidLine);
    Qt::PenJoinStyle join = Qt::PenJoinStyle(Qt::MiterJoin);
    painter.setPen(QPen(Qt::blue, 10, style, cap, join));
    painter.save();
    painter.translate(offset + 150, offset);
    painter.drawPoint(0, queryPointY);
    painter.restore();
    painter.save();
    painter.translate(offset, offset);
    painter.drawPoint(queryPointX, queryPointY);
    painter.restore();
}
