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

    QImage loadThumbnail (void);
    QImage loadImage (void);
private:
    QImageReader m_imageReader;

    int m_random;
};


void ImageLoader_generic_p::openImage (QString imagePath)
{
    m_imageReader.setFileName( imagePath );
}

QImage ImageLoader_generic_p::loadThumbnail ()
{
    if (!m_imageReader.canRead())
        return QImage();

    m_imageReader.setQuality(25);

    QSize image_size = m_imageReader.size();

    int scale = image_size.width() / 480;
    if (scale < 1)
        scale = 1;

    QSize scaled_size( image_size.width() / scale,
                       image_size.height() / scale);

    m_imageReader.setScaledSize ( scaled_size);

    QImage image = m_imageReader.read();

    return image;
}

QImage ImageLoader_generic_p::loadImage ()
{
    if (!m_imageReader.canRead())
        return QImage();

    m_imageReader.setQuality(100);

    QSize image_size = m_imageReader.size();

    m_imageReader.setScaledSize ( image_size);

    QImage image = m_imageReader.read();

    return image;
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

QImage ImageLoader_generic::loadThumbnail()
{
    return p->loadThumbnail();
}

QImage ImageLoader_generic::loadImage()
{
    return p->loadImage();
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
