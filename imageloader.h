#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include <QObject>
#include <QString>
#include <QImage>

class ImageLoader_p;

class ImageLoader : public QObject
{
    Q_OBJECT
public:
    explicit ImageLoader(QObject *parent = 0);
    virtual ~ImageLoader (void);

    void openImage (QString imagePath);

    QImage loadThumbnail (void);
signals:

public slots:

private:
    ImageLoader_p* p;

};

#endif // IMAGELOADER_H
