#ifndef LIBRARY_H
#define LIBRARY_H

#include "model/set.h"

#include <QString>

class Library
{
public:
    Library();

    virtual void importSet (const Set& set) = 0;

    virtual void addSet (const Set& set) = 0;
    virtual void removeSet (QString name) = 0;
};

#endif // LIBRARY_H
