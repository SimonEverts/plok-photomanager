#ifndef PHOTO_H
#define PHOTO_H

#include <QString>
#include <QImage>
#include <QMap>
#include <QVariant>

class Picture
{
public:
    Picture ();
    explicit Picture (QString name, QString path, QImage image);
    virtual ~Picture (void);

    void addInfo (QString name, QVariant value);

    QString name() const {return m_name;}
    QString path() const {return m_path;}

    QImage image() const {return m_image;}
private:
    QString m_name;
    QString m_path;

    QImage m_image;

    QMap <QString, QVariant> m_info;
};

#endif // PHOTO_H
