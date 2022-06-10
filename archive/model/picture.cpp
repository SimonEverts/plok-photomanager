#include "picture.h"

Picture::Picture () :
    m_id (-1)
{

}

Picture::Picture (int id, QString name, QString path, float brightness, float contrast, float gamma, float wb_red, float wb_green, float wb_blue) :
    m_id (id),
    m_name (name),
    m_path (path)
{
    m_pictureProperties.brightness = brightness;
    m_pictureProperties.contrast = contrast;
    m_pictureProperties.gamma = gamma;
    m_pictureProperties.wb_red = wb_red;
    m_pictureProperties.wb_green = wb_green;
    m_pictureProperties.wb_blue = wb_blue;
}

Picture::Picture (QString name, QString path, Image image) :
    m_name (name), m_path (path), m_image (image)
{

}

Picture::~Picture (void)
{

}

void Picture::addInfo (QString name, QVariant value)
{
    m_info[name] = value;
}

void Picture::clearImage (void)
{
    m_image.clear();
    m_pictureProperties.clear();
    m_info.clear();
}
