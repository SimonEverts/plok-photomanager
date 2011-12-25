#include "imageprovider.h"

#include <QFileInfo>
#include <QDebug>

#include <QElapsedTimer>

#include "imageloader_generic.h"
#include "imageloader_raw.h"

#include "thumbnailer.h"

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


// TODO image cache needs a set name
QImage ImageProvider::requestThumbnail ( const QString& id, QSize* size, const QSize& requestedSize )
{
    // Check for cached thumnail
    QString cached_name = QFileInfo (id).baseName();
    QImage thumb = Thumbnailer::getCachedThumbnail(cached_name, "");

    if (thumb.isNull())
    {
        // Load normal thumbnail
        QString suffix = QFileInfo (id).suffix();

        ImageLoader* image_loader = imageLoaderFromFormat( suffix );
        if (image_loader)
        {
            image_loader->openImage( id );
            thumb = image_loader->loadThumbnail();
        }
    } else
        qDebug() << "using cache: " + cached_name;

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

// TODO this is now used for the thumbnail generation, use a seperate function for this, and make a new one using the cached thumbnail?

QImage ImageProvider::getThumbnail (QString fileName)
{
    QString suffix = QFileInfo (fileName).suffix();

    QImage thumb;

    ImageLoader* image_loader = imageLoaderFromFormat( suffix );
    if (image_loader)
    {
        image_loader->openImage( fileName );
        thumb = image_loader->loadThumbnail();
    }

    return thumb;
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
