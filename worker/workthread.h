#ifndef WORKTHREAD_H
#define WORKTHREAD_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>

#include <QStringList>
#include <QImage>

#include "model/picture.h"
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

    void loadPictures (QStringList fileNames);
    QList <Picture> pictures (void);
private:
    QMutex m_mutex;
    QWaitCondition m_conditionChanged;

    bool m_exit;

    ImageProvider m_imageProvider;

    QStringList m_fileNames;
    QList <Picture> m_pictures;

signals:
     void photoLoaded (void);
};

#endif // WORKTHREAD_H
