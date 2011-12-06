#include "imageprovider.h"

#include <QFileInfo>
#include <QDebug>

ImageProvider::ImageProvider(QObject *parent) :
    QDeclarativeImageProvider( QDeclarativeImageProvider::Image )
{
}

QImage ImageProvider::requestImage ( const QString& id, QSize* size, const QSize& requestedSize )
{
    QString suffix = QFileInfo (id).suffix();

    QImage thumb;
    if (suffix == "RAW" || suffix == "raw" ||
            suffix == "ARW" || suffix == "arw" ||
            suffix == "CR2" || suffix == "cr2")
    {
        m_imageLoader.openImage( id );
        thumb = m_imageLoader.loadThumbnail();
    } else
        thumb.load(  id );

    *size = thumb.size();

    if (requestedSize.isValid())
    {
        if (requestedSize.width() == 0)
            return thumb.scaledToHeight(requestedSize.height());
        if (requestedSize.height() == 0)
            return thumb.scaledToHeight(requestedSize.width());

        return thumb.scaled( requestedSize, Qt::KeepAspectRatio);
    }
    else
        return thumb;
}

