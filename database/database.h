#ifndef DATABASE_H
#define DATABASE_H

#include <QString>
#include <QSqlQuery>

#include "model/set.h"

class QSqlDatabase;

class Database
{
public:
    Database();
    virtual ~Database();

    void initialize (void);

    void open (void);
    void close (void);

    QString formatString (QString string);

    // TODO encapsulate QSqlQuery?
    QSqlQuery exec (QString query);
private:
    QSqlDatabase* m_database;
};

#endif // DATABASE_H
