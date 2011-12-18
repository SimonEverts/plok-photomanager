#ifndef IMAGELOADER_GENERIC_H
#define IMAGELOADER_GENERIC_H

#include "imageloader.h"

class ImageLoader_generic_p;

class ImageLoader_generic : public ImageLoader
{
public:
    explicit ImageLoader_generic (QObject *parent = 0);
    virtual ~ImageLoader_generic (void);

    void openImage (QString imagePath);

    QImage loadThumbnail (void);

    QMap <QString, QVariant> loadInfo (void);

    QStringList supportedFormats (void);

private:
    ImageLoader_generic_p* p;
};

#endif // IMAGELOADER_GENERIC_H
