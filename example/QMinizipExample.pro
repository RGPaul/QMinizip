TEMPLATE = app

TARGET = QMinizip

QT += core
QT -= gui

CONFIG += c++11
CONFIG += console
CONFIG -= app_bundle

# link zlib
LIBS += -lz

# link QMinizip
# in this example we assume that we installed the headers to /usr/local/include
# and that we installed the libQMinizip library to /usr/local/lib
INCLUDEPATH += /usr/local/include
LIBS += -L/usr/local/lib  -lQMinizip

SOURCES += example.cpp
