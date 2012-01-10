#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include <QDeclarativeImageProvider>
#include <QImage>

#include "image.h"

class ImageLoader;

class ImageProvider
{
public:
    explicit ImageProvider(QObject *parent = 0);
    virtual ~ImageProvider (void);

    QImage requestThumbnail( const QString& id, QSize* size, const QSize& requestedSize );

    Image loadThumbnail (QString fileName);
    Image loadPreview (QString fileName);
    Image loadMaster (QString fileName);

    ImageLoader* imageLoaderFromFormat (QString format);

    QStringList supportedSuffixes (void);

private:
    QList <ImageLoader*> m_imageLoaders;
};

#endif // IMAGEPROVIDER_H
