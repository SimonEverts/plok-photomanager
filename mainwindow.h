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

#include <QNetworkAccessManager>
#include <QHttpMultiPart>
#include <QNetworkReply>

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

    void uploadError (QNetworkReply::NetworkError code);
    void uploadProgress (qint64 bytesSent, qint64 bytesTotal );
    void uploadFinished (void);

private:
    void loadDirectoryThumbnails (QString dirName);

    void loadImage (QString fileName);

    void uploadImage (QString fileName);

    Ui::MainWindow *ui;

    QString m_currentPath;
    QString m_currentImage;

    QList <QSharedPointer< ThumbnailModelItem > > m_thumbnailModel;

    QNetworkAccessManager m_networkAccessManager;
};

#endif // MAINWINDOW_H
