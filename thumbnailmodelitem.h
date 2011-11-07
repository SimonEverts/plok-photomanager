#ifndef THUMBNAILMODELITEM_H
#define THUMBNAILMODELITEM_H

#include <QObject>

class ThumbnailModelItem : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY( QString path READ path WRITE setPath NOTIFY pathChanged)
    Q_PROPERTY( bool selected READ selected WRITE setSelected NOTIFY selectedChanged)

public:
    ThumbnailModelItem( QString name, QString path);

    QString name (void) {return m_name;}
    void setName (QString name) {m_name = name; emit nameChanged (name);}

    QString path (void) {return m_path;}
    void setPath (QString path) {m_path = path; emit pathChanged (path);}

    bool selected (void) {return m_selected;}
    void setSelected (bool selected) {m_selected = selected;}

signals:
    void nameChanged (QString name);
    void pathChanged (QString path);

    void selectedChanged (bool selected);

private:
    explicit ThumbnailModelItem(QObject* parent = 0);

    QString m_name;
    QString m_path;

    bool m_selected;
};

#endif // THUMBNAILMODELITEM_H
