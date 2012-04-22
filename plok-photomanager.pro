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

SOURCES += main.cpp\
        gui/mainwindow.cpp \
    gui/imageview.cpp \
    gui/thumbnailmodelitem.cpp \
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
    gui/imageprovider_qmlwrapper.cpp \
    model/picture.cpp \
    gui/setviewmodelitem.cpp \
    model/setmanager.cpp \
    model/setdao.cpp \
    gui/createsetdialog.cpp \
    gui/imageeditor.cpp \
    image/imageprocessing.cpp \
    gui/histogramview.cpp \
    gui/lutview.cpp \
    image/image.cpp \
    image/lut.cpp \
    model/picturedao.cpp \
    3rdparty/json.cpp \
    import/directoryimporter.cpp

HEADERS  += gui/mainwindow.h \
    gui/imageview.h \
    gui/thumbnailmodelitem.h \
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
    gui/imageprovider_qmlwrapper.h \
    model/picture.h \
    gui/setviewmodelitem.h \
    model/setmanager.h \
    model/setdao.h \
    gui/createsetdialog.h \
    gui/imageeditor.h \
    image/imageprocessing.h \
    gui/histogramview.h \
    gui/lutview.h \
    image/image.h \
    image/lut.h \
    model/picturedao.h \
    3rdparty/json.h \
    import/directoryimporter.h

FORMS    += gui/mainwindow.ui \
    gui/createsetdialog.ui \
    gui/imageeditor.ui

OTHER_FILES += \
    gui/thumbnailView.qml \
    gui/thumbnailNavigator.qml \
    gui/ThumbnailDelegate.qml \
    gui/setview.qml \
    gui/ThumbnailDelegate_navigator.qml \
    todo.txt

RESOURCES += \
    gui/gui.qrc \
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


































