#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "imageprovider_qmlwrapper.h"

#include "common/miscutils.h"

#include "setviewmodelitem.h"

// Qt includes
#include <QFileDialog>
#include <QImageReader>
#include <QElapsedTimer>
#include <QDebug>
#include <QDateTime>
#include <QDeclarativeContext>
#include <QDeclarativeItem>
#include <QDeclarativeEngine>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_thumbnailNavigator (0),
    m_thumbnailView (0),
    m_imageProvider(),
    m_modeActionGroup (this),
    m_imageThumbnailer (&m_imageProvider),
    m_directoryImporter (&m_imageProvider),
    m_database (),
    m_setDao(&m_database),
    m_pictureDao (&m_database),
    m_createSetDialog( &m_setDao, &m_imageThumbnailer ),
    m_imageEditor (&m_imageProvider, &m_pictureDao, this)
{
    ui->setupUi(this);

    m_thumbnailNavigator = ui->thumbnailNavigator->rootObject()->findChild<QObject*> ("thumbnailNavigator");

    QObject* setView = ui->setView->rootObject()->findChild<QObject*> ("listButtons");
    connect( setView, SIGNAL(createSet()), this, SLOT(on_actionCreate_set_triggered()));
    connect( setView, SIGNAL(deleteSet()), this, SLOT(on_actionDelete_set_triggered()));


    QObject* setViewList = ui->setView->rootObject()->findChild<QObject*> ("setViewList");
    connect( setViewList, SIGNAL(currentIndexChanged()), this, SLOT(currentSetChanged()), Qt::QueuedConnection);

    m_qmlNavImageProvider = new ImageProvider_qmlwrapper();
    m_qmlViewImageProvider = new ImageProvider_qmlwrapper();

    ui->thumbnailNavigator->engine()->addImageProvider(QLatin1String("imageprovider"), m_qmlNavImageProvider);
    ui->thumbnailView->engine()->addImageProvider(QLatin1String("imageprovider"), m_qmlViewImageProvider);

    if (m_thumbnailNavigator)
        connect( m_thumbnailNavigator, SIGNAL(loadNewImage(int)), this, SLOT(currentImageChanged(int)), Qt::QueuedConnection);

    m_fileSystemModel = new QFileSystemModel();
    m_fileSystemModel->setRootPath(QDir::homePath());

    ui->fileBrowserTreeView->setModel( m_fileSystemModel );
    ui->fileBrowserTreeView->setCurrentIndex( m_fileSystemModel->index(QDir::homePath()));
    ui->fileBrowserTreeView->setColumnHidden(1, true);
    ui->fileBrowserTreeView->setColumnHidden(2, true);
    ui->fileBrowserTreeView->setColumnHidden(3, true);

    QDeclarativeContext *nav_context = ui->thumbnailNavigator->rootContext();
    nav_context->setContextProperty("qml_model", m_fileSystemModel);

    ui->sourceStackedWidget->setCurrentWidget(ui->librarySourcePage);
    ui->mainStackedWidget->setCurrentWidget(ui->mainStackedThumbnailPage);

    m_database.initialize();

    connect (&m_createSetDialog, SIGNAL (setCreated()), this, SLOT(loadGUI()));
    loadGUI();

    connect (&m_workThread, SIGNAL(photoLoaded()), this, SLOT(updateImage()), Qt::QueuedConnection);

    m_workThread.start();
}

MainWindow::~MainWindow()
{
    m_workThread.stop();

    delete ui;
    delete m_fileSystemModel;
}

void MainWindow::blockSignals (bool block)
{
    QList<QWidget*> children = findChildren<QWidget*> ();

    for (int i=0; i<children.size(); i++)
    {
        children[i]->blockSignals (block);
    }
    QWidget::blockSignals(block);

    if (m_thumbnailView)
        m_thumbnailView->blockSignals(block);

    if (m_thumbnailNavigator)
        m_thumbnailNavigator->blockSignals(block);

    QObject* image_view = ui->thumbnailNavigator->rootObject()->findChild<QObject*> ("previewImage");
    if (image_view)
        image_view->blockSignals (block);
}

