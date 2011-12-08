#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include <QObject>
#include <QString>
#include <QImage>

class ImageLoader : public QObject
{
    Q_OBJECT
public:
    explicit ImageLoader(QObject *parent = 0);
    virtual ~ImageLoader (void);

    virtual void openImage (QString imagePath) = 0;

    virtual QImage loadThumbnail (void) = 0;
};

#endif // IMAGELOADER_H
