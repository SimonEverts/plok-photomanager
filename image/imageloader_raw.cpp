#include "imageloader_raw.h"

#include <QDebug>
#include <QDateTime>

#include <libraw/libraw.h>

class ImageLoader_raw_p
{
public:
    void openImage (QString imagePath);

    QImage loadThumbnail (void);
    QImage loadImage (void);

    QMap <QString, QVariant> loadInfo (void);
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

    int scale = size.width() / 480;
    if (scale < 1)
        scale = 1;

    QSize scaled_size( size.width() / scale,
                       size.height() / scale);

    return image.scaled (scaled_size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);;
}

QImage ImageLoader_raw_p::loadImage ()
{
    m_rawProcessor.unpack();
    m_rawProcessor.dcraw_process();

    //m_rawProcessor.raw2image();

    // TODO load raw image
    QSize size (m_rawProcessor.imgdata.sizes.iwidth, m_rawProcessor.imgdata.sizes.iheight);

    qDebug() << size;

    QImage image (size, QImage::Format_RGB888);

    unsigned char* dest_image_pixels = reinterpret_cast <unsigned char*> (image.bits());

    unsigned short int* src_image_pixels = reinterpret_cast <unsigned short int*> (m_rawProcessor.imgdata.image);

    int dest_channels = 3;
    int dest_bits_per_pixel = 8;
    int dest_bytes_per_line = size.width() * dest_channels;

    int src_channels = 4;
    int src_bits_per_pixel = 16;
    int src_bytes_per_line = size.width() * src_channels;

    for (int y=0; y<size.height(); y++)
    {
        for (int x=0; x<size.width(); x++)
        {
            unsigned char* dest_pixel = dest_image_pixels + (y*dest_bytes_per_line) + (x * dest_channels);
            unsigned short int* src_pixel = src_image_pixels + (y*src_bytes_per_line) + (x * src_channels);

            dest_pixel[0] = src_pixel[0] >> 8;
            dest_pixel[1] = src_pixel[1] >> 8;
            dest_pixel[2] = src_pixel[2] >> 8;
        }
    }

    return image.copy();
}


QMap <QString, QVariant> ImageLoader_raw_p::loadInfo (void)
{
    QMap <QString, QVariant> info;
    info["model"] = m_rawProcessor.imgdata.idata.model;
    info["make"] = m_rawProcessor.imgdata.idata.make;
    info["aperture"] = m_rawProcessor.imgdata.other.aperture;
    info["artist"] = m_rawProcessor.imgdata.other.artist;
    info["focal_len"] = m_rawProcessor.imgdata.other.focal_len;
    info["iso_speed"] = m_rawProcessor.imgdata.other.iso_speed;
    info["shot_order"] = m_rawProcessor.imgdata.other.shot_order;
    info["shutter"] = m_rawProcessor.imgdata.other.shutter;
    info["timestamp"] = QDateTime::fromTime_t( m_rawProcessor.imgdata.other.timestamp );
    info["black"] = m_rawProcessor.imgdata.color.black;
    info["cam_mul"] = QString::number(m_rawProcessor.imgdata.color.cam_mul[0]) + " " +
            QString::number(m_rawProcessor.imgdata.color.cam_mul[1]) + " " +
            QString::number(m_rawProcessor.imgdata.color.cam_mul[2]) + " " +
            QString::number(m_rawProcessor.imgdata.color.cam_mul[3]);

    return info;
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

QImage ImageLoader_raw::loadImage()
{
    return p->loadImage();
}

QMap <QString, QVariant> ImageLoader_raw::loadInfo (void)
{
    return p->loadInfo();
}

QStringList ImageLoader_raw::supportedFormats (void)
{
    QStringList filter;

    filter << "arw" << "cr2" << "tif";

    return filter;
}
