#include "imageeditor.h"
#include "ui_imageeditor.h"

#include "image/imageprovider.h"
#include "image/imageprocessing.h"

#include <QFileInfo>
#include <QDebug>

#include <cmath>

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

    if (!pictures.size())
        return;

    // TODO just select raw for now
    for (int i=0; i<pictures.size(); i++)
    {
        if (QFileInfo(pictures.at(i)).suffix() == "ARW" ||
            QFileInfo(pictures.at(i)).suffix() == "arw" ||
            QFileInfo(pictures.at(i)).suffix() == "CR2" ||
            QFileInfo(pictures.at(i)).suffix() == "cr2")
            m_currentPicture = pictures.at(i);
    }

    qDebug () << "setCapture:";

    Image image;
    if (ui->imageDeveloper->currentIndex() == 0)
        image = m_imageProvider->loadPreview (m_currentPicture);
    if (ui->imageDeveloper->currentIndex() == 1)
        image = m_imageProvider->loadMaster (m_currentPicture);

    qDebug () << "assign/scale m_workImage:";
    m_workImage = ImageProcessing::fastScale (image, ui->imageView->size());

    updateLut( );
}

void ImageEditor::on_imageDeveloper_currentIndexChanged(const int &currentIndex)
{
    qDebug () << "currentIndexChanged:";


    Image image;

    qDebug () << "imageProvider:";

    if (currentIndex == 0)
        image = m_imageProvider->loadPreview (m_currentPicture);
    if (currentIndex == 1)
        image = m_imageProvider->loadMaster (m_currentPicture);

    qDebug () << "assign/scale m_workImage:";

    m_workImage = ImageProcessing::fastScale (image, ui->imageView->size());

    updateLut ();
}

void ImageEditor::updateHistogram ( Image image )
{
    Histogram histogram;
    memset (&histogram, 0, sizeof(Histogram));

    if (image.depth() == 24 || m_workImage.depth() == 32)
        ImageProcessing::createHistogram_8u(&image, histogram);
    if (image.depth() == 48)
        ImageProcessing::createHistogram_16u(&image, histogram);

    ui->histogramView->setHistogram( histogram );
}

void ImageEditor::updateLut (void)
{
    qDebug () << "updateLut:";

    Lut lut;
    memset (&lut, 0, sizeof(Lut));

    if (m_workImage.depth() == 24 || m_workImage.depth() == 32)
    {
        float contrast = 1 + float(ui->contrastSlider->value()) / 100;
        int brightness = 0x100 * (float(ui->brightnessSlider->value()) / 100);

        for (int i=0; i<0x100; i++)
        {
            float gamma = 255.f * (pow (1.055 * (float(i)/255), 1/2.2) - 0.055);

            int value = (contrast*gamma + brightness);

            if (value < 0)
                value = 0;
            if (value > 0xFF)
                value = 0xFF;

            lut.red[i] = value;
            lut.green[i] = value;
            lut.blue[i] = value;
        }
    }

    if (m_workImage.depth() == 48)
    {
        float contrast = 1 + float(ui->contrastSlider->value()) / 100;
        int brightness = 0x10000 * (float(ui->brightnessSlider->value()) / 100);

        // Generate lut
        for (int i=0; i<0x10000; i++)
        {
            float gamma = 65535.f * (pow (1.055 * (float(i)/65535), 1/2.2) - 0.055);

            int value = (contrast*gamma + brightness);

            if (value < 0)
                value = 0;
            if (value > 0xFFFF)
                value = 0xFFFF;

            lut.red[i] = value >> 8;
            lut.green[i] = value >> 8;
            lut.blue[i] = value >> 8;
        }
    }

    ui->lutView->setLut( lut );


    Image dest_image (m_workImage.size(), m_workImage.channels(), m_workImage.size().width() * m_workImage.channels(), 24);

    qDebug () << "applyLut:";

    if (m_workImage.depth() == 24 || m_workImage.depth() == 32)
        ImageProcessing::applyLut_8u (&m_workImage, &dest_image, lut);
    if (m_workImage.depth() == 48)
        ImageProcessing::applyLut_16u (&m_workImage, &dest_image, lut);

    qDebug () << "updateLut -> imageView->setImage:";

    ui->imageView->setImage(dest_image.toQImage());

    qDebug () << "updateHistogram:";

    updateHistogram( dest_image );
}