void MainWindow::loadGUI (void)
{
    QList <Set> sets = m_setDao.sets();

    QList<QObject*> dataList;
    for (int i=0; i<sets.size(); i++)
    {
        dataList.append( new SetViewModelItem( sets.at(i).name(), sets.at(i).date(), "" ) );
    }

    QDeclarativeContext *context = ui->setView->rootContext();
    context->setContextProperty("setViewModel", QVariant::fromValue<QList<QObject*> >(dataList));

}

void MainWindow::on_fileBrowserTreeView_activated ( const QModelIndex & index )
{
    QString path = m_fileSystemModel->filePath( index );

    QFileInfo file_info ( path );

    if (file_info.isDir())
    {
        m_captures = m_directoryImporter.importCapturesFromDir( path );
        loadThumbnailsFromCaptures();

        selectThumbnails();
    } else
    {
        m_captures = m_directoryImporter.importCapturesFromDir( file_info.absolutePath() );
        loadThumbnailsFromCaptures();

        for (int i=0; i<m_captures.size(); i++)
        {
            if (m_captures.at(i).name() == QFileInfo(path).baseName())
                m_currentCapture = m_captures.at(i);
        }

        loadImage( path );
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

void MainWindow::loadThumbnailsFromCaptures (void)
{
    QList <Capture>::iterator it;

    m_thumbnailModel.clear();
    m_modelList.clear();

    for (it = m_captures.begin(); it != m_captures.end(); it++)
    {
        QString name = it->name();
        QString path = it->previewPhoto();

        QSharedPointer <ThumbnailModelItem> model_item (new ThumbnailModelItem( name, path, it->photoCount()));

        m_thumbnailModel.push_back( model_item );
        m_modelList.append( &(*model_item) );
    }

    blockSignals( true );

    QDeclarativeContext *context = ui->thumbnailView->rootContext();
    context->setContextProperty("thumbnailViewModel", QVariant::fromValue<QList<QObject*> >(m_modelList));
    context->setContextProperty("mainWindow", this);

    QDeclarativeContext *nav_context = ui->thumbnailNavigator->rootContext();
    nav_context->setContextProperty("thumbnailViewModel", QVariant::fromValue<QList<QObject*> >(m_modelList));
    nav_context->setContextProperty("mainWindow", this);

    blockSignals( false );
}

void MainWindow::loadImage (QString fileName)
{
    QObject* image_view = ui->thumbnailNavigator->rootObject()->findChild<QObject*> ("previewImage");
    QObject* image_frame = ui->thumbnailNavigator->rootObject()->findChild<QObject*> ("imageFrame");

    ImageLoader* image_loader = m_imageProvider.imageLoaderFromFormat( QFileInfo(fileName).suffix() );

    if (!image_loader)
        return;

    image_loader->openImage(fileName);
    QMap <QString, QVariant> info = image_loader->loadInfo();

    if (image_frame)
        image_frame->setProperty("infoMap", info);

    if (image_view)
        image_view->setProperty("source", QString("image://imageprovider/") + fileName);

    m_workThread.loadPictures(QStringList() << fileName);

//    qDebug() << "loadImage: "  << fileName;

//    m_imageUploader.authenticate();

    selectPreview ();
}

void MainWindow::updateImage (void)
{
    QList <Picture> pictures = m_workThread.pictures();

    QObject* image_view = ui->thumbnailNavigator->rootObject()->findChild<QObject*> ("previewImage");
    QObject* image_frame = ui->thumbnailNavigator->rootObject()->findChild<QObject*> ("imageFrame");

    qDebug() << "updateImage: "  << m_currentCapture.name();

    for (int i=0; i<pictures.size(); i++)
    {
        if (pictures.at(i).name() == m_currentCapture.name())
        {
            qDebug() << "updateImage: "  << m_currentCapture.name();

            if (image_view)
            {
                int width = image_view->property("width").toInt();
                int height = image_view->property("height").toInt();
                QImage image = pictures.at(i).image().toQImage().scaled(QSize(width, height), Qt::KeepAspectRatio, Qt::SmoothTransformation);


                if (m_qmlNavImageProvider)
                    m_qmlNavImageProvider->setCurrentImage (image);

                image_view->setProperty("source", QString("image://imageprovider/") + "current");
            }
        }
    }
}


void MainWindow::currentSelectionChanged (int currentIndex)
{
    m_thumbnailModel.at( currentIndex )->toggleSelected();
}

void MainWindow::currentImageChanged( int currentIndex )
{
    if (!m_thumbnailModel.size())
        return;

    if (currentIndex < m_captures.size())
        m_currentCapture = m_captures.at (currentIndex);

//    m_currentPath = ;
//    m_currentImage = m_thumbnailModel.at( currentIndex )->name();

    blockSignals( true );

    if (m_thumbnailView)
        m_thumbnailView->setProperty("currentIndex", currentIndex);

    if (m_thumbnailNavigator)
        m_thumbnailNavigator->setProperty("currentIndex", currentIndex);

    blockSignals (false);

    QString current_path = QUrl (m_thumbnailModel.at( currentIndex )->path()).toString(QUrl::RemoveScheme);
    loadImage ( current_path );
}

void MainWindow::currentSetChanged( void )
{
    QList <Set> sets = m_setDao.sets();

    QObject* setView = ui->setView->rootObject()->findChild<QObject*> ("setViewList");
    int index = setView->property("currentIndex").toInt();

    if (index < sets.size())
    {
        QString path = sets.at(index).path();

        QFileInfo file_info ( path );

        if (file_info.exists())
        {
            m_captures = m_directoryImporter.importCapturesFromDir( path );
            loadThumbnailsFromCaptures();
        }
    }

    selectThumbnails();
}

void MainWindow::on_actionStack_triggered()
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
        new_capture.setName(QFileInfo(selected_items.first()).baseName());

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

void MainWindow::on_actionDestack_triggered()
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

void MainWindow::on_actionDelete_triggered()
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
                    QString file_path = QUrl(*photo_it).toString(QUrl::RemoveScheme);

                    QFile file( file_path );

                    if (file.exists())
                    {
                        QDir dir ( MiscUtils::plokpmDir() + "/trash/");

                        bool dir_exists = dir.exists();

                        if (!dir_exists)
                            dir_exists = dir.mkpath( MiscUtils::plokpmDir() + "/trash/");

                        if (dir_exists)
                        {
                            qDebug() << "moving " << file_path << " to " << dir.absolutePath() + "/" + QFileInfo(file_path).fileName();

                            bool result = file.rename(dir.absolutePath() + "/" + QFileInfo(file_path).fileName() );
                            if (!result)
                                qDebug() << file.errorString();
                        }
                    }
                }
            } else
                it++;
        }

        loadThumbnailsFromCaptures();
    }
}

