TEMPLATE = lib

CONFIG += c++11
CONFIG += staticlib

# link zlib
LIBS += -lz

# QMinizip headers / sources
HEADERS += QMinizip.h
SOURCES += QMinizip.cpp

# minizip headers
HEADERS += minizip/zip.h \
           minizip/unzip.h \
           minizip/ioapi.h \
           minizip/ioapi_buf.h \
           minizip/ioapi_mem.h
win32 {
    HEADERS += minizip/iowin32.h
}

# minizip sources
SOURCES += minizip/zip.c \
           minizip/unzip.c \
           minizip/ioapi.c \
           minizip/ioapi_buf.c \
           minizip/ioapi_mem.c
win32 {
    SOURCES += minizip/iowin32.c
}

# installation
unix {
    libfile.path = /usr/local/lib
    libfile.files = libQMinizip.a
    headerfile.path = /usr/local/include
    headerfile.files = QMinizip.h
    INSTALLS += libfile
    INSTALLS += headerfile
}
