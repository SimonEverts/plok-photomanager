
INCLUDEPATH += $$PWD

SOURCES += \
    $$PWD//mainwindow.cpp \
    $$PWD/imageview.cpp \
    $$PWD/thumbnailmodelitem.cpp \
    $$PWD/imageprovider_qmlwrapper.cpp \
    $$PWD/setviewmodelitem.cpp \
    $$PWD/createsetdialog.cpp \
    $$PWD/imageeditor.cpp \
    $$PWD/histogramview.cpp \
    $$PWD/lutview.cpp \
    $$PWD/sourcemenu.cpp


HEADERS += \
    $$PWD/mainwindow.h \
    $$PWD/imageview.h \
    $$PWD/thumbnailmodelitem.h \
    $$PWD/imageprovider_qmlwrapper.h \
    $$PWD/setviewmodelitem.h \
    $$PWD/createsetdialog.h \
    $$PWD/imageeditor.h \
    $$PWD/histogramview.h \
    $$PWD/lutview.h \
    $$PWD/sourcemenu.h

FORMS += \
    $$PWD/mainwindow.ui \
    $$PWD/createsetdialog.ui \
    $$PWD/imageeditor.ui

RESOURCES += \
    $$PWD/gui.qrc

OTHER_FILES += \
    $$PWD/thumbnailView.qml \
    $$PWD/thumbnailNavigator.qml \
    $$PWD/ThumbnailDelegate.qml \
    $$PWD/setview.qml \
    $$PWD/ThumbnailDelegate_navigator.qml





