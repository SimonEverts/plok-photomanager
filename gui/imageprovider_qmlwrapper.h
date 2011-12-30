#ifndef IMAGEPROVIDER_QMLWRAPPER_H
#define IMAGEPROVIDER_QMLWRAPPER_H

#include <QDeclarativeImageProvider>
#include <QImage>
#include <QMutex>

#include "image/imageprovider.h"

class ImageProvider_qmlwrapper : public QDeclarativeImageProvider
{
public:
    explicit ImageProvider_qmlwrapper(QObject *parent = 0);
    virtual ~ImageProvider_qmlwrapper (void);

    QImage requestImage ( const QString& id, QSize* size, const QSize& requestedSize );

    void setCurrentImage (QImage image) {
        m_currentImage = image;
    }

private:
    QMutex m_mutex;

    QImage m_currentImage;

    ImageProvider m_imageProvider;
};

#endif // IMAGEPROVIDER_QMLWRAPPER_H
