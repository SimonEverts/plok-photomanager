#ifndef DATABASE_H
#define DATABASE_H

#include <QString>

#include "model/set.h"

class QSqlDatabase;

class Database
{
public:
    Database();
    virtual ~Database();

    void initialize (void);

    void addSet (Set set);
    QList <Set> sets ();

private:
    QSqlDatabase* m_database;
};

#endif // DATABASE_H
