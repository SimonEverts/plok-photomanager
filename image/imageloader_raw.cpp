#include "imageloader_raw.h"

#include <QDebug>
#include <QDateTime>

#include <libraw/libraw.h>

class ImageLoader_raw_p
{
public:
    void openImage (QString imagePath);

    QImage loadThumbnail (void);
    QImage loadPreview (void);
    QImage loadMaster (void);

    QMap <QString, QVariant> loadInfo (void);
private:
    LibRaw m_rawProcessor;
};


void ImageLoader_raw_p::openImage (QString imagePath)
{
    m_rawProcessor.recycle();

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

QImage ImageLoader_raw_p::loadPreview ()
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

QImage ImageLoader_raw_p::loadMaster ()
{
    m_rawProcessor.unpack();

    // Use camera whitebalance
    m_rawProcessor.imgdata.params.use_camera_wb = 1;

    m_rawProcessor.imgdata.params.no_auto_bright = 1;
//    m_rawProcessor.imgdata.params.bright = 1;

        m_rawProcessor.imgdata.params.gamm[0] = 1.f / 2.2;  // sRGB
        m_rawProcessor.imgdata.params.gamm[1] = 12.92;      // sRGB

    m_rawProcessor.imgdata.params.output_color = 1; // sRGB

    // Use AHC bayer interpolation
    m_rawProcessor.imgdata.params.user_qual = 3;

    m_rawProcessor.imgdata.params.threshold = 100;
    if (m_rawProcessor.imgdata.other.iso_speed)
        m_rawProcessor.imgdata.params.threshold = m_rawProcessor.imgdata.other.iso_speed / 4; // TODO  /8 beter?

//    m_rawProcessor.imgdata.params.med_passes = 1;

    m_rawProcessor.dcraw_process();

    int mem_height = 0;
    int mem_width = 0;
    int mem_channels = 0;
    int mem_bits_per_pixel = 0;

    m_rawProcessor.get_mem_image_format(&mem_width, &mem_height, &mem_channels, &mem_bits_per_pixel);

    QSize size (mem_width, mem_height);

    QImage image;
    if (mem_channels == 3 && mem_bits_per_pixel == 8)
    {
        image = QImage (size, QImage::Format_RGB888);

        m_rawProcessor.copy_mem_image(image.bits(), image.bytesPerLine(), 0);
    }

    return image;
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
    info["lens"] = "todo";

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

QImage ImageLoader_raw::loadPreview()
{
    return p->loadPreview();
}

QImage ImageLoader_raw::loadMaster()
{
    return p->loadMaster();
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
