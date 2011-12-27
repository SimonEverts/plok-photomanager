#ifndef THUMBNAILCACHE_H
#define THUMBNAILCACHE_H

#include <QString>
#include <QImage>

class ThumbnailCache
{
public:
    ThumbnailCache();

    static QImage getCachedThumbnail (QString fileName, QString thumbnailPath);
};

#endif // THUMBNAILCACHE_H
