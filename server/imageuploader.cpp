#include "imageuploader.h"

#include <QHttpMultiPart>
#include <QFileInfo>

#include <QDebug>

//#include <3rdparty/qoauth/interface.h>

ImageUploader::ImageUploader()
{
}

//void ImageUploader::authenticate (void)
//{
//    QByteArray token;
//    QByteArray tokenSecret;

//    QOAuth::Interface oauth_interface;

//    oauth_interface.setConsumerKey( "75b3d557c9268c49cfdf041a" );
//    oauth_interface.setConsumerSecret( "fd12803fbf0760d34cd2ceb9955199ce" );
//    // set a timeout for requests (in msecs)
//    oauth_interface.setRequestTimeout( 10000 );

//    // send a request for an unauthorized token
//    QOAuth::ParamMap reply =
//        oauth_interface.requestToken( "http://www.plok.org/rest/oauth",
//                              QOAuth::GET, QOAuth::HMAC_SHA1 );

//    // if no error occurred, read the received token and token secret
//    if ( oauth_interface.error() == QOAuth::NoError ) {
//      //token = reply.value( QOAuth::ParamToken );
//      //tokenSecret = reply.value( QOAuth::ParamTokenSecret );
//    }

//    qDebug() << reply;
//}

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
