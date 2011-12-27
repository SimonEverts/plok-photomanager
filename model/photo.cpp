#include "photo.h"

Photo::Photo ()
{

}

Photo::Photo (QString name, QString path, QImage image) :
    m_name (name), m_path (path), m_image (image)
{

}

Photo::~Photo (void)
{

}

void Photo::addInfo (QString name, QVariant value)
{
    m_info[name] = value;
}
