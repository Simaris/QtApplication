#include "beziercurve.h"
#include "RangeLookUpStatic.h"


BezierCurve::BezierCurve(QPainter* mpainter, const QPoint mpoints[6], const QPoint mhandlers[5]) : rangeHandler(100, 0, 100){
    painter = mpainter;
    points = mpoints;
    handlers = mhandlers;
    numOfPoints = 6;
    cache = 0;
    rangeHandler.AddRange(mpoints[0].x(), mpoints[1].x(), std::tuple<QPoint, QPoint>(mpoints[0], mpoints[1]));
    rangeHandler.AddRange(mpoints[1].x(), mpoints[2].x(), std::tuple<QPoint, QPoint>(mpoints[1], mpoints[2]));
    rangeHandler.AddRange(mpoints[2].x(), mpoints[3].x(), std::tuple<QPoint, QPoint>(mpoints[2], mpoints[3]));
    rangeHandler.AddRange(mpoints[3].x(), mpoints[4].x(), std::tuple<QPoint, QPoint>(mpoints[3], mpoints[4]));
    rangeHandler.AddRange(mpoints[4].x(), mpoints[5].x(), std::tuple<QPoint, QPoint>(mpoints[4], mpoints[5]));
};

int BezierCurve::GetCurvePoint(int position){
    int i;
    // find relevant fields
    for(i = cache; i < numOfPoints - 1 && points[i].x() < position; i++){

    }
    if(points[i].x() >= position){
        i--;
    }
    // find relevant fields end
    std::tuple<QPoint, QPoint> neighbourPoints = rangeHandler[position].payload;
    i = std::max(i, 0);
    cache = i;
    int npos = position - std::get<0>(neighbourPoints).x();
    float rpos = static_cast<float>(npos)/static_cast<float>(std::get<1>(neighbourPoints).x() - std::get<0>(neighbourPoints).x());
    int result = (1 - rpos) * ((1 - rpos) * std::get<0>(neighbourPoints).y() + rpos * handlers[i].y()) + rpos * ((1 - rpos) * handlers[i].y() + rpos * std::get<1>(neighbourPoints).y());
    return result;
}