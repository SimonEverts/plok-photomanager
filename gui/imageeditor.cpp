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
    connect (ui->gammaSpinBox, SIGNAL(valueChanged(double)), this, SLOT(updateLut()));

    connect (ui->redSpinBox, SIGNAL(valueChanged(double)), this, SLOT(updateLut()));
    connect (ui->greenSpinBox, SIGNAL(valueChanged(double)), this, SLOT(updateLut()));
    connect (ui->blueSpinBox, SIGNAL(valueChanged(double)), this, SLOT(updateLut()));

    connect (ui->medianCheckBox, SIGNAL(toggled(bool)), this, SLOT(updateLut()));
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
    {
        image = m_imageProvider->loadPreview (m_currentPicture);
        ui->gammaSpinBox->setValue(1);
    }
    if (ui->imageDeveloper->currentIndex() == 1)
    {
        image = m_imageProvider->loadMaster (m_currentPicture);
        ui->gammaSpinBox->setValue(2.2);
    }

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
    {
        image = m_imageProvider->loadPreview (m_currentPicture);
        ui->gammaSpinBox->setValue(1);
    }
    if (currentIndex == 1)
    {
        image = m_imageProvider->loadMaster (m_currentPicture);
        ui->gammaSpinBox->setValue(2.2);
    }

    qDebug () << "assign/scale m_workImage:";

    m_workImage = ImageProcessing::fastScale (image, ui->imageView->size());

    updateLut ();
}

void ImageEditor::updateHistogram ( const Image& image )
{
    Histogram histogram (image.depth());

    ImageProcessing::createHistogram (image, histogram);

    ui->histogramView->setHistogram( histogram );
}

void ImageEditor::updateLut (void)
{
    qDebug () << "updateLut:";

    Lut lut (m_workImage.depth());

    int* red = lut.red();
    int* green = lut.green();
    int* blue = lut.blue();

    if (!red || !green || !blue)
        return;

    float contrast = 1 + float(ui->contrastSlider->value()) / 100;
    float brightness = float(ui->brightnessSlider->value()) / 100;
    float gamma = 1.f / ui->gammaSpinBox->value();

    float wb_red = ui->redSpinBox->value();
    float wb_green = ui->greenSpinBox->value();
    float wb_blue = ui->blueSpinBox->value();

    ImageProcessing::generateLut(brightness, contrast, gamma, wb_red, wb_green, wb_blue, lut);

    ui->lutView->setLut( lut );

    Image dest_image (m_workImage.size(), m_workImage.channels(), m_workImage.size().width() * m_workImage.channels(), 8);

    if (m_workImage.depth() == 16 && ui->medianCheckBox->isChecked())
    {
#ifdef OPENCV
        Image tmp_image1 (m_workImage.size(), m_workImage.channels(), m_workImage.size().width() * m_workImage.channels() * 2, 16);

        ImageProcessing::medianFilter_16u(m_workImage, tmp_image1, 5);

        qDebug () << "applyLut:";

        ImageProcessing::applyLut (&tmp_image1, &dest_image, lut);
#else
        ImageProcessing::applyLut (&m_workImage, &dest_image, lut);
#endif
    } else
    {
        ImageProcessing::applyLut (&m_workImage, &dest_image, lut);
    }

    qDebug () << "updateLut -> imageView->setImage:";

    ui->imageView->setImage(dest_image.toQImage());

    qDebug () << "updateHistogram:";

    updateHistogram( dest_image );
}
