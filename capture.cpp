#include "capture.h"

#include <QFileInfo>

Capture::Capture()
{
}

Capture::~Capture()
{

}

void Capture::setName (QString name)
{
    m_name = name;
}

QString Capture::name (void)
{
    return m_name;
}

void Capture::setCaptureTime (QDateTime captureTime)
{
    m_captureTime = captureTime;
}

QDateTime Capture::captureTime (void)
{
    return m_captureTime;
}

void Capture::addPhoto (QString path)
{
    m_photos.push_back( path );
}

QString Capture::previewPhoto (void)
{
    QString result;

    QList<QString>::iterator it;
    for (it=m_photos.begin(); it != m_photos.end(); it++)
    {
        QFileInfo file_info (*it);
        if (file_info.suffix() == "jpg" || file_info.suffix() == "JPG")
            result = *it;
    }

    return result;
}

void Capture::clear (void)
{
    m_photos.clear();
}
