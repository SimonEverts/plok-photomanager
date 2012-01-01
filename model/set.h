#ifndef SET_H
#define SET_H

#include <QString>

class Set
{
public:
    Set( QString name, QString path, int id = -1 );

    int id() const {return m_id;}
    QString name() const {return m_name;}
    QString path() const {return m_path;}

private:
    int m_id;
    QString m_name;
    QString m_path;
};

#endif // SET_H
