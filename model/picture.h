#ifndef PHOTO_H
#define PHOTO_H

#include <QString>
#include <QMap>
#include <QVariant>

#include "image/image.h"

class Picture
{
public:
    Picture ();
    explicit Picture (QString name, QString path, Image image);
    virtual ~Picture (void);

    void addInfo (QString name, QVariant value);

    QString name() const {return m_name;}
    QString path() const {return m_path;}

    const Image& image() const {return m_image;}
private:
    QString m_name;
    QString m_path;

    Image m_image;

    QMap <QString, QVariant> m_info;
};

#endif // PHOTO_H
