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
    photo.cpp

HEADERS  += mainwindow.h \
    imageview.h \
    thumbnailmodelitem.h \
    imageuploader.h \
    capture.h \
    photo.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    thumbnailView.qml \
    thumbnailNavigator.qml \
    ThumbnailDelegate.qml

RESOURCES += \
    gui.qrc \
    icons.qrc




















