#ifndef CAPTURE_H
#define CAPTURE_H

#include <QString>
#include <QList>
#include <QDateTime>

#include <photo.h>

class Capture
{
public:
    Capture();
    virtual ~Capture();

    void setName (QString name);
    QString name (void);

    void setCaptureTime (QDateTime dateTime);
    QDateTime captureTime (void);

    void addPhoto (QString path);

    void setPhotos (QList<QString> photos);
    QList <QString> photoList (void);

    int photoCount (void);

    QString previewPhoto (void);

    void clear (void);
private:
    QString m_name;

    QDateTime m_captureTime;

    QList <QString> m_photos;
};

#endif // PHOTO_H
