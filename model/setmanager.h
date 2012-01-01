#ifndef SETMANAGER_H
#define SETMANAGER_H

#include <QString>

class Database;

class SetManager
{
public:
    SetManager( Database* database);

    void createSet (QString path);
    void deleteSet (QString path);

private:
    Database* m_database;
};

#endif // SETMANAGER_H
