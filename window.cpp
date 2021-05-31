#include "renderarea.h"
#include "window.h"

#include <QtWidgets>

const int IdRole = Qt::UserRole;

Window::Window()
{
    renderArea = new RenderArea;
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setColumnStretch(0, 1);
    mainLayout->setColumnStretch(3, 1);
    mainLayout->addWidget(renderArea, 0, 0, 1, 4);
    setLayout(mainLayout);
    setWindowTitle(tr("QT Gui"));
}
