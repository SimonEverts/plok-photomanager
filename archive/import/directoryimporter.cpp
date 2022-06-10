#include "directoryimporter.h"

#include "image/imageprovider.h"

#include <QDir>
#include <QFileInfo>

#include <cassert>

DirectoryImporter::DirectoryImporter( ImageProvider* imageProvider ) :
    m_imageProvider (imageProvider)
{
}

QList<Capture> DirectoryImporter::importCapturesFromDir (QString dirName)
{
    assert (m_imageProvider);

    QDir dir (dirName);
    dir.setFilter( QDir::Files );
    dir.setSorting( QDir::Name );

    dir.setNameFilters( m_imageProvider->supportedSuffixes() );

    QList <QFileInfo> file_info_list = dir.entryInfoList();

    QList<Capture> capture_list;

    Capture current_capture;

    QList <QFileInfo>::iterator it = file_info_list.begin();
    while( it != file_info_list.end() )
    {
        QString base_name = it->baseName();
        QString file_path = it->filePath();

        if (!current_capture.name().isEmpty() && base_name != current_capture.name())
        {
            capture_list.push_back (current_capture);

            current_capture.setName("");
            current_capture.clear();
        }

        current_capture.setName( base_name );
        current_capture.addPhoto( file_path );

        it++;
    }

    if (!current_capture.name().isEmpty())
        capture_list.push_back (current_capture);

    return capture_list;
}
