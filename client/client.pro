CONFIG += debug
QT += core gui
QT += widgets
requires(qtConfig(combobox))

HEADERS = client.h 
          
SOURCES = client.cpp