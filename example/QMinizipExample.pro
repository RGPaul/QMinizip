TEMPLATE = app

TARGET = QMinizip

QT += core
QT -= gui

CONFIG += c++11
CONFIG += console
CONFIG -= app_bundle

# include path for third party libraries
unix::INCLUDEPATH += /usr/local/include
win32::INCLUDEPATH += "C:/include"

# lib path for third party libraries
unix::LIBS += -L/usr/local/lib

# link zlib
unix::LIBS += -lz
win32::LIBS += "C:/lib/zdll.lib"

# link QMinizip
unix::LIBS += -lQMinizip
win32::LIBS += "C:/lib/QMinizip.lib"

# all source files
SOURCES += example.cpp
