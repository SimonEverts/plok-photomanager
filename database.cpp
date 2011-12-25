#include "database.h"

#include "miscutils.h"

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

    QSqlQuery create_table (QString ("CREATE TABLE IF NOT EXISTS sets (") +
                            "id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT," +
                            "name VARCHAR(30)," +
                            "path VARCHAR(255));",
                            *m_database);



    if (!create_table.exec())
    {
        qDebug() << m_database->lastError();
        qDebug() << create_table.lastError();
    }

    m_database->close();
}

void Database::addSet (Set set)
{
    if (!m_database->open())
        qDebug () << m_database->lastError();

    QSqlDriver* driver = m_database->driver();

    QString query_add_set ("INSERT INTO sets (name, path) VALUES (");

    QSqlField path_field ("name", QVariant::String);
    path_field.setValue( set.path() );

    query_add_set += "'" + set.name() + "',";
    query_add_set += driver->formatValue(path_field) +");";

        qDebug () << query_add_set;

    QSqlQuery add_set (query_add_set, *m_database);

    // TODO use QSqlRecord to format the path

    if (add_set.lastError().isValid())
    {
        qDebug() << m_database->lastError();
        qDebug() << add_set.lastError();
    }

    m_database->close();
}

QList <Set> Database::sets ()
{
    if (!m_database->open())
        qDebug () << m_database->lastError();

    QString query_add_set ("SELECT name,path FROM sets;");


    QSqlQuery add_set (query_add_set, *m_database);

    // TODO use QSqlRecord to format the path

    if (!add_set.exec())
    {
        qDebug() << m_database->lastError();
        qDebug() << add_set.lastError();
    }

    QSqlRecord record = add_set.record();
    int name_index = record.indexOf ("name");
    int path_index = record.indexOf ("path");

    QList <Set> result;

    while (add_set.next())
        result.push_back( Set (add_set.value(name_index).toString(), add_set.value(path_index).toString() ));

    m_database->close();

    return result;
}
