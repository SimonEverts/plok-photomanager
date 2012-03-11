#include "imageprocessing.h"

#include <QDebug>

#include <cmath>

#ifdef OPENCV
# include <opencv/cxcore.h>
# include <opencv/cv.h>
#endif

ImageProcessing::ImageProcessing()
{
}

void ImageProcessing::createHistogram (const Image& image, Histogram& histogram)
{
    if (image.depth() == 8)
        createHistogram_8u (image, histogram);
    if (image.depth() == 16)
        createHistogram_16u (image, histogram);
}

void ImageProcessing::createHistogram_8u (const Image& image, Histogram& histogram)
{
    unsigned char* pixels = image.pixels();

    QSize size = image.size();
    unsigned int step = image.step();
    unsigned int channels = image.channels();

    unsigned int r = 0;
    unsigned int g = 0;
    unsigned int b = 0;

    int height = size.height();
    int width = size.width();

    int* red = histogram.red();
    int* green = histogram.green();
    int* blue = histogram.blue();

    if (!red || !green || !blue)
        return;

    for(int y=0; y < height; y++)
    {
        for(int x=0; x < width; x++)
        {
            unsigned int index = (y*step) + (x*channels);

            red[ pixels[index] ]++;
            green[ pixels[index+1] ]++;
            blue[ pixels[index+2] ]++;
        }
    }

    normalizeHistogram (histogram, 1 << histogram.depth());
}

void ImageProcessing::createHistogram_16u (const Image& image, Histogram& histogram)
{
    unsigned char* pixels = image.pixels();

    QSize size = image.size();
    unsigned int step = image.step();
    unsigned int channels = image.channels();

    unsigned int r = 0;
    unsigned int g = 0;
    unsigned int b = 0;

    int height = size.height();
    int width = size.width();

    int* red = histogram.red();
    int* green = histogram.green();
    int* blue = histogram.blue();

    if (!red || !green || !blue)
        return;

    for(int y=0; y < height; y++)
    {
        for(int x=0; x < width; x++)
        {
            unsigned short int* src_pixel = reinterpret_cast <unsigned short int*> (pixels + y*step) + x*channels;

            red[ src_pixel[0] ]++;
            green[ src_pixel[1] ]++;
            blue[ src_pixel[2] ]++;
        }
    }

    normalizeHistogram (histogram, 1 << histogram.depth());
}

void ImageProcessing::normalizeHistogram (Histogram& histogram, const int max_value)
{
    int* red = histogram.red();
    int* green = histogram.green();
    int* blue = histogram.blue();

    if (!red || !green || !blue)
        return;

    float max = 0;
    for (int i=1; i< max_value-1; i++)
    {
        if (red[i] > max)
            max = red[i];
        if (green[i] > max)
            max = green[i];
        if (blue[i] > max)
            max = blue[i];
    }

    max = 255.f / max;

    // TODO no inplace calc
    if (max > 0)
    {
        for (int i=0; i<max_value; i++)
        {
            red[i] = float(red[i]) * max;
            green[i] = float(green[i]) * max;
            blue[i] = float(blue[i]) * max;
        }
    }
}

void ImageProcessing::generateLut (float brightness, float contrast, float gamma, float wbRed, float wbGreen, float wbBlue, Lut& lut)
{
    int* red = lut.red();
    int* green = lut.green();
    int* blue = lut.blue();

    if (!red || !green || !blue)
        return;

    int max_value = (1 << lut.depth());

    brightness *= max_value;

    // TODO whitebalance not linear in this lut?

    for (int i=0; i<max_value; i++)
    {
        // TODO 8bit images have gamma already applied
        float gamma_value = float(max_value) * (pow ((float(i)/max_value), gamma));

        float r = (contrast * gamma_value * wbRed) + brightness;
        float g = (contrast * gamma_value * wbGreen) + brightness;
        float b = (contrast * gamma_value * wbBlue) + brightness;

// TODO werkt niet?



        if (r < 0)
            r = 0;
        if (r > (max_value-1))
            r = (max_value-1);
        if (g < 0)
            g = 0;
        if (g > (max_value-1))
            g = (max_value-1);
        if (b < 0)
            b = 0;
        if (b > (max_value-1))
            b = (max_value-1);

        red[i] = r;
        green[i] = g;
        blue[i] = b;
    }
}

