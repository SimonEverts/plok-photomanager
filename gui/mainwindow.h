#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Project includes
#include "thumbnailmodelitem.h"

#include "image/imageloader.h"
#include "image/imageprovider.h"
#include "image/thumbnailer.h"

#include "server/imageuploader.h"
#include "server/setcreator.h"

#include "model/capture.h"

#include "database/setdao.h"
#include "database/picturedao.h"
//#include "model/setmanager.h"

#include "database/database.h"
#include "worker/workthread.h"

#include "gui/imageprovider_qmlwrapper.h"
#include "gui/createsetdialog.h"
#include "gui/imageeditor.h"

#include "import/directoryimporter.h"

#include "metadata/exifloader.h"

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

    void blockSignals (bool block);

    QSize sizeHint() const {
        return QSize(1280, 1024);
    }

public slots:
    void updateImage (void);

    void currentSelectionChanged( int currentIndex );

    void currentImageChanged( int currentIndex );

    void currentSetChanged( void );

protected slots:
    void on_fileBrowserTreeView_activated ( const QModelIndex & index );

private slots:
    void loadGUI (void);

    void on_actionUpload_images_triggered( bool checked );

    void on_actionStack_triggered();
    void on_actionDestack_triggered();

    void on_actionDelete_triggered();

    void on_actionCreate_set_triggered();

    void on_actionDelete_set_triggered();

    void on_actionEdit_triggered();

    void on_listWidget_currentRowChanged(int currentRow);

private:

    void selectSourceLibrary (void);
    void selectSourceCamera (void);
    void selectSourceFiles (void);

    void selectPreview (void);
    void selectThumbnails (void);

//    void loadThumbnailsFromDir (QString dirName);
    void loadThumbnailsFromCaptures (void);

    void importCapturesFromDir (QString dirName);

    void loadImage (QString fileName);

    Ui::MainWindow *ui;
    QObject* m_thumbnailNavigator;
    QObject* m_thumbnailView;

    CreateSetDialog m_createSetDialog;
    ImageEditor m_imageEditor;

    QActionGroup m_modeActionGroup;

    QList<QObject*> m_modelList;

    QFileSystemModel* m_fileSystemModel;

    ImageUploader m_imageUploader;
    SetCreator m_setCreator;

    ImageProvider m_imageProvider;
    ImageProvider_qmlwrapper* m_qmlNavImageProvider;
    ImageProvider_qmlwrapper* m_qmlViewImageProvider;

    // TODO rename to imageThumbnailer
    Thumbnailer m_imageThumbnailer;

    DirectoryImporter m_directoryImporter;

    ExifLoader m_exifLoader;

    Capture m_currentCapture;

    //QString m_currentPath;
    //QString m_currentImage;

    Database m_database;

    SetDao m_setDao;
    PictureDao m_pictureDao;

    //    SetManager m_setManager;

    QList <QSharedPointer< ThumbnailModelItem > > m_thumbnailModel;

    QList <Capture> m_captures;

    WorkThread m_workThread;
};

#endif // MAINWINDOW_H
