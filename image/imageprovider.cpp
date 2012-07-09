#include "imageprovider.h"

#include <QFileInfo>
#include <QDebug>

#include <QElapsedTimer>

#include "imageloader_generic.h"
#include "imageloader_raw.h"

#include "metadata/exifloader.h"

#include "thumbnailcache.h"

ImageProvider::ImageProvider(QObject *parent)
{
    m_imageLoaders.append( new ImageLoader_generic() );
    m_imageLoaders.append( new ImageLoader_raw() );

    m_exifLoader = new ExifLoader();
}

ImageProvider::~ImageProvider ()
{
    while (m_imageLoaders.size())
    {
        delete m_imageLoaders.first();
        m_imageLoaders.removeFirst();
    }

    if (m_exifLoader)
        delete m_exifLoader;
    m_exifLoader = NULL;
}


// TODO image cache needs a set name
QImage ImageProvider::requestThumbnail ( const QString& id, QSize* size, const QSize& requestedSize )
{
    QImage thumb;

    // Check for cached thumnail
    QString cached_name = QFileInfo (id).baseName();
    thumb = ThumbnailCache::getCachedThumbnail(cached_name, "");

    if (thumb.isNull())
    {
        // Load normal thumbnail
        QString suffix = QFileInfo (id).suffix();

        ImageLoader* image_loader = imageLoaderFromFormat( suffix );
        if (image_loader)
        {
            image_loader->openImage( id );
            thumb = image_loader->loadThumbnail().toQImage();

            correctOrientation( id, thumb );
        }
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

// TODO this is now used for the thumbnail generation, use a seperate function for this, and make a new one using the cached thumbnail?

Image ImageProvider::loadThumbnail (QString fileName)
{
    QString suffix = QFileInfo (fileName).suffix();

    Image thumb;

    ImageLoader* image_loader = imageLoaderFromFormat( suffix );
    if (image_loader)
    {
        image_loader->openImage( fileName );
        thumb = image_loader->loadThumbnail();

        correctOrientation( fileName, thumb );
    }

    return thumb;
}

Image ImageProvider::loadPreview (QString fileName)
{
    QString suffix = QFileInfo (fileName).suffix();

    Image image;

    ImageLoader* image_loader = imageLoaderFromFormat( suffix );
    if (image_loader)
    {
        image_loader->openImage( fileName );
        image = image_loader->loadPreview();

        correctOrientation( fileName, image );
    }

    return image;
}

Image ImageProvider::loadMaster (QString fileName)
{
    QString suffix = QFileInfo (fileName).suffix();

    Image image;

    ImageLoader* image_loader = imageLoaderFromFormat( suffix );
    if (image_loader)
    {
        image_loader->openImage( fileName );
        image = image_loader->loadMaster();

        correctOrientation( fileName, image );
    }

    return image;
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

// TODO: do not load exif twice
void ImageProvider::correctOrientation (QString fileName, QImage &thumb)
{
    QMap <QString, QVariant> info = m_exifLoader->loadInfo( fileName );
    QString orientation = info["orientation"].toString();

    if (orientation == "8")
    {
        QTransform transform;
        transform.rotate( 270 );
        thumb = thumb.transformed( transform );
    }
    if (orientation == "6")
    {
        QTransform transform;
        transform.rotate( 90 );
        thumb = thumb.transformed( transform );
    }
    if (orientation == "3")
    {
        QTransform transform;
        transform.rotate( 180 );
        thumb = thumb.transformed( transform );
    }
}


// TODO: very inefficient!!
void ImageProvider::correctOrientation (QString fileName, Image &thumb)
{
    QImage temp_image = thumb.toQImage();
    correctOrientation( fileName, temp_image);

    thumb = Image::fromQImage( temp_image );
}
