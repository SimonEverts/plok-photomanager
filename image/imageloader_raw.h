#ifndef IMAGELOADER_RAW_H
#define IMAGELOADER_RAW_H

#include "imageloader.h"

#include "image.h"

class ImageLoader_raw_p;

class ImageLoader_raw : public ImageLoader
{
public:
    explicit ImageLoader_raw (QObject *parent = 0);
    virtual ~ImageLoader_raw (void);

    void openImage (QString imagePath);

    Image loadThumbnail (void);
    Image loadPreview (void);
    Image loadMaster (void);

    QMap <QString, QVariant> loadInfo (void);

    QStringList supportedFormats (void);

private:
    ImageLoader_raw_p* p;
};

#endif // IMAGELOADER_RAW_H
