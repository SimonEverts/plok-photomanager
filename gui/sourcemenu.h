#ifndef SOURCEMENU_H
#define SOURCEMENU_H

#include <QListWidget>

class SourceMenu : public QListWidget
{
protected:
    QStyleOptionViewItem viewOptions () const;
public:
    SourceMenu (QWidget* parent);
    virtual ~SourceMenu();
};

#endif // SOURCEMENU_H
