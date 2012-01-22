#include "imageloader_generic.h"

#include <QImage>
#include <QImageReader>

#include <QElapsedTimer>
#include <QDebug>

class ImageLoader_generic_p
{
public:
    ImageLoader_generic_p() {
        m_random = qrand();
    }

    void openImage (QString imagePath);

    Image loadThumbnail (void);
    Image loadPreview (void);
    Image loadMaster (void);
private:
    QImageReader m_imageReader;

    int m_random;
};


void ImageLoader_generic_p::openImage (QString imagePath)
{
    m_imageReader.setFileName( imagePath );
}

Image ImageLoader_generic_p::loadThumbnail ()
{
    if (!m_imageReader.canRead())
        return Image();

    m_imageReader.setQuality(25);

    QSize image_size = m_imageReader.size();

    int scale = 1;
    while ((image_size.width() >> scale) > 480)
        scale++;

    QSize scaled_size( image_size.width() / scale,
                       image_size.height() / scale);

    m_imageReader.setScaledSize ( scaled_size);

    QImage image = m_imageReader.read();

    return image.convertToFormat(QImage::Format_RGB888);
}

Image ImageLoader_generic_p::loadPreview ()
{
    if (!m_imageReader.canRead())
        return QImage();

    m_imageReader.setQuality(100);

    QSize image_size = m_imageReader.size();

    m_imageReader.setScaledSize ( image_size);

    QImage image = m_imageReader.read();

    return image.convertToFormat(QImage::Format_RGB888);
}

Image ImageLoader_generic_p::loadMaster ()
{
    return loadPreview();
}

ImageLoader_generic::ImageLoader_generic (QObject *parent)
{
    p = new ImageLoader_generic_p();
}

ImageLoader_generic::~ImageLoader_generic()
{
    delete p;
}

void ImageLoader_generic::openImage (QString imagePath)
{
    p->openImage(imagePath);
}

Image ImageLoader_generic::loadThumbnail()
{
    return p->loadThumbnail();
}

Image ImageLoader_generic::loadPreview()
{
    return p->loadPreview();
}

Image ImageLoader_generic::loadMaster()
{
    return p->loadMaster();
}

QMap <QString, QVariant> ImageLoader_generic::loadInfo (void)
{
    return QMap <QString, QVariant> ();
}

QStringList ImageLoader_generic::supportedFormats (void)
{
    QStringList filter;

    QList <QByteArray> supported_formats = QImageReader::supportedImageFormats();

    for (int i=0; i<supported_formats.size(); i++)
        filter << supported_formats.at(i);

    return filter;
}
