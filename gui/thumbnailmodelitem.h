#ifndef THUMBNAILMODELITEM_H
#define THUMBNAILMODELITEM_H

#include <QObject>
#include <QString>
#include <QVariantMap>

class ThumbnailModelItem : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY( QString path READ path WRITE setPath NOTIFY pathChanged)
    Q_PROPERTY( bool selected READ selected WRITE setSelected NOTIFY selectedChanged)

    Q_PROPERTY( int imageCount READ imageCount WRITE setImageCount NOTIFY imageCountChanged)

    Q_PROPERTY( QVariantMap info READ info WRITE setInfo NOTIFY infoChanged)

public:
    ThumbnailModelItem( QString name, QString path, int imageCount);

    QString name (void) {return m_name;}
    void setName (QString name) {m_name = name; emit nameChanged (name);}

    QString path (void) {return m_path;}
    void setPath (QString path) {m_path = path; emit pathChanged (path);}

    bool selected (void) {return m_selected;}
    void setSelected (bool selected) {m_selected = selected; emit selectedChanged( m_selected );}
    void toggleSelected (void) {m_selected = !m_selected; emit selectedChanged( m_selected );}

    int imageCount (void) {return m_imageCount;}
    void setImageCount (int imageCount) {m_imageCount = imageCount; emit imageCountChanged (imageCount);}

    QVariantMap info (void) {return m_info;}
    void setInfo (QVariantMap info) {m_info = info; emit infoChanged (info);}

signals:
    void nameChanged (QString name);
    void pathChanged (QString path);

    void selectedChanged (bool selected);

    void imageCountChanged (int imageCount);

    void infoChanged (QVariantMap info);

private:
    explicit ThumbnailModelItem(QObject* parent = 0);

    QString m_name;
    QString m_path;

    int m_imageCount;

    bool m_selected;

    QVariantMap m_info;
};

#endif // THUMBNAILMODELITEM_H
