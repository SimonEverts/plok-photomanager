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
            Image image = m_imageProvider.loadPreview (file_name);
            if (!image.isNull())
            {
                Picture picture (QFileInfo(file_name).baseName (), file_name, image);

                {
                    QMutexLocker locker( &m_mutex );

                    m_pictures.push_back(picture);
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

void WorkThread::loadPictures (QStringList fileNames)
{
    {
        QMutexLocker locker( &m_mutex );

        m_fileNames = fileNames;
    }

    wakeup();
}

QList <Picture> WorkThread::pictures (void)
{
    QList <Picture> picture_list;
    {
        QMutexLocker locker( &m_mutex );

        picture_list = m_pictures;
        m_pictures.clear();
    }

    return picture_list;
}