void ImageProcessing::applyLut (const Image* src, Image* dest, const Lut& lut)
{
    if (src->depth() == 8 && dest->depth() == 8)
        applyLut_8u( src, dest, lut);
    if (src->depth() == 16 && dest->depth() == 8)
        applyLut_16u8u( src, dest, lut);
}

void ImageProcessing::applyLut_8u (const Image* src, Image* dest, const Lut& lut)
{
    unsigned char* src_pixels = src->pixels();
    unsigned char* dest_pixels = dest->pixels();

    // TODO assume image size and format are the same
    QSize size = src->size();
    unsigned int step = src->step();
    unsigned int channels = src->channels();

    int height = size.height();
    int width = size.width();

    int* red = lut.red();
    int* green = lut.green();
    int* blue = lut.blue();

    for(int y=0; y < height; y++)
    {
        for(int x=0; x < width; x++)
        {
            unsigned int index = (y*step) + (x*channels);

            dest_pixels[index] =  red[ src_pixels[index] ];
            dest_pixels[index+1] = green[ src_pixels[index+1] ];
            dest_pixels[index+2] = blue[ src_pixels[index+2] ];
        }
    }
}

void ImageProcessing::applyLut_16u8u (const Image* src, Image* dest, const Lut& lut)
{
    unsigned char* src_pixels = src->pixels();
    unsigned char* dest_pixels = dest->pixels();

    // TODO assume image size and format are the same
    QSize size = src->size();

    unsigned int src_step = src->step();
    unsigned int dest_step = dest->step();

    unsigned int src_channels = src->channels();
    unsigned int dest_channels = dest->channels();

    int* red = lut.red();
    int* green = lut.green();
    int* blue = lut.blue();

    int height = size.height();
    int width = size.width();

    for(int y=0; y < height; y++)
    {
        for(int x=0; x < width; x++)
        {
            unsigned int dest_index = (y*dest_step) + (x*dest_channels);

            unsigned short int* src_pixel = reinterpret_cast <unsigned short int*> (src_pixels + y*src_step) + x*src_channels;

            dest_pixels[dest_index] = red[ src_pixel[0] ] >> 8;
            dest_pixels[dest_index+1] = green[ src_pixel[1] ] >> 8;
            dest_pixels[dest_index+2] = blue[ src_pixel[2] ] >> 8;

//            dest_pixels[dest_index] = 0;
//            dest_pixels[dest_index+1] = blue[ src_pixel[2] ] >> 8;
//            dest_pixels[dest_index+2] = green[ src_pixel[1] ] >> 8;
//            dest_pixels[dest_index+3] = red[ src_pixel[0] ] >> 8;

//            dest_pixels[dest_index] = blue[ src_pixel[2] ] >> 8;
//            dest_pixels[dest_index+1] = green[ src_pixel[1] ] >> 8;
//            dest_pixels[dest_index+2] = red[ src_pixel[0] ] >> 8;
//            dest_pixels[dest_index+3] = red[ src_pixel[0] ] >> 8;

        }
    }
}

void ImageProcessing::applyGamma_16u (const Image* src, Image* dest)
{
    unsigned char* src_pixels = src->pixels();
    unsigned char* dest_pixels = dest->pixels();

    // TODO assume image size and format are the same
    QSize size = src->size();

    unsigned int src_step = src->step();
    unsigned int dest_step = dest->step();

    unsigned int src_channels = src->channels();
    unsigned int dest_channels = dest->channels();

    int height = size.height();
    int width = size.width();

    float max_value_16b = (1 << 16);
    float max_value_12b = (1 << 12);

    for(int y=0; y < height; y++)
    {
        for(int x=0; x < width; x++)
        {
            unsigned short int* src_pixel = reinterpret_cast <unsigned short int*> (src_pixels + y*src_step) + x*src_channels;
            unsigned short int* dest_pixel = reinterpret_cast <unsigned short int*> (dest_pixels + y*dest_step) + x*dest_channels;

             float r = float(src_pixel[0]) / max_value_16b;
             float g = float(src_pixel[1]) / max_value_16b;
             float b = float(src_pixel[2]) / max_value_16b;

             dest_pixel[0] = pow( r, 1.f / 2.2) * max_value_16b;
             dest_pixel[1] = pow( g, 1.f / 2.2) * max_value_16b;
             dest_pixel[2] = pow( b, 1.f / 2.2) * max_value_16b;
        }
    }
}

