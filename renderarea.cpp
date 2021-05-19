#include "renderarea.h"
#include "beziercurve.h"

#include <QPainter>
#include <QPainterPath>

//! [0]
RenderArea::RenderArea(QWidget *parent)
    : QWidget(parent)
{
    shape = Polygon;

    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}
//! [0]

//! [1]
QSize RenderArea::minimumSizeHint() const
{
    return QSize(100, 100);
}
//! [1]

//! [2]
QSize RenderArea::sizeHint() const
{
    return QSize(400, 200);
}
//! [2]

//! [4]
void RenderArea::setPen(const QPen &pen)
{
    this->pen = pen;
    update();
}
//! [4]

//! [8]
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

    QRect rect(10, 20, 80, 60);

    int startAngle = 20 * 16;
    int arcLength = 120 * 16;
//! [8]

//! [9]
    QPainter painter(this);
    painter.setPen(pen);
//! [9]

//! [10]
//    for (int x = 0; x < width(); x += 100) {
//        for (int y = 0; y < height(); y += 100) {
//            painter.save();
//            painter.translate(x, y);
////! [10]
//
////! [12]
//            //if(false){
//                painter.drawPolygon(points, 4);
//            //}
//            //else{
//                painter.drawArc(rect, startAngle, arcLength);
//            //}
////! [12] //! [13]
//            painter.restore();
//        }
//    }

    int lastX = points[0].x();
    int lastY = points[0].y();
    BezierCurve bezierCurve(&painter, points, handlers);
    for(int x = 0; x <= 100; x++){
        painter.save();
        painter.translate(0, 100);
        painter.drawLine(lastX, lastY, x, bezierCurve.GetCurvePoint(x));
        lastX = x;
        lastY = bezierCurve.GetCurvePoint(x);
        painter.restore();
    }

    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setPen(palette().dark().color());
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(QRect(0, 0, width() - 1, height() - 1));
}
//! [13]
