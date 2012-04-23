#-------------------------------------------------
#
# Project created by QtCreator 2011-11-04T23:11:04
#
#-------------------------------------------------

QT       += core gui declarative network sql

TARGET = plok-photomanager
TEMPLATE = app

CONFIG += crypto

# CONFIG += simon
# CONFIG += opencv

include(3rdparty/qoauth/qoauth.pri)

include (gui/gui.pri)

SOURCES += main.cpp\
    server/imageuploader.cpp \
    server/setcreator.cpp \
    model/capture.cpp \
    image/imageloader.cpp \
    image/imageprovider.cpp \
    image/imageloader_raw.cpp \
    image/imageloader_generic.cpp \
    database/database.cpp \
    model/set.cpp \
    image/thumbnailer.cpp \
    common/miscutils.cpp \
    image/thumbnailcache.cpp \
    worker/workthread.cpp \
    model/picture.cpp \
    model/setmanager.cpp \
    model/setdao.cpp \
    image/imageprocessing.cpp \
    image/image.cpp \
    image/lut.cpp \
    model/picturedao.cpp \
    3rdparty/json.cpp \
    import/directoryimporter.cpp

HEADERS  += \
    server/imageuploader.h \
    server/setcreator.h \
    model/capture.h \
    image/imageloader.h \
    image/imageprovider.h \
    image/imageloader_raw.h \
    image/imageloader_generic.h \
    database/database.h \
    model/set.h \
    image/thumbnailer.h \
    common/miscutils.h \
    image/thumbnailcache.h \
    worker/workthread.h \
    model/picture.h \
    model/setmanager.h \
    model/setdao.h \
    image/imageprocessing.h \
    image/image.h \
    image/lut.h \
    model/picturedao.h \
    3rdparty/json.h \
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


































