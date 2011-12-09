#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include <QDeclarativeImageProvider>
#include <QImage>

class ImageLoader;

class ImageProvider : public QDeclarativeImageProvider
{    
public:
    explicit ImageProvider(QObject *parent = 0);
    virtual ~ImageProvider (void);

    QImage requestImage ( const QString& id, QSize* size, const QSize& requestedSize );


private:
    ImageLoader* m_imageLoader_generic;
    ImageLoader* m_imageLoader_raw;

};

#endif // IMAGEPROVIDER_H
