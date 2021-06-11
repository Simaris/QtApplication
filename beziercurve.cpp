#include "beziercurve.h"
#include "RangeLookUpStatic.h"


BezierCurve::BezierCurve(QPainter* mpainter, const QPoint mpoints[6], const QPoint mhandlers[5]) : rangeHandler(5, 0, 100){
    painter = mpainter;
    points = mpoints;
    handlers = mhandlers;
    numOfPoints = 6;
    cache = 0;
    rangeHandler.AddRange(mpoints[0].x(), mpoints[1].x(), std::tuple<QPoint, QPoint, QPoint>(mpoints[0], mpoints[1], mhandlers[0]));
    rangeHandler.AddRange(mpoints[1].x(), mpoints[2].x(), std::tuple<QPoint, QPoint, QPoint>(mpoints[1], mpoints[2], mhandlers[1]));
    rangeHandler.AddRange(mpoints[2].x(), mpoints[3].x(), std::tuple<QPoint, QPoint, QPoint>(mpoints[2], mpoints[3], mhandlers[2]));
    rangeHandler.AddRange(mpoints[3].x(), mpoints[4].x(), std::tuple<QPoint, QPoint, QPoint>(mpoints[3], mpoints[4], mhandlers[3]));
    rangeHandler.AddRange(mpoints[4].x(), mpoints[5].x(), std::tuple<QPoint, QPoint, QPoint>(mpoints[4], mpoints[5], mhandlers[4]));
};

int BezierCurve::GetCurvePoint(int position){
    std::tuple<QPoint, QPoint, QPoint> neighbourPoints = rangeHandler[position].payload;
    int npos = position - std::get<0>(neighbourPoints).x();
    float rpos = static_cast<float>(npos)/(std::get<1>(neighbourPoints).x() - std::get<0>(neighbourPoints).x());
    int result = (1 - rpos) * ((1 - rpos) * std::get<0>(neighbourPoints).y() + rpos * std::get<2>(neighbourPoints).y()) + rpos * ((1 - rpos) * std::get<2>(neighbourPoints).y() + rpos * std::get<1>(neighbourPoints).y());
    return result;
}