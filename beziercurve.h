#pragma once
#include <QPainter>
#include "RangeLookUpStatic.h"

class BezierCurve {
    private:
        QPainter* painter;
        const QPoint* points;
        const QPoint* handlers;
        int numOfPoints;
        int cache;
        RangeLookUpStatic<std::tuple<QPoint, QPoint>> rangeHandler;

    public:
        BezierCurve(QPainter* mpainter, const QPoint mpoints[], const QPoint mhandlers[]);
        int GetCurvePoint(int position);
};