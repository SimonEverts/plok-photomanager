#include "picturedao.h"

#include "database/database.h"

#include <QSqlRecord>
#include <QVariant>

#include <QDebug>

PictureDao::PictureDao( Database* database) :
    m_database (database)
{
}

void PictureDao::create (Picture picture)
{
    m_database->open();

    QString query ("INSERT INTO pictures (name,path,brightness,contrast,gamma,wb_red,wb_green,wb_blue) VALUES (");

    PictureProperties properties = picture.pictureProperties();

    query += "'" + picture.name() + "',";
    query += m_database->formatString( picture.path() ) +",";
    query += QString::number(properties.brightness) +",";
    query += QString::number(properties.contrast) +",";
    query += QString::number(properties.gamma) +",";
    query += QString::number(properties.wb_red) +",";
    query += QString::number(properties.wb_green) +",";
    query += QString::number(properties.wb_blue) +");";

    m_database->exec( query );

    m_database->close();
}

void PictureDao::remove (Picture picture)
{
    m_database->open();

    QString query ("DELETE FROM pictures WHERE id=");

    query += QString::number(picture.id()) + ";";

    m_database->exec( query );

    m_database->close();
}

Picture PictureDao::read (QString path)
{
    Picture result;

    m_database->open();

    QString query (QString("SELECT id,name,path,brightness,contrast,gamma,wb_red,wb_green,wb_blue FROM pictures WHERE path=") + m_database->formatString( path ) + ";");

    qDebug () << query;

    QSqlQuery qsql_query = m_database->exec( query );

    if (qsql_query.isActive())
    {
        QSqlRecord record = qsql_query.record();
        int id_index = record.indexOf ("id");
        int name_index = record.indexOf ("name");
        int path_index = record.indexOf ("path");

        int brightness_index = record.indexOf ("brightness");
        int constrast_index = record.indexOf ("contrast");
        int gamma_index = record.indexOf ("gamma");

        int wb_red_index = record.indexOf ("wb_red");
        int wb_green_index = record.indexOf ("wb_green");
        int wb_blue_index = record.indexOf ("wb_blue");

        while (qsql_query.next())
        {
            result = Picture(
                            qsql_query.value(id_index).toInt(),
                            qsql_query.value(name_index).toString(),
                            qsql_query.value(path_index).toString(),
                            qsql_query.value(brightness_index).toFloat(),
                            qsql_query.value(constrast_index).toFloat(),
                            qsql_query.value(gamma_index).toFloat(),
                            qsql_query.value(wb_red_index).toFloat(),
                            qsql_query.value(wb_green_index).toFloat(),
                            qsql_query.value(wb_blue_index).toFloat());
        }

    }

    m_database->close();

    return result;
}

void PictureDao::update (Picture picture)
{
    m_database->open();

    PictureProperties properties = picture.pictureProperties();

    QString query ("UPDATE pictures SET ");

    query += "name='" + picture.name() + "',";
    query += "path=" + m_database->formatString( picture.path() ) + ",";

    query += "brightness=" + QString::number(properties.brightness) + ",";
    query += "contrast=" + QString::number(properties.contrast) + ",";
    query += "gamma=" + QString::number(properties.gamma) + ",";

    query += "wb_red=" + QString::number(properties.wb_red) + ",";
    query += "wb_green=" + QString::number(properties.wb_green) + ",";
    query += "wb_blue=" + QString::number(properties.wb_blue);

    query += " WHERE id=" + QString::number(picture.id()) + ";";

    qDebug() << query;

    m_database->exec( query );

    m_database->close();
}

QList <Picture> PictureDao::pictures ()
{
    m_database->open();

    QList <Picture> result;

    QString query ("SELECT id,name,path,brightness,contrast,gamma,wb_red,wb_green,wb_blue FROM pictures;");

    QSqlQuery qsql_query = m_database->exec( query );

    if (qsql_query.isActive())
    {
        QSqlRecord record = qsql_query.record();
        int id_index = record.indexOf ("id");
        int name_index = record.indexOf ("name");
        int path_index = record.indexOf ("path");

        int brightness_index = record.indexOf ("brightness");
        int constrast_index = record.indexOf ("contrast");
        int gamma_index = record.indexOf ("gamma");

        int wb_red_index = record.indexOf ("wb_red");
        int wb_green_index = record.indexOf ("wb_green");
        int wb_blue_index = record.indexOf ("wb_blue");

        while (qsql_query.next())
        {
            result.push_back(
                        Picture (
                            qsql_query.value(id_index).toInt(),
                            qsql_query.value(name_index).toString(),
                            qsql_query.value(path_index).toString(),
                            qsql_query.value(brightness_index).toFloat(),
                            qsql_query.value(constrast_index).toFloat(),
                            qsql_query.value(gamma_index).toFloat(),
                            qsql_query.value(wb_red_index).toFloat(),
                            qsql_query.value(wb_green_index).toFloat(),
                            qsql_query.value(wb_blue_index).toFloat()));
        }

    }

    m_database->close();

    return result;
}
