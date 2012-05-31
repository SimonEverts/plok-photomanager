#ifndef IMAGE_H
#define IMAGE_H

#include <QSize>
#include <QImage>

#include <QAtomicInt>

class Image
{
public:
    explicit Image (void);
    Image(QSize size, unsigned int channels, unsigned int depth);
    Image(QSize size, unsigned int channels, unsigned int step, unsigned int depth);
    Image(unsigned char* pixels, QSize size, unsigned int channels, unsigned int step, unsigned int depth);

    Image (const Image& image);

    virtual ~Image (void);

    Image& operator= (const Image& image);

    Image copy (void);

    QImage toQImage (void) const;
    static Image fromQImage (QImage image);

    void clear (void);
    bool isNull (void) const;

    Image fastScale (const Image& image, QSize minimumSize);

    unsigned char* pixels () const {return m_pixels;}
    QSize size () const {return m_size;}
    unsigned int channels () const {return m_channels;}
    unsigned int step () const {return m_step;}
    unsigned int depth () const {return m_depth;}

private:
    unsigned char* m_pixels;
    QAtomicInt* m_ref;

    QSize m_size;
    unsigned int m_channels;
    unsigned int m_step;
    unsigned int m_depth;
};

#endif // IMAGE_H
