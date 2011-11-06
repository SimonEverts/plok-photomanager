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

private slots:
    void on_fileOpenButton_clicked();

    void on_captureListWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

private:
    void loadDirectory (QString dirName);
    void loadDirectoryThumbnails (QString dirName);

    void loadImage (QString fileName);

    Ui::MainWindow *ui;

    QString m_currentPath;
    std::deque <QString> m_captureList;

    QList <QSharedPointer< ThumbnailModelItem > > m_thumbnailModel;

    QGraphicsScene m_scene;
    QGraphicsPixmapItem* m_pixmapItem;
};

#endif // MAINWINDOW_H
