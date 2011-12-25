#include "thumbnailer.h"

#include "imageprovider.h"
#include "miscutils.h"

#include <QFileInfo>
#include <QDir>
#include <QDebug>

Thumbnailer::Thumbnailer( ImageProvider* imageProvider ) :
    m_imageProvider (imageProvider)
{
}

QImage Thumbnailer::getCachedThumbnail (QString baseName, QString thumbnailPath)
{
    QString thumbnail_path = MiscUtils::plokpmDir() + "/thumbnails/" + thumbnailPath + "/";
    QDir thumbnail_dir (thumbnail_path);

    bool dir_exists = thumbnail_dir.exists();

    if (!dir_exists)
        return QImage();

    return QImage (thumbnail_path + baseName);
}

// TODO use captures instead of filenames
void Thumbnailer::generateThumbnails (QStringList photos, QString thumbnailPath)
{
    QString thumbnail_path = MiscUtils::plokpmDir() + "/thumbnails/" + thumbnailPath + "/";
    QDir thumbnail_dir (thumbnail_path);

    bool dir_exists = thumbnail_dir.exists();

    if (!dir_exists)
        dir_exists = thumbnail_dir.mkpath( thumbnail_path );

    if (!dir_exists)
        return;

    for (int i=0; i<photos.size(); i++)
    {
        QFileInfo file_info (photos.at(i));
        if (!file_info.exists())
            continue;

        QImage thumb = m_imageProvider->getThumbnail(photos.at(i));
        thumb.save( thumbnail_path + file_info.baseName() + ".jpg");
    }
}
