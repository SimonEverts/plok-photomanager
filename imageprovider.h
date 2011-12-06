#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include <QDeclarativeImageProvider>
#include <QImage>

#include "imageloader.h"

class ImageProvider : public QDeclarativeImageProvider
{    
public:
    explicit ImageProvider(QObject *parent = 0);

    QImage requestImage ( const QString& id, QSize* size, const QSize& requestedSize );

private:
    ImageLoader m_imageLoader;

};

#endif // IMAGEPROVIDER_H
