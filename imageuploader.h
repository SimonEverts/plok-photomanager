#ifndef IMAGEUPLOADER_H
#define IMAGEUPLOADER_H

#include <QObject>
#include <QString>
#include <QNetworkReply>
#include <QNetworkAccessManager>

class ImageUploader : public QObject
{
    Q_OBJECT

public:
    ImageUploader();

    void uploadImage (QString fileName);

public slots:
    void uploadError (QNetworkReply::NetworkError code);
    void uploadProgress (qint64 bytesSent, qint64 bytesTotal );
    void uploadFinished (void);

private:
    QNetworkAccessManager m_networkAccessManager;
};

#endif // IMAGEUPLOADER_H
