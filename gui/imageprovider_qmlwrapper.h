#ifndef IMAGEPROVIDER_QMLWRAPPER_H
#define IMAGEPROVIDER_QMLWRAPPER_H

#include <QDeclarativeImageProvider>
#include <QImage>

#include "image/imageprovider.h"

class ImageProvider_qmlwrapper : public QDeclarativeImageProvider
{
public:
    explicit ImageProvider_qmlwrapper(QObject *parent = 0);
    virtual ~ImageProvider_qmlwrapper (void);

    void setImageProvider (ImageProvider* imageProvider) {
        m_imageProvider = imageProvider;
    }

    QImage requestImage ( const QString& id, QSize* size, const QSize& requestedSize );

private:
    ImageProvider* m_imageProvider;
};

#endif // IMAGEPROVIDER_QMLWRAPPER_H
