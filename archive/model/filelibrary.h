#ifndef FILELIBRARY_H
#define FILELIBRARY_H

#include "library.h"
#include "set.h"

class FileLibrary : public Library
{
public:
    FileLibrary();

    Set set (QString name);

    void importSet (const Set& set) = 0;

    void addSet (const Set& set);
    void removeSet (QString name);

private:
    QString m_rootPath;
};

#endif // FILELIBRARY_H
