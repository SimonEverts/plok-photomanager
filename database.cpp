#include "database.h"

#include <QProcessEnvironment>
#include <QSqlDatabase>
#include <QSqlError>

#include <QList>
#include <QDebug>
#include <QDir>

Database::Database()
{
    m_database = new QSqlDatabase (QSqlDatabase::addDatabase("QSQLITE"));
}

Database::~Database()
{
    delete m_database;
}

void Database::initialize()
{
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment ();

    QDir database_dir (env.value("HOME") + "/.plok-pm/");

    if (!database_dir.exists())
        database_dir.mkpath (database_dir.path());

    qDebug () << database_dir.path();

    m_database->setDatabaseName( database_dir.path() + "/plok-pm.db.sqlite");

    if (!m_database->open())
        qDebug () << m_database->lastError();

    m_database->close();
}

void Database::addSet (Set set)
{
    if (!m_database->open())
        qDebug () << m_database->lastError();



    m_database->close();
}

QList <Set> Database::sets ()
{

    return QList <Set> ();
}
