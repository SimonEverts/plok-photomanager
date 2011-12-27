#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "imageview.h"

#include "imageprovider_qmlwrapper.h"

#include "miscutils.h"

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
    m_currentPath ("."),
    m_imageProvider(),
    m_imageThumbnailer (&m_imageProvider)
{
    ui->setupUi(this);

    m_imageViewActionGroup = new QActionGroup (this);
    m_imageViewActionGroup->addAction (ui->actionThumbnails);
    m_imageViewActionGroup->addAction (ui->actionPreview);

    m_sourceActionGroup = new QActionGroup (this);
    m_sourceActionGroup->addAction (ui->actionFiles);
    m_sourceActionGroup->addAction (ui->actionAlbums);

    m_thumbnailNavigator = ui->thumbnailNavigator->rootObject()->findChild<QObject*> ("thumbnailNavigator");


    ImageProvider_qmlwrapper* image_provider_qml = new ImageProvider_qmlwrapper(this);
    image_provider_qml->setImageProvider(&m_imageProvider);

    ui->thumbnailNavigator->engine()->addImageProvider(QLatin1String("imageprovider"), image_provider_qml);
    ui->thumbnailView->engine()->addImageProvider(QLatin1String("imageprovider"), image_provider_qml);


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


    ui->navStackedWidget->setCurrentWidget(ui->setBrowserPage);

    m_database.initialize();

    loadGUI();

    connect (&m_workThread, SIGNAL(photoLoaded()), this, SLOT(updateImage()), Qt::QueuedConnection);

    m_workThread.start();
}

MainWindow::~MainWindow()
{
    m_workThread.stop();

    delete ui;

    delete m_imageViewActionGroup;
    delete m_sourceActionGroup;

    delete m_fileSystemModel;
}

void MainWindow::loadGUI (void)
{
    QList <Set> sets = m_database.sets();
    for (int i=0; i<sets.size(); i++)
    {
        ui->setBrowser->addItem(sets.at(i).name());
    }
}

