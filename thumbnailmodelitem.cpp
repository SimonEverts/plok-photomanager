#include "thumbnailmodelitem.h"

ThumbnailModelItem::ThumbnailModelItem (QObject* parent) :
    QObject (parent)
{
}

ThumbnailModelItem::ThumbnailModelItem( QString name, QString path) :
    m_name( name ),
    m_path( path )
{

}
