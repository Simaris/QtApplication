#include "beziercurve.h"


BezierCurve::BezierCurve(QPainter* mpainter, const QPoint mpoints[6], const QPoint mhandlers[5]){
    painter = mpainter;
    points = mpoints;
    handlers = mhandlers;
    numOfPoints = 6;
    cache = 0;
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
    i = std::max(i, 0);
    cache = i;
    int npos = position - points[i].x();
    float rpos = static_cast<float>(npos)/static_cast<float>(points[i + 1].x() - points[i].x());
    int result = (1 - rpos) * ((1 - rpos) * points[i].y() + rpos * handlers[i].y()) + rpos * ((1 - rpos) * handlers[i].y() + rpos * points[i + 1].y());
    return result;
}