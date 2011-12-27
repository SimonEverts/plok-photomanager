#include "thumbnailcache.h"

#include <QDir>

#include "miscutils.h"

ThumbnailCache::ThumbnailCache()
{
}

QImage ThumbnailCache::getCachedThumbnail (QString baseName, QString thumbnailPath)
{
    QString thumbnail_path = MiscUtils::plokpmDir() + "/thumbnails/" + thumbnailPath + "/";
    QDir thumbnail_dir (thumbnail_path);

    bool dir_exists = thumbnail_dir.exists();

    if (!dir_exists)
        return QImage();

    return QImage (thumbnail_path + baseName);
}
