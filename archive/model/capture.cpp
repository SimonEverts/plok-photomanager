#include "capture.h"

#include <QFileInfo>

#include <QDebug>

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

QString Capture::name (void) const
{
    return m_name;
}

void Capture::setCaptureTime (QDateTime captureTime)
{
    m_captureTime = captureTime;
}

QDateTime Capture::captureTime (void) const
{
    return m_captureTime;
}

void Capture::addPhoto (QString path)
{
    m_pictures.push_back( path );
}

void Capture::setPhotos (QList<QString> photos)
{
    m_pictures = photos;
}

QList <QString> Capture::photoList (void)
{
    return m_pictures;
}

int Capture::photoCount (void) const
{
    return m_pictures.size();
}

void Capture::selectMaster (QString path)
{
    m_master.clearImage();

    if (path.isEmpty())
        path = previewPhoto();

    m_master.setPath( path );

}

QString Capture::previewPhoto (void)
{
    QString result;

    // Check if one of the foto's is more suited for a thumbnail
    QList<QString>::iterator it;
    for (it=m_pictures.begin(); it != m_pictures.end(); it++)
    {
        QFileInfo file_info (*it);
        QString suffix = file_info.suffix();

        if (suffix == "RAW" || suffix == "raw" ||
                suffix == "ARW" || suffix == "arw" ||
                suffix == "CR2" || suffix == "cr2")
            result = *it;
    }

//    if (it == m_photos.end())

    if (m_pictures.size() && result.isEmpty())
        result = m_pictures.first();

    return result;
}

void Capture::clear (void)
{
    m_pictures.clear();
}
