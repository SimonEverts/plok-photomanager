#ifndef CAPTURE_H
#define CAPTURE_H

#include <QString>
#include <QList>
#include <QDateTime>

#include <model/picture.h>

class Capture
{
public:
    Capture();
    virtual ~Capture();

    void setName (QString name);
    QString name (void) const;

    void setCaptureTime (QDateTime dateTime);
    QDateTime captureTime (void) const;

    void addPhoto (QString path);

    void setPhotos (QList<QString> photos);
    QList <QString> photoList (void);

    Picture jpegByCamera (void) {return m_jpegByCamera;}
    void setJpegByCamera (Picture picture) {m_jpegByCamera = picture;}

    Picture raw (void) {return m_raw;}
    void setRaw (Picture picture) {m_raw = picture;}

    Picture rawJpegByCamera (void) {return m_rawJpegByCamera;}
    void setRawJpegByCamera (Picture picture) {m_rawJpegByCamera = picture;}

    int photoCount (void) const;

    QString previewPhoto (void);

    void clear (void);
private:
    QString m_name;

    QDateTime m_captureTime;

    QList <QString> m_pictures;

    Picture m_jpegByCamera;
    Picture m_raw;
    Picture m_rawJpegByCamera;
};

#endif // PHOTO_H
