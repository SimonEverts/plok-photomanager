#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include "image.h"

struct Histogram {
    int red[ 0x10000 ];
    int green[ 0x10000 ];
    int blue[ 0x10000 ];
};

struct Lut {
    int red[ 0x10000 ];
    int green[ 0x10000 ];
    int blue[ 0x10000 ];
};

class ImageProcessing
{
public:
    ImageProcessing();

    static void createHistogram (const Image& image, Histogram& histogram);

    static void normalizeHistogram (Histogram& histogram, const int max_value);

    static void applyLut (Image* src, Image* dest, const Lut& lut);
    static Image fastScale (const Image& image, QSize minimumSize);

private:
    static void createHistogram_8u (const Image& image, Histogram& histogram);
    static void createHistogram_16u (const Image& image, Histogram& histogram);

    static void applyLut_8u (Image* src, Image* dest, const Lut& lut);
    static void applyLut_16u8u (Image* src, Image* dest, const Lut& lut);

    static Image fastScale_8u (const Image& image, int scale);
    static Image fastScale_16u (const Image& image, int scale);
};

#endif // IMAGEPROCESSING_H
