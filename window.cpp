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
    QLabel* label = new QLabel(this);
    label->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    label->setText("0");
    label->setAlignment(Qt::AlignBottom | Qt::AlignRight);
    renderArea->label = label;
    mainLayout->addWidget(label, 1, 3, 1, 4);
    setLayout(mainLayout);
    setWindowTitle(tr("QT Gui"));
}

float Window::gainCurrentModifier(){
    return renderArea->gainCurrentModifier();
}
