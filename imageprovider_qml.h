#ifndef IMAGEPROVIDER_QML_H
#define IMAGEPROVIDER_QML_H

#include <QDeclarativeImageProvider>
#include <QImage>

#include "imageprovider.h"

class ImageProvider_qml : public QDeclarativeImageProvider
{
public:
    explicit ImageProvider_qml(QObject *parent = 0);
    virtual ~ImageProvider_qml (void);

    QImage requestImage ( const QString& id, QSize* size, const QSize& requestedSize );

private:
    ImageProvider m_imageProvider;
};

#endif // IMAGEPROVIDER_QML_H