void ImageProcessing::applyCameraMatrix_16u (const Image* src, Image* dest)
{
    unsigned char* src_pixels = src->pixels();
    unsigned char* dest_pixels = dest->pixels();

    // TODO assume image size and format are the same
    QSize size = src->size();

    unsigned int src_step = src->step();
    unsigned int dest_step = dest->step();

    unsigned int src_channels = src->channels();
    unsigned int dest_channels = dest->channels();

    int height = size.height();
    int width = size.width();

    float max_value_16b = (1 << 16);
    float max_value_12b = (1 << 12);

    for(int y=0; y < height; y++)
    {
        for(int x=0; x < width; x++)
        {
            unsigned short int* src_pixel = reinterpret_cast <unsigned short int*> (src_pixels + y*src_step) + x*src_channels;
            unsigned short int* dest_pixel = reinterpret_cast <unsigned short int*> (dest_pixels + y*dest_step) + x*dest_channels;

             float raw1 = float(src_pixel[0]) / max_value_16b;
             float raw2 = float(src_pixel[1]) / max_value_16b;
             float raw3 = float(src_pixel[2]) / max_value_16b;

             float r = raw1 * 1.82002354 + raw2 * -0.726660609 + raw3 * -0.0933628976;
             float g = raw1 * -0.167555362 + raw2 * 1.70425475 + raw3 * -0.536699355;
             float b = raw1 * 0.000880597509 + raw2 * -0.404351801 + raw3 * 1.40347123;

//             float x = raw1 * 1.82002354 + raw2 * -0.167555362 + raw3 * 0.000880597509;
//             float y = raw1 * -0.726660609 + raw2 * 1.70425475 + raw3 * -0.404351801;
//             float z = raw1 * -0.0933628976 + raw2 * -0.536699355 + raw3 * 1.40347123;







//             float r = raw1 * 5991.f / 10000 ;//+ raw2 * -4764.f/10000 + raw3 * -707.f/10000;
//             float g = raw1 * -1456.f / 10000 + raw2 * 12135.f/10000 + raw3 * 1425.f/10000;
//             float b = raw1 * -455.f/10000 + raw2 * -2980.f/10000 + raw3 * 6701.f/10000;

//             float x = raw1 * 5991.f / 10000 + raw2 * -1456.f / 10000 + raw3 * -455.f/10000;
//             float y = raw1 * -4764.f/10000 + raw2 * 12135.f/10000 + raw3 * -2980.f/10000;
//             float z = raw1 * -707.f/10000 + raw2 * 1425.f/10000 + raw3 * 6701.f/10000;

//             float r = x *  3.2406 + y * -1.5372 + z * -0.4986;
//             float g = x * -0.9689 + y *  1.8758 + z *  0.0415;
//             float b = x *  0.0557 + y * -0.2040 + z *  1.0570;

//             if ( r > 0.0031308 )
//                 r = 1.055 * ( pow (r, ( 1 / 2.4 )) ) - 0.055;
//             else
//                 r = 12.92 * r;

//             if ( g > 0.0031308 )
//                 g = 1.055 * ( pow (g, ( 1 / 2.4 )) ) - 0.055;
//             else
//                 g = 12.92 * g;

//             if ( b > 0.0031308 )
//                 b = 1.055 * ( pow (b, ( 1 / 2.4 )) ) - 0.055;
//             else
//                 b = 12.92 * b;


             if (r < 0)
                 r = 0;
             if (g < 0)
                 g = 0;
             if (b < 0)
                 b = 0;


             dest_pixel[0] = r * max_value_16b;
             dest_pixel[1] = g * max_value_16b;
             dest_pixel[2] = b * max_value_16b;
        }
    }
}

Image ImageProcessing::fastScale (const Image& image, QSize minimumSize)
{
    QSize image_size = image.size();

    int scale = 0;
    int i = 1;
    while (((image_size.width() >> i) > minimumSize.width() ))
    {
        i++;
        scale++;
    }

    Image scaled_image;
    if (image.depth() == 8)
        scaled_image = ImageProcessing::fastScale_8u(image, scale);
    if (image.depth() == 16)
        scaled_image = ImageProcessing::fastScale_16u(image, scale);

    return scaled_image;
}

