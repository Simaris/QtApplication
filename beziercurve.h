#include <QPainter>

class BezierCurve {
    private:
        QPainter* painter;
        const QPoint* points;
        const QPoint* handlers;
        int numOfPoints;
        int cache;
    public:
        BezierCurve(QPainter* mpainter, const QPoint mpoints[], const QPoint mhandlers[]);
        int GetCurvePoint(int position);
};