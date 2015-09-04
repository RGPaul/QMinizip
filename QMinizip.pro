TEMPLATE = lib

CONFIG += c++11
CONFIG += staticlib

# link zlib
LIBS += -lz

# QMinizip sources
HEADERS += QMinizip.h
SOURCES += QMinizip.cpp

# minizip sources
HEADERS += minizip/zip.h minizip/unzip.h
SOURCES += minizip/zip.c minizip/unzip.c
