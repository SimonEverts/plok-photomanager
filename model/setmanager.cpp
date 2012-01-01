#include "setmanager.h"

#include "set.h"

#include "database/database.h"

#include <QDir>
#include <QFileInfo>
#include <QStringList>

SetManager::SetManager( Database* database ) :
    m_database (database)
{
}

void SetManager::createSet (QString path)
{
    QDir dir = QFileInfo(path).absoluteDir();

    if (!path.isEmpty())
    {
//        Set set (dir.dirName(), dir.absolutePath());

//        m_database->addSet (set);
    }
}

void SetManager::deleteSet (QString path)
{

}
