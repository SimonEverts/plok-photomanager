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

    Picture& jpegMaster (void) {return m_jpegMaster;}
    void setJpegMaster (Picture picture) {m_jpegMaster = picture;}

    Picture& rawMaster (void) {return m_rawMaster;}
    void setRawMaster (Picture picture) {m_rawMaster = picture;}

    Picture& preview (void) {return m_preview;}
    void setPreview (Picture picture) {m_preview = picture;}

    int photoCount (void) const;

    QString previewPhoto (void);

    void clear (void);
private:
    QString m_name;

    QDateTime m_captureTime;

    QList <QString> m_pictures;

    Picture m_jpegMaster;
    Picture m_rawMaster;

    Picture m_preview;
};

#endif // PHOTO_H
