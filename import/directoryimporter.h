#ifndef DIRECTORYIMPORTER_H
#define DIRECTORYIMPORTER_H

#include "model/capture.h"

#include <QList>

class ImageProvider;

class DirectoryImporter
{
public:
    DirectoryImporter(ImageProvider *imageProvider);

    QList<Capture> importCapturesFromDir (QString dirName);

private:
    ImageProvider* m_imageProvider;
};

#endif // DIRECTORYIMPORTER_H
