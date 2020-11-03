QT += concurrent

TEMPLATE = lib
TARGET = steganographylib

DESTDIR = lib

CONFIG += c++11
DEFINES += STEGANOGRAPHYLIB

SOURCES += \
        filecontainer.cpp \
        imagecontainer.cpp \
        synchronizer.cpp \
        utils.cpp

HEADERS += \
    container.h \
    filecontainer.h \
    imagecontainer.h \
    steganographylib_global.h \
    synchronizer.h \
    utils.h

CONFIG(debug, debug|release) {
    unix: TARGET = $$join(TARGET,,,_debug)
    else: TARGET = $$join(TARGET,,,d)
}

unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target
