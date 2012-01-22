#ifndef PICTUREDAO_H
#define PICTUREDAO_H

#include "picture.h"

#include <QList>

class Database;

class PictureDao
{
public:
    PictureDao( Database* m_database );

    void create (Picture picture);
    void remove (Picture picture);

    Picture read (QString path);

    void update (Picture picture);

    QList <Picture> pictures ();

private:
    Database* m_database;
};

#endif // PICTUREDAO_H
