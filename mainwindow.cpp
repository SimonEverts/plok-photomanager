#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "imageview.h"

// Qt includes
#include <QFileDialog>
#include <QImageReader>
#include <QElapsedTimer>
#include <QDebug>
#include <QDateTime>
#include <QDeclarativeContext>
#include <QDeclarativeItem>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_thumbnailNavigator (0),
    m_thumbnailView (0),
    m_currentPath (".")
{
    ui->setupUi(this);

//    m_thumbnailView = ui->thumbnailView->rootObject()->findChild<QObject*> ("thumbnailView");
//    if (m_thumbnailView)
//        connect( thumbnailView, SIGNAL(loadNewImage(int)), this, SLOT(currentImageChanged(int)));

    m_thumbnailNavigator = ui->thumbnailNavigator->rootObject()->findChild<QObject*> ("thumbnailNavigator");
    if (m_thumbnailNavigator)
        connect( m_thumbnailNavigator, SIGNAL(loadNewImage(int)), this, SLOT(currentImageChanged(int)), Qt::QueuedConnection);

    m_fileSystemModel = new QFileSystemModel();
    m_fileSystemModel->setRootPath(QDir::currentPath());

    ui->fileBrowserTreeView->setModel( m_fileSystemModel );
    ui->fileBrowserTreeView->setColumnHidden(1, true);
    ui->fileBrowserTreeView->setColumnHidden(2, true);
    ui->fileBrowserTreeView->setColumnHidden(3, true);
}

MainWindow::~MainWindow()
{
    delete ui;

    delete m_fileSystemModel;
}

//void MainWindow::on_fileOpenButton_clicked()
//{
//    m_currentPath = QFileDialog::getExistingDirectory(this, "Select image directory");
//    ui->fileEdit->setText( m_currentPath );

//    importCapturesFromDir( m_currentPath );
//    loadThumbnailsFromCaptures();
//}

void MainWindow::on_fileBrowserTreeView_clicked( const QModelIndex& index )
{
    QString path = m_fileSystemModel->filePath( index );

    QFileInfo file_info ( path );

    if (file_info.isDir())
    {
        importCapturesFromDir( path );
        loadThumbnailsFromCaptures();
    } else
    {
        loadImage( path );

        importCapturesFromDir( file_info.absolutePath() );
        loadThumbnailsFromCaptures();
    }
}

void MainWindow::on_actionUpload_images_triggered( bool checked )
{
    qDebug () << "Upload image";

    QList <QSharedPointer< ThumbnailModelItem > >::iterator it;
    for (it = m_thumbnailModel.begin(); it != m_thumbnailModel.end(); it++)
    {
        if ((*it)->selected())
        {
            qDebug () << "Uploading: " << (*it)->path();

            m_imageUploader.uploadImage( (*it)->path() );
        }
    }
}

void MainWindow::loadThumbnailsFromDir (QString dirName)
{
    QDir dir (dirName);
    dir.setFilter( QDir::Files );
    dir.setNameFilters(QStringList() << "*.jpg");

    QList <QFileInfo> file_info_list = dir.entryInfoList();
    QList <QFileInfo>::iterator it;

    QList<QObject*> modelList;
    m_thumbnailModel.clear();

    for (it = file_info_list.begin(); it != file_info_list.end(); it++)
    {
        QString file_name = it->fileName();
        QString file_path = QString ("file://") + it->filePath();

        QSharedPointer <ThumbnailModelItem> model_item (new ThumbnailModelItem( file_name, file_path ));

        m_thumbnailModel.push_back( model_item );
        modelList.append( &(*model_item) );
    }

    QDeclarativeContext *context = ui->thumbnailView->rootContext();
    context->setContextProperty("thumbnailViewModel", QVariant::fromValue<QList<QObject*> >(modelList));
    context->setContextProperty("mainWindow", this);

    QDeclarativeContext *nav_context = ui->thumbnailNavigator->rootContext();
    nav_context->setContextProperty("thumbnailViewModel", QVariant::fromValue<QList<QObject*> >(modelList));
    nav_context->setContextProperty("mainWindow", this);
}

