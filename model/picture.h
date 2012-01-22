#ifndef PHOTO_H
#define PHOTO_H

#include <QString>
#include <QMap>
#include <QVariant>

#include "image/image.h"

class PictureProperties
{
public:
    PictureProperties (void) :
        brightness (0),
        contrast (1),
        gamma (1),
        wb_red (1),
        wb_green (1),
        wb_blue (1)
    {}

    void clear (void) {
        brightness = 0;
        contrast = 1;
        gamma = 1;
        wb_red = 1;
        wb_green = 1;
        wb_blue = 1;
    }

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
    explicit Picture (int id, QString name, QString path, float brightness, float contrast, float gamma, float wb_red, float wb_green, float wb_blue);
    explicit Picture (QString name, QString path, Image image);
    virtual ~Picture (void);

    void addInfo (QString name, QVariant value);

    int id() const {return m_id;}
    void setId (int id) {m_id = id;}

    QString name() const {return m_name;}

    QString path() const {return m_path;}
    void setPath (QString path) {m_path = path;}

    const Image& image() const {return m_image;}
    void setImage (const Image& image) {m_image = image;}

    bool loaded (void) const {return !m_image.isNull();}

    PictureProperties& pictureProperties (void) {return m_pictureProperties;}
    void setPictureProperties (const PictureProperties& pictureProperties) {m_pictureProperties = pictureProperties;}
private:
    int m_id;

    QString m_name;
    QString m_path;

    Image m_image;

    // Editor settings
    PictureProperties m_pictureProperties;

    // Info
    QMap <QString, QVariant> m_info;
};

#endif // PHOTO_H