void MainWindow::on_actionCreate_set_triggered()
{
    qDebug () << "Create set";

    m_setCreator.createSet("");

    m_createSetDialog.show();
}

void MainWindow::on_actionDelete_set_triggered()
{
    QList <Set> sets = m_setDao.sets();

    QObject* setView = ui->setView->rootObject()->findChild<QObject*> ("setViewList");
    int index = setView->property("currentIndex").toInt();

    if (index < sets.size())
    {
        m_setDao.remove(sets.at(index));
    }

    loadGUI();
}

void MainWindow::on_actionEdit_triggered()
{
    m_imageEditor.show();

    m_imageEditor.setCapture( m_currentCapture );
}



void MainWindow::on_listWidget_currentRowChanged(int currentRow)
{
    switch (currentRow)
    {
    case 0: selectSourceLibrary(); break;
    case 1: selectSourceCamera(); break;
    case 2: selectSourceFiles(); break;
    default: {
        qFatal("Unknown source Page!");
    }
    }
}

void MainWindow::selectSourceLibrary (void)
{
    ui->sourceStackedWidget->setCurrentWidget(ui->librarySourcePage);

    selectThumbnails();
}

void MainWindow::selectSourceCamera (void)
{
    selectSourceFiles();
}

void MainWindow::selectSourceFiles (void)
{
    ui->sourceStackedWidget->setCurrentWidget(ui->filesSourcePage);

    selectThumbnails();
}

void MainWindow::selectPreview (void)
{
    ui->mainStackedWidget->setCurrentWidget(ui->mainStackedPreviewPage);
}

void MainWindow::selectThumbnails (void)
{
    ui->mainStackedWidget->setCurrentWidget(ui->mainStackedThumbnailPage);
}
