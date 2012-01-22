#include "database.h"

#include "common/miscutils.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlDriver>
#include <QSqlField>

#include <QList>
#include <QDebug>
#include <QDir>

#include <QRgb>

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
    QDir database_dir (MiscUtils::plokpmDir());

    if (!database_dir.exists())
        database_dir.mkpath (database_dir.path());

    qDebug () << database_dir.path();

    m_database->setDatabaseName( database_dir.path() + "/plok-pm.db.sqlite");

    if (!m_database->open())
        qDebug () << m_database->lastError();

    QSqlQuery create_sets (QString ("CREATE TABLE IF NOT EXISTS sets (") +
                            "id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT," +
                            "name VARCHAR(30)," +
                            "path VARCHAR(255));",
                            *m_database);

    if (create_sets.lastError().isValid())
    {
        qDebug() << m_database->lastError();
        qDebug() << create_sets.lastError();
    }

    QSqlQuery create_pictures (QString ("CREATE TABLE IF NOT EXISTS pictures (") +
                               "id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT," +
                               "name VARCHAR(30)," +
                               "path VARCHAR(255)," +
                               "brightness FLOAT," +
                               "contrast FLOAT," +
                               "gamma FLOAT," +
                               "wb_red FLOAT,"  +
                               "wb_green FLOAT," +
                               "wb_blue FLOAT);",
                               *m_database);

    if (create_pictures.lastError().isValid())
    {
        qDebug() << m_database->lastError();
        qDebug() << create_pictures.lastError();
    }


    m_database->close();
}

void Database::open (void)
{
    if (!m_database->open())
        qDebug () << m_database->lastError();
}

void Database::close (void)
{
    m_database->close();
}

QString Database::formatString (QString string)
{
    QSqlDriver* driver = m_database->driver();

    QSqlField field ("", QVariant::String);
    field.setValue( string );

    return driver->formatValue( field );
}

QSqlQuery Database::exec (QString query)
{
    QSqlQuery qsql_query (query, *m_database);

    if (qsql_query.lastError().isValid())
    {
        qDebug() << m_database->lastError();
        qDebug() << qsql_query.lastError();
    }

    return qsql_query;
}
