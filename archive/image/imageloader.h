#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include <QObject>
#include <QString>
#include <QStringList>

#include <QMap>
#include <QVariant>

#include "image.h"

class ImageLoader : public QObject
{
    Q_OBJECT
public:
    explicit ImageLoader(QObject *parent = 0);
    virtual ~ImageLoader (void);

    virtual void openImage (QString imagePath) = 0;

    virtual Image loadThumbnail (void) = 0;
    virtual Image loadPreview (void) = 0;
    virtual Image loadMaster (void) = 0;

    virtual QMap <QString, QVariant> loadInfo (void) = 0;

    virtual QStringList supportedFormats (void) = 0;
};

#endif // IMAGELOADER_H
