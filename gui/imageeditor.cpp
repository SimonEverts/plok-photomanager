#include "imageeditor.h"
#include "ui_imageeditor.h"

#include "image/imageprovider.h"

#include <QFileInfo>
#include <QDebug>

ImageEditor::ImageEditor(ImageProvider* imageProvider, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageEditor),
    m_imageProvider (imageProvider)
{
    ui->setupUi(this);
}

ImageEditor::~ImageEditor()
{
    delete ui;
}

void ImageEditor::setCapture(Capture capture)
{
    m_capture = capture;

    QStringList pictures = m_capture.photoList();

    // TODO just select raw for now
    for (int i=0; i<pictures.size(); i++)
    {
        if (QFileInfo(pictures.at(i)).suffix() == "ARW" ||
            QFileInfo(pictures.at(i)).suffix() == "arw" ||
            QFileInfo(pictures.at(i)).suffix() == "CR2" ||
            QFileInfo(pictures.at(i)).suffix() == "cr2")
            m_currentPicture = pictures.at(i);
    }

    QImage image;
    if (ui->imageDeveloper->currentIndex() == 0)
        image = m_imageProvider->loadPreview (m_currentPicture);
    if (ui->imageDeveloper->currentIndex() == 1)
        image = m_imageProvider->loadMaster (m_currentPicture);

    ui->imageView->setImage( image );

    m_workImage = ui->imageView->scaledImage();

    updateHistogram();
}

void ImageEditor::on_imageDeveloper_currentIndexChanged(const int &currentIndex)
{
    QImage image;

    if (currentIndex == 0)
        image = m_imageProvider->loadPreview (m_currentPicture);
    if (currentIndex == 1)
        image = m_imageProvider->loadMaster (m_currentPicture);

    ui->imageView->setImage( image );

    m_workImage = ui->imageView->scaledImage();

    updateHistogram();
}

void ImageEditor::updateHistogram ()
{

}
