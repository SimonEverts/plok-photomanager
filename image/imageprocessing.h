#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include "image.h"

#include "lut.h"

typedef Lut Histogram;

class ImageProcessing
{
public:
    ImageProcessing();

    static void createHistogram (const Image& image, Histogram& histogram);    
    static void normalizeHistogram (Histogram& histogram, const int max_value);

    static void generateLut (float brightness, float contrast, float gamma, Lut& lut);
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
