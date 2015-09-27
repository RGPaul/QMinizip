TEMPLATE = lib

CONFIG += c++11
CONFIG += shared

VERSION = 1.0.0

DEFINES += Q_MINIZIP_LIBRARY=1

macx::QMAKE_MAC_SDK = macosx10.11

win32::INCLUDEPATH += "C:/include"
win32::LIBS += "C:/lib/zdll.lib"

# link zlib
unix::LIBS += -lz

# QMinizip headers / sources
HEADERS += QMinizip.h
SOURCES += QMinizip.cpp

# minizip headers
HEADERS += minizip/zip.h \
           minizip/unzip.h \
           minizip/ioapi.h \
           minizip/ioapi_buf.h \
           minizip/ioapi_mem.h \
           minizip/crypt.h
win32::HEADERS += minizip/iowin32.h

# minizip sources
SOURCES += minizip/zip.c \
           minizip/unzip.c \
           minizip/ioapi.c \
           minizip/ioapi_buf.c \
           minizip/ioapi_mem.c
win32::SOURCES += minizip/iowin32.c

# installation
unix {
    libfile.path = /usr/local/lib
    macx { libfile.files = libQMinizip.$${VERSION}.dylib }
    !macx { libfile.files = libQMinizip.$${VERSION}.so }
    headerfile.path = /usr/local/include/QMinizip
    headerfile.files = QMinizip.h
    INSTALLS += libfile
    INSTALLS += headerfile
}
