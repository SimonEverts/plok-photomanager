#include "setdao.h"

#include "database/database.h"

#include <QSqlRecord>
#include <QVariant>

SetDao::SetDao( Database* database) :
    m_database (database)
{
}

void SetDao::create (Set set)
{
    m_database->open();

    QString query ("INSERT INTO sets (name, date, path) VALUES (");

    query += "'" + set.name() + "',";
    query += "'" + set.date() + "',";
    query += m_database->formatString( set.path() ) +");";

    m_database->exec( query );

    m_database->close();
}

void SetDao::remove (Set set)
{
    m_database->open();

    QString query ("DELETE FROM sets WHERE id=");

    query += QString::number(set.id()) + ";";

    m_database->exec( query );

    m_database->close();
}


void SetDao::update (Set set)
{
    m_database->open();

    QString query ("UPDATE sets SET ");

    query += "name='" + set.name() + "',";
    query += "date='" + set.date() + "',";
    query += "path='" + m_database->formatString( set.path() ) + "'";

    query += " WHERE id=" + QString::number(set.id()) + ";";

    m_database->exec( query );

    m_database->close();
}

QList <Set> SetDao::sets ()
{
    m_database->open();

    QList <Set> result;

    QString query ("SELECT id,name,date,path FROM sets;");

    QSqlQuery qsql_query = m_database->exec( query );

    if (qsql_query.isActive())
    {
        QSqlRecord record = qsql_query.record();
        int id_index = record.indexOf ("id");
        int name_index = record.indexOf ("name");
        int date_index = record.indexOf ("date");
        int path_index = record.indexOf ("path");

        while (qsql_query.next())
            result.push_back(
                        Set (
                            qsql_query.value(name_index).toString(),
                            qsql_query.value(path_index).toString(),
                            qsql_query.value(date_index).toString(),
                            qsql_query.value(id_index).toInt() ));
    }

    m_database->close();

    return result;
}
