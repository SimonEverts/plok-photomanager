#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "imageview.h"

// Qt includes
#include <QFileDialog>
#include <QImageReader>
#include <QElapsedTimer>
#include <QDebug>
#include <QDeclarativeContext>
#include <QDeclarativeItem>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_currentPath (".")
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_fileOpenButton_clicked()
{
    m_currentPath = QFileDialog::getExistingDirectory(this, "Select image directory");
    ui->fileEdit->setText( m_currentPath );

    loadDirectoryThumbnails( m_currentPath );
}

void MainWindow::loadDirectoryThumbnails (QString dirName)
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

void MainWindow::loadImage (QString fileName)
{
    QElapsedTimer timer;
    timer.start();

    QImageReader image_reader (fileName);
    if (!image_reader.canRead())
        return;

    QSize image_size = image_reader.size();

    int scale = image_size.width() / 800;

    QSize scaled_size( image_size.width() / scale, image_size.height() / scale);

    image_reader.setScaledSize ( scaled_size);

    QImage image = image_reader.read();

    qDebug () << "image loading: " << timer.elapsed();

    ui->imageView->setImage (image);

    qDebug () << "drawing: " << timer.elapsed();
}

void MainWindow::currentIndexChanged (int currentIndex)
{
    QString image_file_name = QUrl (m_thumbnailModel.at( currentIndex )->path()).toString(QUrl::RemoveScheme);
    qDebug() << QUrl(m_thumbnailModel.at( currentIndex)->path()).toString(QUrl::RemoveScheme);

    loadImage (image_file_name);

    ui->mainTabWidget->setCurrentWidget(ui->imageViewPage);
}
