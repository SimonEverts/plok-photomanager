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
            red[i] *= max;
            green[i] *= max;
            blue[i] *= max;
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

    for (int i=0; i<max_value; i++)
    {
        // TODO 8bit images have gamma already applied
        float gamma_value = float(max_value) * (pow ((float(i)/max_value), gamma));

        float value = (contrast*gamma_value + brightness);

        float r = value * wbRed;
        float g = value * wbGreen;
        float b = value * wbBlue;

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

    unsigned int channels = src->channels();

    int* red = lut.red();
    int* green = lut.green();
    int* blue = lut.blue();

    int height = size.height();
    int width = size.width();

    for(int y=0; y < height; y++)
    {
        for(int x=0; x < width; x++)
        {
            unsigned int dest_index = (y*dest_step) + (x*channels);

            unsigned short int* src_pixel = reinterpret_cast <unsigned short int*> (src_pixels + y*src_step) + x*channels;

            dest_pixels[dest_index] =  red[ src_pixel[0] ] >> 8;
            dest_pixels[dest_index+1] = green[ src_pixel[1] ] >> 8;
            dest_pixels[dest_index+2] = blue[ src_pixel[2] ] >> 8;
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
