#pragma once
#include <QPainter>
#include "RangeLookUpStatic.h"

class BezierCurve {
    private:
        int numOfPoints;
        RangeLookUpStatic<std::tuple<QPoint, QPoint, QPoint>> rangeHandler;

    public:
        BezierCurve(const QPoint mpoints[], const QPoint mhandlers[]);
        int GetCurvePoint(int position);
};