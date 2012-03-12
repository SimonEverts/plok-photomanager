#include "image.h"

#include <cstring>

#include <QDebug>

Image::Image (void) :
    m_pixels (NULL),
    m_ref (NULL),
    m_size (0,0),
    m_channels (0),
    m_step (0),
    m_depth (0),
    m_owner (false)
{
}

Image::Image(QSize size, unsigned int channels, unsigned int depth) :
    m_size (size),
    m_channels (channels),
    m_depth (depth),
    m_owner (true)
{
    m_step = size.width() * channels * depth/8;

    m_pixels = new unsigned char [size_t(size.height()) * m_step];

    m_ref = new QAtomicInt();
    m_ref->ref();

    qDebug () << "construct image";
}

Image::Image(QSize size, unsigned int channels, unsigned int step, unsigned int depth) :
    m_size (size),
    m_channels (channels),
    m_step (step),
    m_depth (depth),
    m_owner (true)
{
    m_pixels = new unsigned char [size_t(size.height()) * step];

    m_ref = new QAtomicInt();
    m_ref->ref();

    qDebug () << "construct image";
}

Image::Image(unsigned char* pixels, QSize size, unsigned int channels, unsigned int step, unsigned int depth) :
    m_pixels (pixels),
    m_ref (NULL),
    m_size (size),
    m_channels (channels),
    m_step (step),
    m_depth (depth),
    m_owner (false)
{
}

Image::Image (const Image& image) :
    m_size (image.size()),
    m_channels (image.channels()),
    m_step (image.step()),
    m_depth (image.depth()),
    m_owner (true)
{
    m_pixels = image.pixels(); //new unsigned char [m_size.height()  * m_step];
    m_ref = image.m_ref;
    if (m_ref)
        m_ref->ref();
}

Image::Image (QImage image) :
    m_size (image.size()),
    m_step (image.bytesPerLine()),
    m_owner (false)
{    
    // TODO what if image isNull()?

    m_channels = 0;
    m_depth = 0;

    switch (image.format()) {
    case QImage::Format_RGB32: {
        m_channels = 4;
        m_depth = 8;
    }; break;
    case QImage::Format_ARGB32: {
        m_channels = 4;
        m_depth = 8;
    }; break;
    case QImage::Format_RGB888: {
        m_channels = 3;
        m_depth = 8;
    }; break;
    default: {
        qDebug() << "Unknown image format: " << image.format();
        qFatal("unknown image format");
    }
    }

    qDebug () << "COPY construct copy qimage";

    m_pixels = new unsigned char [m_size.height() * m_step];

    memcpy (m_pixels, image.bits(), size_t(image.size().height()) * m_step);

    m_ref = new QAtomicInt();
    m_ref->ref();
}

Image::~Image (void)
{
    if (!m_ref)
        return;

    if (m_pixels && !m_ref->deref())
    {
        qDebug () << "destruct image";

        delete[] m_pixels;
        m_pixels = 0;

        delete m_ref;
    }
}

void Image::clear (void)
{
    if (m_pixels && !m_ref->deref())
    {
        qDebug () << "destruct image";

        delete[] m_pixels;
        delete m_ref;
    }

    m_pixels = 0;
    m_ref = 0;
}

Image& Image::operator= (const Image& image)
{
    if (&image != this)
    {
        if (m_pixels && !m_ref->deref())
        {
            qDebug () << "assign: cleanup image";

            delete[] m_pixels;
            delete m_ref;
        }
    }

    m_size = image.size();
    m_channels = image.channels();
    m_step = image.step();
    m_depth = image.depth();
    m_owner = true;

    m_pixels = image.m_pixels;
    m_ref = image.m_ref;

    if (m_ref)
        m_ref->ref();

    return *this;
}

Image Image::copy (void)
{
    Image image (m_size, m_channels, m_step, m_depth);

    memcpy (image.pixels(), m_pixels, size_t(m_size.height()) * m_step);

    qDebug () << "COPY image";

    return image;
}

QImage Image::toQImage () const
{
    qDebug () << "copy to qimage";

    QImage image;


    if (m_channels == 3)
        image = QImage (m_pixels, m_size.width(), m_size.height(), m_step, QImage::Format_RGB888);

    if (m_channels == 4)
        image = QImage (m_pixels, m_size.width(), m_size.height(), m_step, QImage::Format_RGB32);



    return image.copy();
}

bool Image::isNull (void) const
{
    return (m_pixels == NULL) || (m_size.width() == 0) || (m_size.height() == 0);
}
