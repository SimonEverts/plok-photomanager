#include "imageprovider.h"

#include <QFileInfo>
#include <QDebug>

#include <QElapsedTimer>

#include "imageloader_generic.h"
#include "imageloader_raw.h"

ImageProvider::ImageProvider(QObject *parent)
{
    m_imageLoaders.append( new ImageLoader_generic() );
    m_imageLoaders.append( new ImageLoader_raw() );
}

ImageProvider::~ImageProvider ()
{
    while (m_imageLoaders.size())
    {
        delete m_imageLoaders.first();
        m_imageLoaders.removeFirst();
    }
}

QImage ImageProvider::requestImage ( const QString& id, QSize* size, const QSize& requestedSize )
{
    QString suffix = QFileInfo (id).suffix();

    QImage thumb;
//    if (suffix == "RAW" || suffix == "raw" ||
//            suffix == "ARW" || suffix == "arw" ||
//            suffix == "CR2" || suffix == "cr2")
//    {
//        m_imageLoader_raw->openImage( id );
//        thumb = m_imageLoader_raw->loadThumbnail();
//    } else

    ImageLoader* image_loader = imageLoaderFromFormat( suffix );
    if (image_loader)
    {
        image_loader->openImage( id );
        thumb = image_loader->loadThumbnail();
    }

    *size = thumb.size();

    QImage result_image;

    if (requestedSize.isValid())
    {
        if (requestedSize.width() == 0)
            result_image = thumb.scaledToHeight(requestedSize.height());
        if (requestedSize.height() == 0)
            result_image = thumb.scaledToWidth(requestedSize.width());

        if (requestedSize.width() && requestedSize.height())
            result_image = thumb.scaled( requestedSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
    else
        result_image = thumb;

    return result_image;
}

ImageLoader* ImageProvider::imageLoaderFromFormat (QString format)
{
    ImageLoader* result = NULL;

    for (int i=0; i < m_imageLoaders.size(); i++)
    {
        QStringList supported_formats = m_imageLoaders.at(i)->supportedFormats();
        if (supported_formats.contains( format.toLower() ))
            result = m_imageLoaders.at(i);
    }

    return result;
}

QStringList ImageProvider::supportedSuffixes (void)
{
    QStringList result;

    for (int i=0; i < m_imageLoaders.size(); i++)
    {
        QStringList supported_formats = m_imageLoaders.at(i)->supportedFormats();

        for (int j=0; j < supported_formats.size(); j++)
            result << QString("*.") + supported_formats.at(j);
    }

    return result;
}
