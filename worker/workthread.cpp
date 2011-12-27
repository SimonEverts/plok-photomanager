#include "workthread.h"

#include <QMutexLocker>

#include <QFileInfo>

#include <QDebug>

WorkThread::WorkThread()
{
}

void WorkThread::start()
{
    {
        QMutexLocker locker (&m_mutex);

        m_exit = false;
    }

    QThread::start();
}

void WorkThread::stop()
{
    {
        QMutexLocker locker (&m_mutex);

        m_exit = true;
        m_conditionChanged.wakeOne();
    }

    wait();
}

void WorkThread::run()
{
    bool exit = false;

    setPriority (QThread::LowPriority);

    while (!exit)
    {
        QString file_name;

        {
            QMutexLocker locker( &m_mutex );

            if( m_exit)
            {
                exit = true;
                continue;
            }

            if (!m_fileNames.isEmpty())
                file_name = m_fileNames.takeFirst();
            else
                m_conditionChanged.wait( &m_mutex );
        }

        // Load image
        if (!file_name.isEmpty())
        {
            QImage image = m_imageProvider.loadImage (file_name);
            if (!image.isNull())
            {
                Photo photo (QFileInfo(file_name).baseName (), file_name, image);

                {
                    QMutexLocker locker( &m_mutex );

                    m_photos.push_back(photo);
                }

                emit photoLoaded();
            }
        }
    }
}

void WorkThread::wakeup ()
{
    QMutexLocker locker( &m_mutex );

    m_conditionChanged.wakeOne();
}

void WorkThread::loadPhotos (QStringList fileNames)
{
    {
        QMutexLocker locker( &m_mutex );

        m_fileNames = fileNames;
    }

    wakeup();
}

QList <Photo> WorkThread::photos (void)
{
    QList <Photo> photo_list;
    {
        QMutexLocker locker( &m_mutex );

        photo_list = m_photos;
        m_photos.clear();
    }

    return photo_list;
}
