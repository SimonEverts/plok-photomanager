#include "imageeditor.h"
#include "ui_imageeditor.h"

#include "image/imageprovider.h"
#include "image/imageprocessing.h"

#include "model/picturedao.h"

#include <QFileInfo>
#include <QDebug>

#include <cassert>

ImageEditor::ImageEditor(ImageProvider* imageProvider, PictureDao* pictureDao, QWidget *parent) :
    QWidget(parent, Qt::Tool),
    ui(new Ui::ImageEditor),
    m_imageProvider (imageProvider),
    m_pictureDao (pictureDao)
{
    ui->setupUi(this);

    connect (ui->brightnessSlider, SIGNAL(valueChanged(int)), this, SLOT(guiChanged()));
    connect (ui->contrastSlider, SIGNAL(valueChanged(int)), this, SLOT(guiChanged()));
    connect (ui->gammaSpinBox, SIGNAL(valueChanged(double)), this, SLOT(guiChanged()));

    connect (ui->redSpinBox, SIGNAL(valueChanged(double)), this, SLOT(guiChanged()));
    connect (ui->greenSpinBox, SIGNAL(valueChanged(double)), this, SLOT(guiChanged()));
    connect (ui->blueSpinBox, SIGNAL(valueChanged(double)), this, SLOT(guiChanged()));
}

ImageEditor::~ImageEditor()
{
    delete ui;
}

void ImageEditor::blockSignals (bool block)
{
    QList<QWidget*> children = findChildren<QWidget*> ();

    for (int i=0; i<children.size(); i++)
    {
        children[i]->blockSignals (block);
    }
    QWidget::blockSignals(block);
}

void ImageEditor::setCapture(Capture capture)
{
    m_capture = capture;

    QStringList photo_list = m_capture.photoList();
    for (int i=0; i<photo_list.size(); i++)
        photo_list[i] = QFileInfo( photo_list[i] ).fileName();

    blockSignals(true);

    ui->masterSelection->clear();
    ui->masterSelection->addItems( photo_list );

    blockSignals(false);

    m_workImage.clear();

    if (m_capture.master().path().isEmpty())
        m_capture.selectMaster();

    loadPicture ();
    updateLut ();
}

void ImageEditor::on_masterSelection_currentIndexChanged(const int &currentIndex)
{   
    QStringList photo_list = m_capture.photoList();

    if (currentIndex < photo_list.size() && currentIndex != -1)
    {
        m_capture.selectMaster( photo_list.at(currentIndex) );

        m_workImage.clear();

        loadPicture ();
        updateLut ();
    }
}

void ImageEditor::updateHistogram ( const Image& image )
{
    Histogram histogram (image.depth());

    ImageProcessing::createHistogram (image, histogram);

    ui->histogramView->setHistogram( histogram );
}

void ImageEditor::updateLut (void)
{
    assert (!m_workImage.isNull());

    PictureProperties& picture_properties = m_capture.master().pictureProperties();

    Lut lut (m_workImage.depth());

    int* red = lut.red();
    int* green = lut.green();
    int* blue = lut.blue();

    if (!red || !green || !blue)
        return;

    float contrast = picture_properties.contrast;
    float brightness = picture_properties.brightness;
    float gamma = picture_properties.gamma;

    float wb_red = picture_properties.wb_red;
    float wb_green = picture_properties.wb_green;
    float wb_blue = picture_properties.wb_blue;

    Image dest_image (m_workImage.size(), 3, 8);
    ImageProcessing::generateLut(brightness, contrast, gamma, wb_red, wb_green, wb_blue, lut);
    ui->lutView->setLut( lut );

    ImageProcessing::applyLut (&m_workImage, &dest_image, lut);

    ui->imageView->setImage(dest_image.toQImage());

    updateHistogram( dest_image );
}

void ImageEditor::loadPicture (void)
{
    Picture& picture = m_capture.master();

    if (!picture.loaded())
    {
        Picture new_picture;
        new_picture = m_pictureDao->read( picture.path() );
        picture.setPictureProperties (new_picture.pictureProperties());

        Image image = m_imageProvider->loadMaster( picture.path() );
        picture.setImage( image );
    }

    if (m_workImage.isNull() && !picture.image().isNull())
         m_workImage = ImageProcessing::fastScale (picture.image(), QSize(800,600));

    loadGUI_pictureProperties (picture.pictureProperties());
}

void ImageEditor::guiChanged (void)
{
    PictureProperties& picture_adjustment = m_capture.master().pictureProperties();

    picture_adjustment.brightness = float(ui->brightnessSlider->value()) / 100;
    picture_adjustment.contrast = 1.f + (float(ui->contrastSlider->value()) / 10);
    picture_adjustment.gamma = 1.f / float(ui->gammaSpinBox->value());

    picture_adjustment.wb_red = ui->redSpinBox->value();
    picture_adjustment.wb_green = ui->greenSpinBox->value();
    picture_adjustment.wb_blue = ui->blueSpinBox->value();

    updateLut();
}

void ImageEditor::loadGUI_pictureProperties (PictureProperties pictureAdjustments)
{
    blockSignals(true);

    float brightness = pictureAdjustments.brightness * 100;
    float contrast = (pictureAdjustments.contrast - 1.f) * 10;

    ui->brightnessSlider->setValue( brightness );
    ui->brightnessSpinBox->setValue( brightness );
    ui->contrastSlider->setValue( contrast );
    ui->contrastSpinBox->setValue( contrast );

    ui->gammaSpinBox->setValue( 1.f / float(pictureAdjustments.gamma) );

    ui->redSpinBox->setValue( pictureAdjustments.wb_red );
    ui->greenSpinBox->setValue( pictureAdjustments.wb_green );
    ui->blueSpinBox->setValue( pictureAdjustments.wb_blue );

    blockSignals (false);
}

void ImageEditor::on_saveButton_clicked()
{
    Picture& picture = m_capture.master();

    int id = m_pictureDao->read(picture.path()).id();
    if ( id != -1  )
    {
        picture.setId (id);
        m_pictureDao->update( picture );
    } else
        m_pictureDao->create ( picture );
}

void ImageEditor::on_previewButton_pressed()
{
    Picture& picture = m_capture.preview();

    Image image;
    if (!picture.loaded())
    {
        image = m_imageProvider->loadPreview ( m_capture.master().path() );
        picture.setImage( image );
    } else
        image = picture.image();

    ui->imageView->setImage( image.toQImage() );
}

void ImageEditor::on_previewButton_released()
{
    loadPicture ();
    updateLut ();
}
