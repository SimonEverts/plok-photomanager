#ifndef WORKTHREAD_H
#define WORKTHREAD_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>

#include <QStringList>
#include <QImage>

#include "model/photo.h"
#include "image/imageprovider.h"

class WorkThread : public QThread
{
    Q_OBJECT

public:
    WorkThread();

    void start();
    void stop();

    void run();
    void wakeup ();

    void loadPhotos (QStringList fileNames);
    QList <Photo> photos (void);
private:
    QMutex m_mutex;
    QWaitCondition m_conditionChanged;

    bool m_exit;

    ImageProvider m_imageProvider;

    QStringList m_fileNames;
    QList <Photo> m_photos;

signals:
     void photoLoaded (void);
};

#endif // WORKTHREAD_H
