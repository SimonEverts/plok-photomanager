#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Project includes
#include "thumbnailmodelitem.h"

// Qt includes
#include <QMainWindow>
#include <QGraphicsScene>
#include <QString>
#include <QListWidgetItem>
#include <QList>

// stl includes
#include <deque>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void currentIndexChanged (int currentIndex);

private slots:
    void on_fileOpenButton_clicked();

private:
    void loadDirectoryThumbnails (QString dirName);

    void loadImage (QString fileName);

    Ui::MainWindow *ui;

    QString m_currentPath;

    QList <QSharedPointer< ThumbnailModelItem > > m_thumbnailModel;
};

#endif // MAINWINDOW_H
