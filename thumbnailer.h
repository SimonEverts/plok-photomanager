#ifndef THUMBNAILER_H
#define THUMBNAILER_H

#include <QStringList>
#include <QImage>

class ImageProvider;

class Thumbnailer
{
public:
    Thumbnailer( ImageProvider* imageProvider );

    static QImage getCachedThumbnail (QString fileName, QString thumbnailPath);

    void generateThumbnails (QStringList photos, QString thumbnailPath);
private:

    ImageProvider* m_imageProvider;
};

#endif // THUMBNAILER_H
