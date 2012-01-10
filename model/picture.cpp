#include "picture.h"

Picture::Picture ()
{

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
