#include "imageloader_generic.h"

#include <QImage>
#include <QImageReader>

class ImageLoader_generic_p
{
public:
    void openImage (QString imagePath);

    QImage loadThumbnail (void);
private:
    QImageReader m_imageReader;
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

    int scale = image_size.width() / 800;

    QSize scaled_size( image_size.width() / scale,
                       image_size.height() / scale);

    m_imageReader.setScaledSize ( scaled_size);

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

QMap <QString, QVariant> ImageLoader_generic::loadInfo (void)
{
    return QMap <QString, QVariant> ();
}
