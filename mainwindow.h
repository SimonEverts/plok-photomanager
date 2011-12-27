#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Project includes
#include "thumbnailmodelitem.h"
#include "imageloader.h"
#include "imageuploader.h"
#include "capture.h"

#include "imageprovider.h"
#include "thumbnailer.h"

#include "database.h"

#include "workthread.h"

// Qt includes
#include <QMainWindow>
#include <QGraphicsScene>
#include <QString>
#include <QListWidgetItem>
#include <QList>
#include <QActionGroup>

#include <QFileSystemModel>

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
    void updateImage (void);

    void currentSelectionChanged( int currentIndex );

    void doubleClickOnThumbnail( int currentIndex );

protected slots:
    void on_fileBrowserTreeView_activated ( const QModelIndex & index );

private slots:

    void on_actionUpload_images_triggered( bool checked );

    void on_actionCombine_triggered();

    void on_actionSplit_triggered();

    void on_actionThumbnails_triggered();

    void on_actionPreview_triggered();

    void on_actionDelete_triggered();

    void on_actionCreate_set_triggered();

    void on_setBrowser_itemActivated(QListWidgetItem *item);

    void on_actionFiles_triggered();

    void on_actionAlbums_triggered();

private:
    void loadGUI (void);

    void loadThumbnailsFromDir (QString dirName);
    void loadThumbnailsFromCaptures (void);

    void importCapturesFromDir (QString dirName);

    void loadImage (QString fileName);

    Ui::MainWindow *ui;
    QObject* m_thumbnailNavigator;
    QObject* m_thumbnailView;

    QActionGroup* m_sourceActionGroup;
    QActionGroup* m_imageViewActionGroup;

    QList<QObject*> m_modelList;

    QFileSystemModel* m_fileSystemModel;

    ImageUploader m_imageUploader;
    ImageProvider m_imageProvider;

    // TODO rename to imageThumbnailer
    Thumbnailer m_imageThumbnailer;

    QString m_currentPath;
    QString m_currentImage;

    Database m_database;

    QList <QSharedPointer< ThumbnailModelItem > > m_thumbnailModel;

    QList <Capture> m_captures;

    WorkThread m_workThread;
};

#endif // MAINWINDOW_H
