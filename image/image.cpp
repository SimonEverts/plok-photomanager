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
        qDebug () << "construct null image";
}

Image::Image(QSize size, unsigned int channels, unsigned int depth) :
    m_size (size),
    m_channels (channels),
    m_depth (depth),
    m_owner (true)
{
    m_step = size.width() * depth/8;

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
    qDebug () << "construct shallow image";
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
    m_ref->ref();

    qDebug () << "construct copy image";

    //memcpy (m_pixels, image.pixels(), size_t(image.size().height())  * m_step);
}

Image::Image (QImage image) :
    m_size (image.size()),
    m_step (image.bytesPerLine()),
    m_depth (image.depth()),
    m_owner (false)
{
    if (image.depth())
        m_channels = image.depth()/8;

    qDebug () << "COPY construct copy qimage";

    m_pixels = new unsigned char [m_size.height() * m_step];

    memcpy (m_pixels, image.bits(), size_t(image.size().height()) * m_step);

    m_ref = new QAtomicInt();
    m_ref->ref();

//    unsigned char* pixels = image.bits();

//    for (int y=0; y<m_size.height(); y++)
//    {
//        for (int x=0; x<m_size.width(); x++)
//        {
//            unsigned int index = (y*m_step) + x;

//            m_pixels[index] = pixels[index];
//        }
//    }
}

Image::~Image (void)
{
    if (m_pixels && !m_ref->deref())
    {
        qDebug () << "destruct owner image";

        delete[] m_pixels;
        m_pixels = 0;

        delete m_ref;
    } else
        qDebug () << "destruct image";
}

Image& Image::operator= (const Image& image)
{
    qDebug () << "assign image";

    if (&image != this)
    {
        if (m_pixels && !m_ref->deref())
        {
            delete[] m_pixels;
            delete m_ref;
        }

//        m_pixels = new unsigned char [size_t(image.size().height())  * image.step()];
    }

    m_size = image.size();
    m_channels = image.channels();
    m_step = image.step();
    m_depth = image.depth();
    m_owner = true;

    m_pixels = image.m_pixels;
    m_ref = image.m_ref;
    m_ref->ref();

    //memcpy (m_pixels, image.pixels(), size_t(image.size().height()) * m_step);

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

    if (m_channels == 4)
        image = QImage (m_pixels, m_size.width(), m_size.height(), m_step, QImage::Format_RGB32);

    if (m_channels == 3)
        image = QImage (m_pixels, m_size.width(), m_size.height(), m_step, QImage::Format_RGB888);

    return image.copy();
}

bool Image::isNull (void)
{
    return (m_pixels == NULL) || (m_size.width() == 0) || (m_size.height() == 0);
}
