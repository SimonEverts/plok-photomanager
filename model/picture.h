#ifndef PHOTO_H
#define PHOTO_H

#include <QString>
#include <QMap>
#include <QVariant>

#include "image/image.h"

class PictureAdjustments
{
public:
    PictureAdjustments (void) :
        brightness (0),
        contrast (1),
        gamma (1),
        wb_red (1),
        wb_green (1),
        wb_blue (1)
    {}

    float brightness;
    float contrast;
    float gamma;

    float wb_red;
    float wb_green;
    float wb_blue;
};

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

    // Editor settings
    PictureAdjustments m_pictureAdjustments;

    // Info
    QMap <QString, QVariant> m_info;
};

#endif // PHOTO_H
