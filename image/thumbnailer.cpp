#include "thumbnailer.h"

#include "imageprovider.h"
#include "common/miscutils.h"

#include <QFileInfo>
#include <QDir>
#include <QDebug>

Thumbnailer::Thumbnailer( ImageProvider* imageProvider ) :
    m_imageProvider (imageProvider)
{
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

        QImage thumb = m_imageProvider->loadThumbnail(photos.at(i));
        thumb.save( thumbnail_path + file_info.baseName() + ".jpg");
    }
}

void Thumbnailer::generateThumbnails (QString path, QString thumbnailPath)
{
    // TODO generate list for now, this will be part of database in the future
    QDir dir (path);
    QStringList dir_entry_list = dir.entryList( m_imageProvider->supportedSuffixes() );
    QStringList absolute_paths;
    for (int i=0; i<dir_entry_list.size(); i++)
        absolute_paths.push_back( dir.absolutePath() + "/" + dir_entry_list.at(i) );

    generateThumbnails( absolute_paths, thumbnailPath);
}
