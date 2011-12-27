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

void Capture::setPhotos (QList<QString> photos)
{
    m_photos = photos;
}

QList <QString> Capture::photoList (void)
{
    return m_photos;
}

int Capture::photoCount (void)
{
    return m_photos.size();
}

QString Capture::previewPhoto (void)
{
    QString result;

    qDebug() << m_photos.size();

    // Check if one of the foto's is more suited for a thumbnail
    QList<QString>::iterator it;
    for (it=m_photos.begin(); it != m_photos.end(); it++)
    {
        QFileInfo file_info (*it);
        QString suffix = file_info.suffix();

        if (suffix == "RAW" || suffix == "raw" ||
                suffix == "ARW" || suffix == "arw" ||
                suffix == "CR2" || suffix == "cr2")
            result = *it;
    }

//    if (it == m_photos.end())

    if (m_photos.size() && result.isEmpty())
        result = m_photos.first();

    return result;
}

void Capture::clear (void)
{
    m_photos.clear();
}
