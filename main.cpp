#include <QtGui/QApplication>
#include <QDebug>

#include "gui/mainwindow.h"

#include "image/imageprovider.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug() << "Compiled against Qt version: " << QT_VERSION_STR;
    qDebug() << "Using Qt version: " << qVersion();

    MainWindow w;
    w.show();

    return a.exec();
}
