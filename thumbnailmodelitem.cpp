#include "thumbnailmodelitem.h"

ThumbnailModelItem::ThumbnailModelItem (QObject* parent) :
    QObject (parent)
{
}

ThumbnailModelItem::ThumbnailModelItem( QString name, QString path, int imageCount) :
    m_name( name ),
    m_path( path ),
    m_selected (false),
    m_imageCount (imageCount)
{

}
