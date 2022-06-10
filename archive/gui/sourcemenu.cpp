#include "sourcemenu.h"

SourceMenu::SourceMenu (QWidget* parent) :
        QListWidget (parent)
{
}

SourceMenu::~SourceMenu()
{
}

QStyleOptionViewItem SourceMenu::viewOptions () const
{
    QStyleOptionViewItem option = QListView::viewOptions();

    option.decorationPosition = QStyleOptionViewItem::Top;

    return option;
}
