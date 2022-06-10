#ifndef SETDAO_H
#define SETDAO_H

#include "model/set.h"

#include <QList>

class Database;

class SetDao
{
public:
    SetDao( Database* m_database );

    void create (Set set);
    void remove (Set set);

    void update (Set set);

    QList <Set> sets ();

private:
    Database* m_database;
};

#endif // SETDAO_H
