#include "setviewmodelitem.h"

SetViewModelItem::SetViewModelItem( QString name, QString date, QString image ) :
    QObject(0),
    m_name (name),
    m_date (date),
    m_image (image)
{
}
