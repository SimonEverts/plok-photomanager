#ifndef PHOTO_H
#define PHOTO_H

#include <QString>
#include <QImage>
#include <QMap>
#include <QVariant>

class Photo
{
public:
    Photo ();
    explicit Photo (QString name, QString path, QImage image);
    virtual ~Photo (void);

    void addInfo (QString name, QVariant value);
private:
    QString m_name;
    QString m_path;

    QImage m_image;

    QMap <QString, QVariant> m_info;
};

#endif // PHOTO_H
