#ifndef EXIFLOADER_H
#define EXIFLOADER_H

#include <QString>
#include <QMap>
#include <QVariant>

class ExifLoader
{
public:
    ExifLoader();

    QMap <QString, QVariant> loadInfo (QString path);
};

#endif // EXIFLOADER_H