Image ImageProcessing::fastScale_8u (const Image& image, int scale)
{
    QSize scaled_size( image.size().width() >> scale,
                       image.size().height() >> scale);

    Image dest_image (scaled_size, image.channels(), 8);

    unsigned char* src_pixels = image.pixels();
    unsigned char* dest_pixels = dest_image.pixels();

    // TODO assume image size and format are the same
    QSize src_size = image.size();

    unsigned int src_step = image.step();
    unsigned int dest_step = dest_image.step();

    unsigned int src_channels = image.channels();
    unsigned int dest_channels = dest_image.channels();

    int height = scaled_size.height();
    int width = scaled_size.width();

    for(int y=0; y < height; y++)
    {
        for(int x=0; x < width; x++)
        {
            unsigned int src_index = ((y << scale)*src_step) + ((x << scale)*src_channels);
            unsigned int dest_index = (y*dest_step) + (x*dest_channels);

            dest_pixels[dest_index] =  src_pixels[src_index];
            dest_pixels[dest_index+1] = src_pixels[src_index+1];
            dest_pixels[dest_index+2] = src_pixels[src_index+2];
        }
    }

    return dest_image;
}

Image ImageProcessing::fastScale_16u (const Image& image, int scale)
{
    QSize scaled_size( image.size().width() >> scale,
                       image.size().height() >> scale);

    Image dest_image (scaled_size, image.channels(), 16);

    unsigned char* src_pixels = image.pixels();
    unsigned char* dest_pixels = dest_image.pixels();

    // TODO assume image size and format are the same
    QSize src_size = image.size();

    unsigned int src_step = image.step();
    unsigned int dest_step = dest_image.step();

    unsigned int src_channels = image.channels();
    unsigned int dest_channels = dest_image.channels();

    int height = scaled_size.height();
    int width = scaled_size.width();

    for(int y=0; y < height; y++)
    {
        for(int x=0; x < width; x++)
        {
            unsigned short int* src_pixel = reinterpret_cast <unsigned short int*> (src_pixels + (y << scale)*src_step) + (x << scale)*src_channels;
            unsigned short int* dest_pixel = reinterpret_cast <unsigned short int*> (dest_pixels + y*dest_step) + x*dest_channels;

            dest_pixel[0] = src_pixel[0];
            dest_pixel[1] = src_pixel[1];
            dest_pixel[2] = src_pixel[2];
        }
    }

    return dest_image;
}

void ImageProcessing::fastDebayer_16u (const Image& src, Image& dest)
{
    // TODO for a fast half-size debayering, to a two pass method
    // TODO use second green

    // TODO or split a row in half size of cache

    unsigned char* src_pixels = src.pixels();
    unsigned char* dest_pixels = dest.pixels();

    // TODO assume image size and format are the same
    QSize src_size = src.size();

    unsigned int src_step = src.step();
    unsigned int dest_step = dest.step();

    unsigned int src_channels = src.channels();
    unsigned int dest_channels = dest.channels();

    int height = src_size.height() / 2;
    int width = src_size.width() / 2;

    for(int y=0; y < height; y++)
    {
        for(int x=0; x < width; x++)
        {
            unsigned short int* src_pixel = reinterpret_cast <unsigned short int*> (src_pixels + y*2*src_step) + (x*2);
            unsigned short int* dest_pixel = reinterpret_cast <unsigned short int*> (dest_pixels + y*dest_step) + x*dest_channels;

            dest_pixel[0] = src_pixel[1];
            dest_pixel[1] = src_pixel[1];
        }
    }

    for(int y=0; y < height; y++)
    {
        for(int x=0; x < width; x++)
        {
            unsigned short int* src_pixel = reinterpret_cast <unsigned short int*> (src_pixels + ((y*2)+1)*src_step) + (x*2);
            unsigned short int* dest_pixel = reinterpret_cast <unsigned short int*> (dest_pixels + y*dest_step) + x*dest_channels;

            dest_pixel[2] = 0;//src_pixel[0];
        }
    }
}