void MainWindow::on_fileBrowserTreeView_activated ( const QModelIndex & index )
{
    QString path = m_fileSystemModel->filePath( index );

    QFileInfo file_info ( path );

    if (file_info.isDir())
    {
        importCapturesFromDir( path );
        loadThumbnailsFromCaptures();
    } else
    {
        if (QFileInfo(path).absolutePath() != QFileInfo(m_currentPath).absolutePath())
        {
            importCapturesFromDir( file_info.absolutePath() );
            loadThumbnailsFromCaptures();
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

void MainWindow::loadThumbnailsFromDir (QString dirName)
{
    QDir dir (dirName);
    dir.setFilter( QDir::Files );
    dir.setNameFilters( m_imageProvider.supportedSuffixes() );

//    qDebug () << (QStringList() << m_imageLoader_generic.supportedFormatFilter() << "*.arw" << "*.ARW" << "*.CR2" << "*.cr2");

    QList <QFileInfo> file_info_list = dir.entryInfoList();
    QList <QFileInfo>::iterator it;

    QList<QObject*> modelList;
    m_thumbnailModel.clear();

    for (it = file_info_list.begin(); it != file_info_list.end(); it++)
    {
        QString file_name = it->fileName();
        QString file_path = it->filePath();

        QSharedPointer <ThumbnailModelItem> model_item (new ThumbnailModelItem( file_name, file_path, 1 ));

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

        qDebug () << it->photoCount();

        QSharedPointer <ThumbnailModelItem> model_item (new ThumbnailModelItem( name, path, it->photoCount()));

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
    //dir.setNameFilters(QStringList() << "*.jpg" << "*.JPG" << "*.arw" << "*.ARW" << "*.CR2" << "*.cr2");

    dir.setNameFilters( m_imageProvider.supportedSuffixes() ); // << "*.arw" << "*.ARW" << "*.CR2" << "*.cr2");

//    qDebug () << (QStringList() << m_imageLoader_generic.supportedFormatFilter() << "*.arw" << "*.ARW" << "*.CR2" << "*.cr2");

    QList <QFileInfo> file_info_list = dir.entryInfoList();

    m_captures.clear();

    Capture current_capture;

    QList <QFileInfo>::iterator it = file_info_list.begin();
    while( it != file_info_list.end() )
    {
        QString base_name = it->baseName();
        QString file_path = it->filePath();

        QDateTime capture_time = it->created();

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

    if (!current_capture.name().isEmpty())
        m_captures.push_back (current_capture);
}

void MainWindow::loadImage (QString fileName)
{
    m_currentPath = fileName;
    m_currentImage = QFileInfo (fileName).baseName();

    QObject* image_view = ui->thumbnailNavigator->rootObject()->findChild<QObject*> ("previewImage");
    QObject* image_frame = ui->thumbnailNavigator->rootObject()->findChild<QObject*> ("imageFrame");

    ImageLoader* image_loader = m_imageProvider.imageLoaderFromFormat( QFileInfo(fileName).suffix() );

    image_loader->openImage(fileName);
    QMap <QString, QVariant> info = image_loader->loadInfo();

    QMap <QString, QVariant>::iterator it;

    // TODO move info loading to seperate class

    QList <QVariant> values = info.values();

    for (it = info.begin(); it != info.end(); it++)
        qDebug() << it.key() << it.value();

    if (image_frame)
        image_frame->setProperty("infoMap", info);

    if (image_view)
        image_view->setProperty("source", QString("image://imageprovider/") + fileName);

    m_workThread.loadPhotos(QStringList() << fileName);

    ui->mainStackedWidget->setCurrentWidget(ui->mainStackedPreviewPage);
}

void MainWindow::updateImage (void)
{
    QList <Photo> photos = m_workThread.photos();

    QObject* image_view = ui->thumbnailNavigator->rootObject()->findChild<QObject*> ("previewImage");
    QObject* image_frame = ui->thumbnailNavigator->rootObject()->findChild<QObject*> ("imageFrame");

    for (int i=0; i<photos.size(); i++)
    {
        if (photos.at(i).name() == m_currentImage)
            m_imageProvider.setCurrentImage (photos.at(i).image());
    }

    if (image_view)
        image_view->setProperty("source", QString("image://imageprovider/") + "current");

    ui->mainStackedWidget->setCurrentWidget(ui->mainStackedPreviewPage);
}

void MainWindow::currentSelectionChanged (int currentIndex)
{
    m_thumbnailModel.at( currentIndex )->toggleSelected();
}

void MainWindow::doubleClickOnThumbnail( int currentIndex )
{
    m_currentPath = QUrl (m_thumbnailModel.at( currentIndex )->path()).toString(QUrl::RemoveScheme);
    m_currentImage = m_thumbnailModel.at( currentIndex )->name();

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
    QDir dir = QFileInfo(m_currentPath).absoluteDir();

    if (!m_currentPath.isEmpty())
    {
        Set set (dir.dirName(), dir.absolutePath());

        m_database.addSet (set);

        QStringList dir_entry_list = dir.entryList( m_imageProvider.supportedSuffixes() );
        QStringList absolute_paths;
        for (int i=0; i<dir_entry_list.size(); i++)
            absolute_paths.push_back( dir.absolutePath() + "/" + dir_entry_list.at(i) );

        // Generate thumnbails
        m_imageThumbnailer.generateThumbnails (absolute_paths, "");
    }
}

void MainWindow::on_setBrowser_itemActivated(QListWidgetItem *item)
{
    QList <Set> sets = m_database.sets();

    int index = ui->setBrowser->currentRow();

    QString path = sets.at(index).path();

    QFileInfo file_info ( path );

    if (file_info.isDir())
    {
        importCapturesFromDir( path );
        loadThumbnailsFromCaptures();
    } else
    {
        if (QFileInfo(path).absolutePath() != QFileInfo(m_currentPath).absolutePath())
        {
            importCapturesFromDir( file_info.absolutePath() );
            loadThumbnailsFromCaptures();
        }

        loadImage( path );
    }

    ui->mainStackedWidget->setCurrentWidget(ui->mainStackedThumbnailPage);
}

void MainWindow::on_actionAlbums_triggered()
{
    ui->navStackedWidget->setCurrentWidget(ui->setBrowserPage);
}

void MainWindow::on_actionFiles_triggered()
{
    ui->navStackedWidget->setCurrentWidget(ui->fileBrowserPage);
}
