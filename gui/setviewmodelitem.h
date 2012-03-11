#ifndef SETVIEWMODELITEM_H
#define SETVIEWMODELITEM_H

#include <QObject>
#include <QString>

class SetViewModelItem : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString date READ date WRITE setDate NOTIFY dateChanged)
    Q_PROPERTY(QString image READ image WRITE setImage NOTIFY imageChanged)
public:
    SetViewModelItem( QString name, QString date, QString image );
    
    void setName (QString name) {m_name = name;}
    QString name (void) {return m_name;}

    void setDate (QString date) {m_date = date;}
    QString date (void) {return m_date;}

    void setImage (QString image) {m_image = image;}
    QString image (void) {return m_image;}

signals:
    void nameChanged (QString name);
    void dateChanged (QString date);
    void imageChanged (QString date);
    
public slots:

private:
    QString m_name;
    QString m_date;

    QString m_image;
    
};

#endif // SETVIEWMODELITEM_H