void ImageProcessing::medianFilter_16u (const Image& src, Image& dest, int kernel)
{
#ifdef OPENCV
    QSize qsize = src.size();
    CvSize size = {qsize.width(), qsize.height()};

    cv::Mat src_mat (size, CV_16UC3, reinterpret_cast <char*> (src.pixels()));
    cv::Mat dest_mat (size, CV_16UC3, reinterpret_cast <char*> (dest.pixels()));

    cv::medianBlur (src_mat, dest_mat, kernel);
#endif
}

void ImageProcessing::applyProperties (const Image* src, Image* dest, float brightness, float contrast, float gamma, float wbRed, float wbGreen, float wbBlue)
{
    if (src->depth() == 8 && dest->depth() == 8)
        applyProperties_8u( src, dest, brightness, contrast, gamma, wbRed, wbGreen, wbBlue);
    if (src->depth() == 16 && dest->depth() == 8)
        applyProperties_16u8u( src, dest, brightness, contrast, gamma, wbRed, wbGreen, wbBlue);
}

void ImageProcessing::applyProperties_8u (const Image* src, Image* dest, float brightness, float contrast, float gamma, float wbRed, float wbGreen, float wbBlue )
{
    unsigned char* src_pixels = src->pixels();
    unsigned char* dest_pixels = dest->pixels();

    // TODO assume image size and format are the same
    QSize size = src->size();

    unsigned int src_step = src->step();
    unsigned int dest_step = dest->step();

    unsigned int channels = src->channels();

    int height = size.height();
    int width = size.width();

    const float norm_value = (1 << 8);

    for(int y=0; y < height; y++)
    {
        for(int x=0; x < width; x++)
        {
            unsigned int index = (y*src_step) + (x*channels);

            float r = static_cast<float> (src_pixels[index]) / norm_value;
            float g = static_cast<float> (src_pixels[index+1]) / norm_value;
            float b = static_cast<float> (src_pixels[index+2]) / norm_value;

            float x, y, z;
            convertRGBtoXYZ(r, g, b, x, y, z);

            float l, a, lab_b;
            convertXYZtoLAB(x, y, z, l, a, lab_b);

            l = l * contrast + (brightness * 100);

            a = a * wbRed * wbGreen;
            lab_b = lab_b * wbRed * wbBlue;

            convertLABtoXYZ(l, a, lab_b, x, y, z);

            convertXYZtoRGB(x, y, z, r, g, b);

            r = r * 255.f;
            g = g * 255.f;
            b = b * 255.f;


            if (r < 0)
                r = 0;
            if (r > 255)
                r = 255;
            if (g < 0)
                g = 0;
            if (g > 255)
                g = 255;
            if (b < 0)
                b = 0;
            if (b > 255)
                b = 255;

            dest_pixels[index] =  r;
            dest_pixels[index+1] = g;
            dest_pixels[index+2] = b;
        }
    }
}

void ImageProcessing::applyProperties_16u8u (const Image* src, Image* dest, float brightness, float contrast, float gamma, float wbRed, float wbGreen, float wbBlue )
{
    unsigned char* src_pixels = src->pixels();
    unsigned char* dest_pixels = dest->pixels();

    // TODO assume image size and format are the same
    QSize size = src->size();

    unsigned int src_step = src->step();
    unsigned int dest_step = dest->step();

    unsigned int channels = src->channels();

    const float norm_value = (1 << 16);

    int height = size.height();
    int width = size.width();

    for(int y=0; y < height; y++)
    {
        for(int x=0; x < width; x++)
        {
            unsigned int dest_index = (y*dest_step) + (x*channels);

            unsigned short int* src_pixel = reinterpret_cast <unsigned short int*> (src_pixels + y*src_step) + x*channels;

            float r = static_cast<float> (src_pixel[0]) / norm_value;
            float g = static_cast<float> (src_pixel[1]) / norm_value;
            float b = static_cast<float> (src_pixel[2]) / norm_value;

            float x, y, z;
            convertRGBtoXYZ(r, g, b, x, y, z);

            float l, a, lab_b;
            convertXYZtoLAB(x, y, z, l, a, lab_b);

            l = l * contrast + (brightness * 100);

            a = a * wbRed * wbGreen;
            lab_b = lab_b * wbRed * wbBlue;

            convertLABtoXYZ(l, a, lab_b, x, y, z);

            convertXYZtoRGB(x, y, z, r, g, b);

            r = r * 255.f;
            g = g * 255.f;
            b = b * 255.f;


            if (r < 0)
                r = 0;
            if (r > 255)
                r = 255;
            if (g < 0)
                g = 0;
            if (g > 255)
                g = 255;
            if (b < 0)
                b = 0;
            if (b > 255)
                b = 255;

            dest_pixels[dest_index] =  r;
            dest_pixels[dest_index+1] = g;
            dest_pixels[dest_index+2] = b;
        }
    }
}

