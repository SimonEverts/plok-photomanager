#ifndef THUMBNAILER_H
#define THUMBNAILER_H

#include <QStringList>
#include <QImage>

class ImageProvider;

class Thumbnailer
{
public:
    Thumbnailer( ImageProvider* imageProvider );

    void generateThumbnails (QStringList photos, QString thumbnailPath);

    void generateThumbnails (QString path, QString thumbnailPath);
private:

    ImageProvider* m_imageProvider;
};

#endif // THUMBNAILER_H
