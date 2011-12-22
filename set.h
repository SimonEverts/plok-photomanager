#ifndef SET_H
#define SET_H

#include <QString>

class Set
{
public:
    Set( QString name, QString path);

    QString name() const {return m_name;}
    QString path() const {return m_path;}

private:
    QString m_name;
    QString m_path;
};

#endif // SET_H