void ImageProcessing::convertRGBtoXYZ (float r, float g, float b, float& x, float& y, float& z)
{
    if ( r > 0.04045 )
        r = pow( (( r + 0.055 ) / 1.055 ), 2.4);
    else
        r = r / 12.92;

    if ( g > 0.04045 )
        g = pow( (( g + 0.055 ) / 1.055 ), 2.4);
    else
        g = g / 12.92;

    if ( b > 0.04045 )
        b = pow( (( b + 0.055 ) / 1.055 ), 2.4);
    else
        b = b / 12.92;

    r = r * 100.f;
    g = g * 100.f;
    b = b * 100.f;

    //Observer. = 2°, Illuminant = D65
    x = r * 0.4124 + g * 0.3576 + b * 0.1805;
    y = r * 0.2126 + g * 0.7152 + b * 0.0722;
    z = r * 0.0193 + g * 0.1192 + b * 0.9505;
}

void ImageProcessing::convertXYZtoRGB (float x, float y, float z, float& r, float& g, float& b)
{
    x = x / 100.f;        //X from 0 to  95.047      (Observer = 2°, Illuminant = D65)
    y = y / 100.f;        //Y from 0 to 100.000
    z = z / 100.f;        //Z from 0 to 108.883

    r = x *  3.2406 + y * -1.5372 + z * -0.4986;
    g = x * -0.9689 + y *  1.8758 + z *  0.0415;
    b = x *  0.0557 + y * -0.2040 + z *  1.0570;

    if ( r > 0.0031308 )
        r = 1.055 * ( pow (r, ( 1 / 2.4 )) ) - 0.055;
    else
        r = 12.92 * r;

    if ( g > 0.0031308 )
        g = 1.055 * ( pow (g, ( 1 / 2.4 )) ) - 0.055;
    else
        g = 12.92 * g;

    if ( b > 0.0031308 )
        b = 1.055 * ( pow (b, ( 1 / 2.4 )) ) - 0.055;
    else
        b = 12.92 * b;
}

void ImageProcessing::convertLABtoXYZ (float l, float a, float b, float& x, float& y, float& z)
{
    y = ( l + 16.f ) / 116.f;
    x = (a / 500.f) + y;
    z = y - (b / 200.f);

    if ( pow (y,3) > 0.008856 )
        y= pow(y,3);
    else
        y = ( y - (16.f / 116.f) ) / 7.787;

    if ( pow (x,3) > 0.008856 )
        x = pow (x, 3);
    else
        x = ( x - (16.f / 116.f) ) / 7.787;

    if ( pow (z,3) > 0.008856 )
        z = pow(z, 3);
    else
        z = ( z - (16.f / 116.f) ) / 7.787;

    x = 95.047 * x;     //ref_X =       Observer= 2°, Illuminant= D65
    y = 100.000 * y;     //ref_Y =
    z = 108.883 * z;     //ref_Z =
}

void ImageProcessing::convertXYZtoLAB (float x, float y, float z, float& l, float& a, float& b)
{
    x = x / 95.047;          //ref_X =     Observer= 2°, Illuminant= D65
    y = y / 100.000;          //ref_Y =
    z = z / 108.883;          //ref_Z =

    if ( x > 0.008856 )
        x = pow (x, ( 1.f/3.f ));
    else
        x = ( 7.787 * x ) + ( 16.f / 116.f );

    if ( y > 0.008856 )
        y = pow (y, ( 1.f/3.f ));
    else
        y = ( 7.787 * y ) + ( 16.f / 116.f );

    if ( z > 0.008856 )
        z = pow (z, ( 1.f/3.f ));
    else
        z = ( 7.787 * z ) + ( 16.f / 116.f );

    l = ( 116.f * y ) - 16;
    a = 500.f * ( x - y );
    b = 200.f * ( y - z );
}
