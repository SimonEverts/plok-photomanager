#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include <QDeclarativeImageProvider>
#include <QImage>

class ImageLoader;

class ImageProvider
{
public:
    explicit ImageProvider(QObject *parent = 0);
    virtual ~ImageProvider (void);

    QImage requestThumbnail( const QString& id, QSize* size, const QSize& requestedSize );

    QImage loadThumbnail (QString fileName);
    QImage loadPreview (QString fileName);
    QImage loadMaster (QString fileName);

    ImageLoader* imageLoaderFromFormat (QString format);

    QStringList supportedSuffixes (void);

    void setCurrentImage (QImage image) {
        m_currentImage = image;
    }

private:
    QList <ImageLoader*> m_imageLoaders;

    QImage m_currentImage;
};

#endif // IMAGEPROVIDER_H
