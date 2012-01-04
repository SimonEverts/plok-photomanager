#include "imageeditor.h"
#include "ui_imageeditor.h"

#include "image/imageprovider.h"
#include "image/imageprocessing.h"

#include <QFileInfo>
#include <QDebug>

ImageEditor::ImageEditor(ImageProvider* imageProvider, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageEditor),
    m_imageProvider (imageProvider)
{
    ui->setupUi(this);

    updateLut();

    connect (ui->brightnessSlider, SIGNAL(valueChanged(int)), this, SLOT(updateLut()));
    connect (ui->contrastSlider, SIGNAL(valueChanged(int)), this, SLOT(updateLut()));
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

    updateLut( );
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

    updateLut ();
}

void ImageEditor::updateHistogram ( QImage image )
{
    Histogram histogram;
    memset (&histogram, 0, sizeof(Histogram));

    ImageProcessing::createHistogram(&image, histogram);

    ui->histogramView->setHistogram( histogram );

    for( int i=0; i<255; i++)
    {
        qDebug() << histogram.red[i] << histogram.green[i] << histogram.blue[i];
    }
}

void ImageEditor::updateLut (void)
{
    Lut lut;
    //memset (&lut, 0, sizeof(Lut));

    float contrast = 1 + float(ui->contrastSlider->value()) / 100;
    int brightness = ui->brightnessSlider->value();

    // Generate lut
    for (int i=0; i<255; i++)
    {
        int value = (i*contrast) + brightness;
        if (value < 0)
            value = 0;
        if (value > 255)
            value = 255;

        lut.red[i] = value;
        lut.green[i] = value;
        lut.blue[i] = value;
    }

    ui->lutView->setLut( lut );

    for( int i=0; i<255; i++)
    {
        qDebug() << lut.red[i] << lut.green[i] << lut.blue[i];
    }

    QImage dest_image (m_workImage.size(), m_workImage.format());

    ImageProcessing::applyLut (&m_workImage, &dest_image, lut);

    ui->imageView->setImage(dest_image);

    updateHistogram( dest_image );
}

void ImageEditor::on_brightnessSlider_valueChanged(int value)
{
//    QImage dest_image (m_workImage.size(), m_workImage.format());

//    ImageProcessing::adjustBrightness (&m_workImage, &dest_image, value);

//    ui->imageView->setImage(dest_image);

//    updateHistogram( dest_image );
}
