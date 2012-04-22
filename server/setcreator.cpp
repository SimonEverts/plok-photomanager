#include "setcreator.h"

#include <QHttpMultiPart>
#include <QFileInfo>

#include <QDebug>

#include "3rdparty/json.h"

SetCreator::SetCreator()
{
}

void SetCreator::createSet (QString name)
{
//    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

//    QHttpPart uuidPart;
//    uuidPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"uuid\""));
//    uuidPart.setBody("");

//    QHttpPart filenamePart;
//    filenamePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"filename\""));
//    filenamePart.setBody( QFileInfo(fileName).fileName().toAscii() );

//    qDebug() << QFileInfo(fileName).fileName();

//    QHttpPart imagePart;
//    imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/jpeg"));
//    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"datafile\""));

//    QFile *file = new QFile( QUrl(fileName).toString(QUrl::RemoveScheme) );
//    file->open(QIODevice::ReadOnly);
//    imagePart.setBodyDevice(file);

//    file->setParent(multiPart); // we cannot delete the file now, so delete it with the multiPart

//    multiPart->append(uuidPart);
//    multiPart->append(filenamePart);
//    multiPart->append(imagePart);

    QUrl url("http://www.plok.org/rest/sets");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

//    QVariantMap map;
//    map["method"] = "createSet";

//    QVariantMap params;
//    params["name"] = "MySetName";
//    params["path"] = "DoWeNeedAPath";

//    map["params"] = params;

    QVariantMap map;
    map["setName"] = "MySetName";

//    QVariantMap params;
//    params["name"] = "MySetName";
//    params["path"] = "DoWeNeedAPath";

//    map["params"] = params;



    QByteArray json = QtJson::Json::serialize( map );

        qDebug() << json;

    QNetworkReply *reply = m_networkAccessManager.post(request, json);
    //multiPart->setParent(reply); // delete the multiPart with the reply
    // here connect signals etc.

    connect( reply, SIGNAL( error(QNetworkReply::NetworkError)), this, SLOT(createSetError(QNetworkReply::NetworkError)));
    connect( reply, SIGNAL( uploadProgress (qint64, qint64)), this, SLOT(createSetProgress (qint64, qint64)));
    connect( reply, SIGNAL( finished()), this, SLOT(createSetFinished()));
}

void SetCreator::createSetError (QNetworkReply::NetworkError code)
{
    QString error_string;

    qDebug() << "Upload error: " << code;
}

void SetCreator::createSetProgress (qint64 bytesSent, qint64 bytesTotal )
{
    qDebug() << "Bytes send: " << bytesSent << "\t Bytes total: " << bytesTotal;
}

void SetCreator::createSetFinished (void)
{
    qDebug() << "Upload finished!";
}
