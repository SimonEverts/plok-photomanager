#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Project includes
#include "thumbnailmodelitem.h"
#include "imageuploader.h"

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
    void currentImageChanged ( int currentIndex );
    void currentSelectionChanged( int currentIndex );

    void doubleClickOnThumbnail( int currentIndex );

private slots:
    void on_fileOpenButton_clicked();

    void on_actionUpload_images_triggered( bool checked );

private:
    void loadDirectoryThumbnails (QString dirName);

    void loadImage (QString fileName);

    Ui::MainWindow *ui;
    QObject* m_thumbnailNavigator;
    QObject* m_thumbnailView;

    ImageUploader m_imageUploader;

    QString m_currentPath;
    QString m_currentImage;

    QList <QSharedPointer< ThumbnailModelItem > > m_thumbnailModel;
};

#endif // MAINWINDOW_H
