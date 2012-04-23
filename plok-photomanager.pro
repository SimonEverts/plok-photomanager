#-------------------------------------------------
#
# Project created by QtCreator 2011-11-04T23:11:04
#
#-------------------------------------------------

QT       += core gui declarative network sql

TARGET = plok-photomanager
TEMPLATE = app

#CONFIG += crypto

# CONFIG += simon
# CONFIG += opencv

#include(3rdparty/qoauth/qoauth.pri)

include (3rdparty/3rdparty.pri)
include (model/model.pri)
include (gui/gui.pri)


SOURCES += main.cpp\
    server/imageuploader.cpp \
    server/setcreator.cpp \
    image/imageloader.cpp \
    image/imageprovider.cpp \
    image/imageloader_raw.cpp \
    image/imageloader_generic.cpp \
    database/database.cpp \
    image/thumbnailer.cpp \
    common/miscutils.cpp \
    image/thumbnailcache.cpp \
    worker/workthread.cpp \
    image/imageprocessing.cpp \
    image/image.cpp \
    image/lut.cpp \
    import/directoryimporter.cpp

HEADERS  += \
    server/imageuploader.h \
    server/setcreator.h \
    image/imageloader.h \
    image/imageprovider.h \
    image/imageloader_raw.h \
    image/imageloader_generic.h \
    database/database.h \
    image/thumbnailer.h \
    common/miscutils.h \
    image/thumbnailcache.h \
    worker/workthread.h \
    image/imageprocessing.h \
    image/image.h \
    image/lut.h \
    import/directoryimporter.h

RESOURCES += \
    icons.qrc

unix {
    LIBS += -lraw -lgomp
}

CONFIG (simon) {
    INCLUDEPATH += /home/simon/Projects/plok-photomanager/photo-backend
    LIBS +=  -lphotostore -L/home/simon/Projects/plok-photomanager/photo-backend/photostore-build-desktop-Qt_4_8_0_in_PATH__System__Release
}

CONFIG (opencv) {
    DEFINES += OPENCV

    LIBS += -lopencv_imgproc -lopencv_core
}

win32 {
    INCLUDEPATH += G:\Projects\LibRaw-0.14.4
    LIBS += -LG:\Projects\LibRaw-0.14.4\lib -llibraw
}


































