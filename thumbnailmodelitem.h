#ifndef THUMBNAILMODELITEM_H
#define THUMBNAILMODELITEM_H

#include <QObject>

class ThumbnailModelItem : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY( QString path READ path WRITE setPath NOTIFY pathChanged)

public:
    ThumbnailModelItem( QString name, QString path);

    QString name (void) {return m_name;}
    void setName (QString name) {m_name = name; emit nameChanged (name);}

    QString path (void) {return m_path;}
    void setPath (QString path) {m_path = path; emit pathChanged (path);}

signals:
    void nameChanged (QString name);
    void pathChanged (QString path);

private:
    explicit ThumbnailModelItem(QObject* parent = 0);

    QString m_name;
    QString m_path;
};

#endif // THUMBNAILMODELITEM_H
