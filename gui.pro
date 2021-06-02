CONFIG += debug
QT += core gui
QT += widgets
QT_QPA_PLATFORM=wayland
requires(qtConfig(combobox))

HEADERS = renderarea.h \
          window.h \
          beziercurve.h \
          RangeLookUpStatic.h
SOURCES = gui.cpp \
          renderarea.cpp \
          window.cpp \
          beziercurve.cpp \
          RangeLookUpStatic.cpp
