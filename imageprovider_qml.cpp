#include "imageprovider_qml.h"

#include "imageprovider.h"

ImageProvider_qml::ImageProvider_qml(QObject *parent) :
    QDeclarativeImageProvider( QDeclarativeImageProvider::Image )
{
}

ImageProvider_qml::~ImageProvider_qml ()
{
}

QImage ImageProvider_qml::requestImage ( const QString& id, QSize* size, const QSize& requestedSize )
{
    return m_imageProvider.requestThumbnail (id, size, requestedSize);
}
