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
include (database/database.pri)
include (image/image.pri)
include (gui/gui.pri)

SOURCES += main.cpp\
    server/imageuploader.cpp \
    server/setcreator.cpp \
    common/miscutils.cpp \
    worker/workthread.cpp \
    import/directoryimporter.cpp

HEADERS  += \
    server/imageuploader.h \
    server/setcreator.h \
    common/miscutils.h \
    worker/workthread.h \
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































