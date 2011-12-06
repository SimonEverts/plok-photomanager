#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Project includes
#include "thumbnailmodelitem.h"
#include "imageloader.h"
#include "imageuploader.h"
#include "capture.h"

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
    void currentImageChanged ( int currentIndex );
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

private:
    void loadThumbnailsFromDir (QString dirName);
    void loadThumbnailsFromCaptures (void);

    void importCapturesFromDir (QString dirName);

    void loadImage (QString fileName);
    void loadPreviewImage (QString fileName);

    Ui::MainWindow *ui;
    QObject* m_thumbnailNavigator;
    QObject* m_thumbnailView;

    QActionGroup* m_sourceActionGroup;
    QActionGroup* m_imageViewActionGroup;

    QList<QObject*> m_modelList;

    QFileSystemModel* m_fileSystemModel;

    ImageUploader m_imageUploader;

    QString m_currentPath;
    QString m_currentImage;

    QList <QSharedPointer< ThumbnailModelItem > > m_thumbnailModel;

    QList <Capture> m_captures;
};

#endif // MAINWINDOW_H
