QT += testlib
TARGET = test
DESTDIR = bin

INCLUDEPATH += $$PWD/../src
LIBS += -L$$PWD/../src/lib -lsteganographylib

SOURCES = \
    steganographytest.cpp \
