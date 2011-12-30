#include "imageprovider_qmlwrapper.h"

#include "image/imageprovider.h"

ImageProvider_qmlwrapper::ImageProvider_qmlwrapper(QObject *parent) :
    QDeclarativeImageProvider( QDeclarativeImageProvider::Image )
{
}

ImageProvider_qmlwrapper::~ImageProvider_qmlwrapper ()
{
}

QImage ImageProvider_qmlwrapper::requestImage ( const QString& id, QSize* size, const QSize& requestedSize )
{
    QMutexLocker locker (&m_mutex);

    // TODO request thumbnail not to be used for normal images

    QImage image;

    if (id == "current")
        image = m_currentImage;
    else
        image = m_imageProvider.requestThumbnail (id, size, requestedSize);

    return image;
}
