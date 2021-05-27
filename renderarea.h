#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QBrush>
#include <QPen>
#include <QPixmap>
#include <QWidget>

//! [0]
class RenderArea : public QWidget
{
    Q_OBJECT

public:
    enum Shape { Line, Points, Polyline, Polygon, Rect, RoundedRect, Ellipse, Arc,
                 Chord, Pie, Path, Text, Pixmap };

    explicit RenderArea(QWidget *parent = nullptr);
    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;
    int stepSize = 1;

//public slots:
//    void setPen(const QPen &pen);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Shape shape;
    QPen pen;
    int step = 0;
};
//! [0]

#endif // RENDERAREA_H