#ifndef SET_H
#define SET_H

#include <QString>

class Set
{
public:
    Set( QString name, QString path, QString date, int id = -1 );

    int id() const {return m_id;}
    QString name() const {return m_name;}
    QString path() const {return m_path;}
    QString date() const {return m_date;}

private:
    int m_id;
    QString m_name;
    QString m_path;
    QString m_date;
};

#endif // SET_H
