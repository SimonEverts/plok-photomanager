#-------------------------------------------------
#
# Project created by QtCreator 2011-11-04T23:11:04
#
#-------------------------------------------------

QT       += core gui declarative network sql

TARGET = plok-photomanager
TEMPLATE = app


SOURCES += main.cpp\
        gui/mainwindow.cpp \
    gui/imageview.cpp \
    gui/thumbnailmodelitem.cpp \
    server/imageuploader.cpp \
    model/capture.cpp \
    model/photo.cpp \
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
    gui/imageprovider_qmlwrapper.cpp

HEADERS  += gui/mainwindow.h \
    gui/imageview.h \
    gui/thumbnailmodelitem.h \
    server/imageuploader.h \
    model/capture.h \
    model/photo.h \
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
    gui/imageprovider_qmlwrapper.h

FORMS    += gui/mainwindow.ui

OTHER_FILES += \
    gui/thumbnailView.qml \
    gui/thumbnailNavigator.qml \
    gui/ThumbnailDelegate.qml

RESOURCES += \
    gui/gui.qrc \
    icons.qrc

unix {
    LIBS += -lraw -lgomp
}

win32 {
    INCLUDEPATH += G:\Projects\LibRaw-0.14.4
    LIBS += -LG:\Projects\LibRaw-0.14.4\lib -llibraw
}


































