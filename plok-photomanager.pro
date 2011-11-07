#-------------------------------------------------
#
# Project created by QtCreator 2011-11-04T23:11:04
#
#-------------------------------------------------

QT       += core gui declarative

TARGET = plok-photomanager
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    imageview.cpp \
    thumbnailmodelitem.cpp

HEADERS  += mainwindow.h \
    imageview.h \
    thumbnailmodelitem.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    thumbnailView.qml \
    thumbnailNavigator.qml

RESOURCES += \
    gui.qrc






