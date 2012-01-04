#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include <QImage>

struct Histogram {
    int red[255];
    int green[255];
    int blue[255];
};

struct Lut {
    int red[255];
    int green[255];
    int blue[255];
};

class ImageProcessing
{
public:
    ImageProcessing();

    static void createHistogram (QImage* image, Histogram& histogram);

    static void adjustBrightness (QImage* src, QImage* dest, int brightnessOffset);

    static void applyLut (QImage* src, QImage* dest, const Lut& lut);
};

#endif // IMAGEPROCESSING_H