void MainWindow::loadThumbnailsFromCaptures (void)
{
    QList <Capture>::iterator it;

    m_thumbnailModel.clear();
    m_modelList.clear();

    for (it = m_captures.begin(); it != m_captures.end(); it++)
    {
        QString name = it->name();
        QString path = it->previewPhoto();

        QSharedPointer <ThumbnailModelItem> model_item (new ThumbnailModelItem( name, path ));

        m_thumbnailModel.push_back( model_item );
        m_modelList.append( &(*model_item) );
    }

    QDeclarativeContext *context = ui->thumbnailView->rootContext();
    context->setContextProperty("thumbnailViewModel", QVariant::fromValue<QList<QObject*> >(m_modelList));
    context->setContextProperty("mainWindow", this);

    QDeclarativeContext *nav_context = ui->thumbnailNavigator->rootContext();
    nav_context->setContextProperty("thumbnailViewModel", QVariant::fromValue<QList<QObject*> >(m_modelList));
    nav_context->setContextProperty("mainWindow", this);
}

void MainWindow::importCapturesFromDir (QString dirName)
{
    QDir dir (dirName);
    dir.setFilter( QDir::Files );
    dir.setSorting( QDir::Name );
    dir.setNameFilters(QStringList() << "*.jpg" << "*.JPG" << "*.arw" << "*.ARW");

    QList <QFileInfo> file_info_list = dir.entryInfoList();

//    QSharedPointer <Capture> current_capture = QSharedPointer <Capture> ( new Capture() );

    m_captures.clear();

    Capture current_capture;

    QList <QFileInfo>::iterator it = file_info_list.begin();
    while( it != file_info_list.end() )
    {
        QString base_name = it->baseName();
        QString file_path = QString ("file://") + it->filePath();

        QDateTime capture_time = it->created();

        //qDebug() << file_path << capture_time;

        //int capture_time_diff = abs( capture_time.secsTo( current_capture.captureTime() ));

        if (!current_capture.name().isEmpty() && base_name != current_capture.name())
        {
            m_captures.push_back (current_capture);

            current_capture.setName("");
            current_capture.clear();
        }

        current_capture.setName( base_name );
        current_capture.addPhoto( file_path );
        current_capture.setCaptureTime( capture_time );

        it++;
    }

}

void MainWindow::loadImage (QString fileName)
{
//    QElapsedTimer timer;
//    timer.start();

//    QImageReader image_reader (fileName);
//    if (!image_reader.canRead())
//        return;

//    m_currentImage = fileName;

//    QImage image = image_reader.read();

//    qDebug () << "image loading: " << timer.elapsed();

//    //ui->imageView->setImage (image);

//    QObject* image_view = ui->thumbnailNavigator->rootObject()->findChild<QObject*> ("previewImage");

//    if (image_view)
//        image_view->setProperty("source", QString("file:") + fileName);

//    qDebug () << "drawing: " << timer.elapsed();
}

void MainWindow::loadPreviewImage (QString fileName)
{
//    QElapsedTimer timer;
//    timer.start();

//    QImageReader image_reader (fileName);
//    if (!image_reader.canRead())
//        return;

//    image_reader.setQuality(25);

//    m_currentImage = fileName;

//    QSize image_size = image_reader.size();

//    int scale = image_size.width() / 800;

//    QSize scaled_size( image_size.width() / scale, image_size.height() / scale);

//    image_reader.setScaledSize ( scaled_size);

//    QImage image = image_reader.read();

//    qDebug () << "image loading: " << timer.elapsed();

//    ui->imageView->setImage (image);

//    qDebug () << "drawing: " << timer.elapsed();
}

