#include "imageuploader.h"

#include <QHttpMultiPart>
#include <QFileInfo>

#include <QDebug>

ImageUploader::ImageUploader()
{
}

void ImageUploader::uploadImage (QString fileName)
{
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart uuidPart;
    uuidPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"uuid\""));
    uuidPart.setBody("");

    QHttpPart filenamePart;
    filenamePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"filename\""));
    filenamePart.setBody( QFileInfo(fileName).fileName().toAscii() );

    qDebug() << QFileInfo(fileName).fileName();

    QHttpPart imagePart;
    imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/jpeg"));
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"datafile\""));

    QFile *file = new QFile( QUrl(fileName).toString(QUrl::RemoveScheme) );
    file->open(QIODevice::ReadOnly);
    imagePart.setBodyDevice(file);

    file->setParent(multiPart); // we cannot delete the file now, so delete it with the multiPart

    multiPart->append(uuidPart);
    multiPart->append(filenamePart);
    multiPart->append(imagePart);

    QUrl url("http://plok.org:8282/plokmc-1.0-SNAPSHOT/fileUpload");
    QNetworkRequest request(url);

    QNetworkReply *reply = m_networkAccessManager.post(request, multiPart);
    multiPart->setParent(reply); // delete the multiPart with the reply
    // here connect signals etc.

    connect( reply, SIGNAL( error(QNetworkReply::NetworkError)), this, SLOT(uploadError(QNetworkReply::NetworkError)));
    connect( reply, SIGNAL( uploadProgress (qint64, qint64)), this, SLOT(uploadProgress (qint64, qint64)));
    connect( reply, SIGNAL( finished()), this, SLOT(uploadFinished()));
}

void ImageUploader::uploadError (QNetworkReply::NetworkError code)
{
    QString error_string;

    qDebug() << "Upload error: " << code;
}

void ImageUploader::uploadProgress (qint64 bytesSent, qint64 bytesTotal )
{
    qDebug() << "Bytes send: " << bytesSent << "\t Bytes total: " << bytesTotal;
}

void ImageUploader::uploadFinished (void)
{
    qDebug() << "Upload finished!";
}
