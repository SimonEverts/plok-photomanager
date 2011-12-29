#ifndef SETVIEWMODELITEM_H
#define SETVIEWMODELITEM_H

#include <QObject>
#include <QString>

class SetViewModelItem : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
public:
    SetViewModelItem( QString name );
    
    void setName (QString name) {m_name = name;}
    QString name (void) {return m_name;}

signals:
    void nameChanged (QString name);
    
public slots:

private:
    QString m_name;
    
};

#endif // SETVIEWMODELITEM_H
