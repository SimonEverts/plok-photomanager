#ifndef SETCREATOR_H
#define SETCREATOR_H

#include <QObject>
#include <QString>
#include <QNetworkReply>
#include <QNetworkAccessManager>

class SetCreator : public QObject
{
    Q_OBJECT

public:
    SetCreator();

    void createSet (QString name);

public slots:
    void createSetError (QNetworkReply::NetworkError code);
    void createSetProgress (qint64 bytesSent, qint64 bytesTotal );
    void createSetFinished (void);

private:
    QNetworkAccessManager m_networkAccessManager;
};

#endif // SETCREATOR_H
