#include "imageloader_raw.h"

#include <QDebug>
#include <QDateTime>

#include <libraw/libraw.h>

class ImageLoader_raw_p
{
public:
    void openImage (QString imagePath);

    Image loadThumbnail (void);
    Image loadPreview (void);
    Image loadMaster (void);

    QMap <QString, QVariant> loadInfo (void);
private:
    LibRaw m_rawProcessor;
};


void ImageLoader_raw_p::openImage (QString imagePath)
{
    m_rawProcessor.recycle();

    m_rawProcessor.open_file( imagePath.toAscii() );
}

Image ImageLoader_raw_p::loadThumbnail ()
{
    m_rawProcessor.unpack_thumb();

    QSize size (m_rawProcessor.imgdata.thumbnail.twidth, m_rawProcessor.imgdata.thumbnail.theight);

    QImage image;
    image.loadFromData(
                reinterpret_cast <unsigned char*> (m_rawProcessor.imgdata.thumbnail.thumb),
                m_rawProcessor.imgdata.thumbnail.tlength,
                "JPG");

    int scale = 1;
    while ((image.size().width() >> scale) > 480)
        scale++;

    QSize scaled_size( size.width() / scale,
                       size.height() / scale);

    return Image (image.scaled (scaled_size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}

Image ImageLoader_raw_p::loadPreview ()
{
    m_rawProcessor.unpack_thumb();

    QSize size (m_rawProcessor.imgdata.thumbnail.twidth, m_rawProcessor.imgdata.thumbnail.theight);

    Image result;
    if (m_rawProcessor.imgdata.thumbnail.thumb &&
            m_rawProcessor.imgdata.thumbnail.twidth)
    {
        QImage image;
        image.loadFromData(
                    reinterpret_cast <unsigned char*> (m_rawProcessor.imgdata.thumbnail.thumb),
                    m_rawProcessor.imgdata.thumbnail.tlength,
                    "JPG");

        result = Image ( image.convertToFormat(QImage::Format_RGB888) );
    }

    return result;
}

Image ImageLoader_raw_p::loadMaster ()
{
    /**
      * Custom raw import:
      * half_size = 1
      * document_mode = 2
      * gamma[0] gamma[1] = 1
      */


    // Skip debayer for now
    m_rawProcessor.imgdata.params.half_size = 0;
    m_rawProcessor.imgdata.params.document_mode = 0;

    m_rawProcessor.imgdata.params.use_camera_wb = 1;
    m_rawProcessor.imgdata.params.use_auto_wb = 0;
    m_rawProcessor.imgdata.params.med_passes = 0;
    m_rawProcessor.imgdata.params.no_auto_bright = 1;

    m_rawProcessor.imgdata.params.output_color = 1; // sRGB
    m_rawProcessor.imgdata.params.output_bps = 16;  // 16 bits

    m_rawProcessor.unpack();

    // Use AHC bayer interpolation
    m_rawProcessor.imgdata.params.user_qual = 1;

    m_rawProcessor.imgdata.params.threshold = 100;
    if (m_rawProcessor.imgdata.other.iso_speed)
        m_rawProcessor.imgdata.params.threshold = m_rawProcessor.imgdata.other.iso_speed / 8; // TODO  /8 beter?

    m_rawProcessor.dcraw_process();

    int mem_height = 0;
    int mem_width = 0;
    int mem_channels = 0;
    int mem_bits_per_pixel = 0;

    m_rawProcessor.get_mem_image_format(&mem_width, &mem_height, &mem_channels, &mem_bits_per_pixel);

    QSize size (mem_width, mem_height);

    qDebug () << "Create rawprocessor image:";
    qDebug () << mem_width << mem_height << mem_channels << mem_bits_per_pixel;

    Image image (size, mem_channels, size.width() * mem_channels * mem_bits_per_pixel, mem_bits_per_pixel);

//    image.setMaxValue( m_rawProcessor.imgdata.color.maximum );

    m_rawProcessor.copy_mem_image(image.pixels(), image.step(), 0);

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

Image ImageLoader_raw::loadThumbnail()
{
    return p->loadThumbnail();
}

Image ImageLoader_raw::loadPreview()
{
    return p->loadPreview();
}

Image ImageLoader_raw::loadMaster()
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
