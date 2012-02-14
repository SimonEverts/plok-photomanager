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

    static void generateLut (float brightness, float contrast, float gamma, float wbRed, float wbGreen, float wbBlue, Lut& lut);
    static void applyLut (const Image* src, Image* dest, const Lut& lut);

    static Image fastScale (const Image& image, QSize minimumSize);

    static void fastDebayer_16u (const Image& src, Image& dest);

    static void medianFilter_16u (const Image& src, Image& dest, int kernel);

    static void applyProperties (const Image* src, Image* dest, float brightness, float contrast, float gamma, float wbRed, float wbGreen, float wbBlue);

private:
    static void createHistogram_8u (const Image& image, Histogram& histogram);
    static void createHistogram_16u (const Image& image, Histogram& histogram);

    static void applyLut_8u (const Image* src, Image* dest, const Lut& lut);
    static void applyLut_16u8u (const Image* src, Image* dest, const Lut& lut);

    static Image fastScale_8u (const Image& image, int scale);
    static Image fastScale_16u (const Image& image, int scale);

    static void applyProperties_8u (const Image* src, Image* dest, float brightness, float contrast, float gamma, float wbRed, float wbGreen, float wbBlue);
    static void applyProperties_16u8u (const Image* src, Image* dest, float brightness, float contrast, float gamma, float wbRed, float wbGreen, float wbBlue);

    static void convertRGBtoXYZ (float r, float g, float b, float& x, float& y, float& z);
    static void convertXYZtoRGB (float x, float y, float z, float& r, float& g, float& b);

    static void convertLABtoXYZ (float l, float a, float b, float& x, float& y, float& z);
    static void convertXYZtoLAB (float x, float y, float z, float& l, float& a, float& b);
};

#endif // IMAGEPROCESSING_H
