#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

class RenderArea;

//! [0]
class Window : public QWidget
{
    Q_OBJECT

public:
    Window();
    float gainCurrentModifier();

private:
    RenderArea *renderArea;
};
//! [0]

#endif // WINDOW_H
