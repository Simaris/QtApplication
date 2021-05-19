CONFIG += debug
QT += core gui
QT += widgets
requires(qtConfig(combobox))

HEADERS       = renderarea.h \
                window.h \
                beziercurve.h
SOURCES       = gui.cpp \
                renderarea.cpp \
                window.cpp \
                beziercurve.cpp
