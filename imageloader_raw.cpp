#include "imageloader_raw.h"

#include <QDebug>

#include <libraw/libraw.h>

class ImageLoader_raw_p
{
public:
    void openImage (QString imagePath);

    QImage loadThumbnail (void);
private:
    LibRaw m_rawProcessor;
};


void ImageLoader_raw_p::openImage (QString imagePath)
{
    m_rawProcessor.open_file( imagePath.toAscii() );
}

QImage ImageLoader_raw_p::loadThumbnail ()
{
    m_rawProcessor.unpack_thumb();

    QSize size (m_rawProcessor.imgdata.thumbnail.twidth, m_rawProcessor.imgdata.thumbnail.theight);

    QImage image;
    image.loadFromData(
                reinterpret_cast <unsigned char*> (m_rawProcessor.imgdata.thumbnail.thumb),
                m_rawProcessor.imgdata.thumbnail.tlength,
                "JPG");

    return image;
}

ImageLoader_raw::ImageLoader_raw (QObject *parent)
{
    p = new ImageLoader_raw_p();
}

ImageLoader_raw::~ImageLoader_raw()
{
    delete p;
}

void ImageLoader_raw::openImage (QString imagePath)
{
    p->openImage(imagePath);
}

QImage ImageLoader_raw::loadThumbnail()
{
    return p->loadThumbnail();
}
