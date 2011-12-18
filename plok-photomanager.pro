#-------------------------------------------------
#
# Project created by QtCreator 2011-11-04T23:11:04
#
#-------------------------------------------------

QT       += core gui declarative network

TARGET = plok-photomanager
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    imageview.cpp \
    thumbnailmodelitem.cpp \
    imageuploader.cpp \
    capture.cpp \
    photo.cpp \
    imageloader.cpp \
    imageprovider.cpp \
    imageloader_raw.cpp \
    imageloader_generic.cpp \
    imageprovider_qml.cpp

HEADERS  += mainwindow.h \
    imageview.h \
    thumbnailmodelitem.h \
    imageuploader.h \
    capture.h \
    photo.h \
    imageloader.h \
    imageprovider.h \
    imageloader_raw.h \
    imageloader_generic.h \
    imageprovider_qml.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    thumbnailView.qml \
    thumbnailNavigator.qml \
    ThumbnailDelegate.qml

RESOURCES += \
    gui.qrc \
    icons.qrc

INCLUDEPATH += G:\Projects\LibRaw-0.14.4
LIBS += -LG:\Projects\LibRaw-0.14.4\lib -llibraw






























