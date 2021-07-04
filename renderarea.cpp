#include "renderarea.h"
#include "beziercurve.h"

#include <QPainter>
#include <QPainterPath>
#include <QTime>
#include <QTimer>
#include <string>
#include <QMouseEvent>
#include <QLabel>

RenderArea::RenderArea(QWidget *parent)
    : QWidget(parent)
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    // QTimer *timer = new QTimer(this);
    // connect(timer, &QTimer::timeout, this, QOverload<>::of(&RenderArea::update));
    // timer->start(100);
}

QSize RenderArea::minimumSizeHint() const
{
    return QSize(100, 100);
}

QSize RenderArea::sizeHint() const
{
    return QSize(400, 200);
}

void RenderArea::mousePressEvent(QMouseEvent *)
{
    this->setMouseTracking(true);
}

void RenderArea::mouseMoveEvent(QMouseEvent *event)
{
    step = event->pos().x() - offset;
    step = std::min(step, 100);
    step = std::max(0, step);
    this->update();
}

void RenderArea::mouseReleaseEvent(QMouseEvent *)
{
    this->setMouseTracking(false);
}

float RenderArea::gainCurrentModifier()
{
    return 1.0f;
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

    
    QPainter painter(this);
    painter.setPen(pen);

    // draw bezier line and (on the side) calculate y value on the bezier line
    int queryPointY;
    int lastX = points[0].x();  // cache for position from last iteration
    int lastY = points[0].y();
    BezierCurve bezierCurve(points, handlers);
    for(int x = 0; x <= 100; x++){
        painter.save();
        painter.translate(offset, offset);
        painter.drawLine(lastX, lastY, x, bezierCurve.GetCurvePoint(x));

        lastX = x;
        lastY = bezierCurve.GetCurvePoint(x);
        if(step == x){
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
    painter.drawPoint(step, queryPointY);
    painter.restore();
    label->setText(std::to_string(100 - queryPointY).c_str());
}