void MainWindow::currentImageChanged (int currentIndex)
{
//    if (currentIndex < m_thumbnailModel.size() && m_thumbnailModel.size())
//    {
//        m_currentPath = QUrl (m_thumbnailModel.at( currentIndex )->path()).toString(QUrl::RemoveScheme);
//        m_currentImage = m_thumbnailModel.at( currentIndex )->name();

//        qDebug() << m_currentPath;

////        ui->mainTabWidget->setCurrentWidget(ui->imageViewPage);

//        loadImage (m_currentPath);
//    }
}

void MainWindow::currentSelectionChanged (int currentIndex)
{
    m_thumbnailModel.at( currentIndex )->toggleSelected();
}

void MainWindow::doubleClickOnThumbnail( int currentIndex )
{
    m_currentPath = QUrl (m_thumbnailModel.at( currentIndex )->path()).toString(QUrl::RemoveScheme);
    m_currentImage = m_thumbnailModel.at( currentIndex )->name();

    qDebug() << m_currentPath;

    if (m_thumbnailView)
        m_thumbnailView->setProperty("currentIndex", currentIndex);

    if (m_thumbnailNavigator)
        m_thumbnailNavigator->setProperty("currentIndex", currentIndex);

    ui->mainStackedWidget->setCurrentWidget(ui->mainStackedPreviewPage);

    loadImage (m_currentPath);
}



void MainWindow::on_actionCombine_triggered()
{
    QStringList selected_items;

    QList <QSharedPointer< ThumbnailModelItem > >::iterator thumb_it;
    for (thumb_it = m_thumbnailModel.begin(); thumb_it != m_thumbnailModel.end(); thumb_it++)
    {
        if ((*thumb_it)->selected())
            selected_items << (*thumb_it)->name();
    }

    Capture new_capture;
    QList <QString> new_photo_list;

    if (selected_items.size())
    {
        new_capture.setName(selected_items.first());

        QList <Capture>::iterator after_first_match;

        QList <Capture>::iterator it = m_captures.begin();
        while( it != m_captures.end() )
        {
            if (selected_items.contains(it->name()))
            {
                QList <QString> photo_list = it->photoList();

                new_photo_list.append( photo_list );

                it = m_captures.erase(it);
                after_first_match = it;
            } else
                it++;
        }

        new_capture.setPhotos( new_photo_list );
        m_captures.insert(after_first_match, new_capture);

        loadThumbnailsFromCaptures();
    }
}

void MainWindow::on_actionSplit_triggered()
{
    QStringList selected_items;

    QList <QSharedPointer< ThumbnailModelItem > >::iterator thumb_it;
    for (thumb_it = m_thumbnailModel.begin(); thumb_it != m_thumbnailModel.end(); thumb_it++)
    {
        if ((*thumb_it)->selected())
            selected_items << (*thumb_it)->name();
    }

    if (selected_items.size())
    {
//        new_capture.setName(selected_items.first());

//        QList <Capture>::iterator after_first_match;

        QList <Capture>::iterator it = m_captures.begin();
        while( it != m_captures.end() )
        {
            if (selected_items.contains(it->name()))
            {
                QList <QString> photo_list = it->photoList();
                it = m_captures.erase(it);

                QList <QString>::iterator photo_it;
                for (photo_it = photo_list.begin(); photo_it != photo_list.end(); photo_it++)
                {
                    Capture new_capture;
                    new_capture.setName( QFileInfo(*photo_it).fileName() );

                    QList <QString> new_list;
                    new_list.push_back( *photo_it );
                    new_capture.setPhotos( new_list );

                    it = m_captures.insert( it, new_capture );
                }

            } else
                it++;
        }

        loadThumbnailsFromCaptures();
    }
}

void MainWindow::on_actionThumbnails_triggered()
{
    ui->mainStackedWidget->setCurrentWidget(ui->mainStackedThumbnailPage);
}

void MainWindow::on_actionPreview_triggered()
{
    ui->mainStackedWidget->setCurrentWidget(ui->mainStackedPreviewPage);
}
