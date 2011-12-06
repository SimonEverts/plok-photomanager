#include "imageloader.h"

#include <QDebug>

#include <libraw/libraw.h>


class ImageLoader_p
{
public:
    void openImage (QString imagePath);

    QImage loadThumbnail (void);
private:
    LibRaw m_rawProcessor;
};

void ImageLoader_p::openImage (QString imagePath)
{
    qDebug() << imagePath.toAscii();
    m_rawProcessor.open_file( imagePath.toAscii() );

    qDebug() << "Image size: " << m_rawProcessor.imgdata.sizes.width << m_rawProcessor.imgdata.sizes.height;
    qDebug() << "Thumbnail size: " << m_rawProcessor.imgdata.thumbnail.twidth << m_rawProcessor.imgdata.thumbnail.theight;
}

QImage ImageLoader_p::loadThumbnail ()
{
    m_rawProcessor.unpack_thumb();

    QSize size (m_rawProcessor.imgdata.thumbnail.twidth, m_rawProcessor.imgdata.thumbnail.theight);

    qDebug () << m_rawProcessor.imgdata.thumbnail.tformat;

    QImage image;
    image.loadFromData(
                reinterpret_cast <unsigned char*> (m_rawProcessor.imgdata.thumbnail.thumb),
                m_rawProcessor.imgdata.thumbnail.tlength,
                "JPG");

    return image;
}


ImageLoader::ImageLoader(QObject *parent) :
    QObject(parent)
{
    p = new ImageLoader_p();
}

ImageLoader::~ImageLoader (void)
{
    delete p;
}

void ImageLoader::openImage (QString imagePath)
{
    p->openImage(imagePath);
}

QImage ImageLoader::loadThumbnail()
{
    return p->loadThumbnail();
}
