#include "imageprovider_qmlwrapper.h"

#include "imageprovider.h"

ImageProvider_qmlwrapper::ImageProvider_qmlwrapper(QObject *parent) :
    QDeclarativeImageProvider( QDeclarativeImageProvider::Image )
{
}

ImageProvider_qmlwrapper::~ImageProvider_qmlwrapper ()
{
}

QImage ImageProvider_qmlwrapper::requestImage ( const QString& id, QSize* size, const QSize& requestedSize )
{
    return m_imageProvider->requestThumbnail (id, size, requestedSize);
}
