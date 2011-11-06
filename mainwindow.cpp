#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "imageview.h"

// Qt includes
#include <QFileDialog>
#include <QImageReader>
#include <QElapsedTimer>
#include <QDebug>

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

    QDir dir (m_currentPath);
    dir.setFilter( QDir::Files );

    QList <QFileInfo> file_info_list = dir.entryInfoList();
    QList <QFileInfo>::iterator it;

    for (it = file_info_list.begin(); it != file_info_list.end(); it++)
    {
        QString file_name = it->fileName();

        m_captureList.push_back( file_name );

        ui->captureListWidget->addItem( file_name );
    }
}

void MainWindow::loadImage (QString fileName)
{
    QElapsedTimer timer;
    timer.start();

    QImageReader image_reader (fileName);

    QSize image_size = image_reader.size();

    int scale = image_size.width() / 400;

    QSize scaled_size( image_size.width() / scale, image_size.height() / scale);

    image_reader.setScaledSize ( scaled_size);

    QImage image = image_reader.read();

    qDebug () << "image loading: " << timer.elapsed();

    ui->imageView->setImage (image);

//    m_pixmapItem = m_scene.addPixmap( QPixmap::fromImage( image ));

//    ui->graphicsView->fitInView( static_cast <QGraphicsItem*> (m_pixmapItem), Qt::KeepAspectRatio);

    qDebug () << "drawing: " << timer.elapsed();
}

void MainWindow::on_captureListWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    QString image_file_name = m_currentPath + '/' + current->text();

    loadImage (image_file_name);
}
