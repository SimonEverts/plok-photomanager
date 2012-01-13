#include "imageprocessing.h"

#include <QDebug>

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

    for(int y=0; y < height; y++)
    {
        for(int x=0; x < width; x++)
        {
            unsigned int index = (y*step) + (x*channels);

            histogram.red[ pixels[index] ]++;
            histogram.green[ pixels[index+1] ]++;
            histogram.blue[ pixels[index+2] ]++;
        }
    }

    normalizeHistogram (histogram, 0x100);
}

void ImageProcessing::createHistogram_16u (const Image& image, Histogram& histogram)
{
    short int* pixels = reinterpret_cast <short int*> (image.pixels());

    QSize size = image.size();
    unsigned int step = image.step();
    unsigned int channels = image.channels();

    unsigned int r = 0;
    unsigned int g = 0;
    unsigned int b = 0;

    int height = size.height();
    int width = size.width();

    for(int y=0; y < height; y++)
    {
        for(int x=0; x < width; x++)
        {
            unsigned int index = (y*step) + (x*channels);

            histogram.red[ pixels[index] ]++;
            histogram.green[ pixels[index+1] ]++;
            histogram.blue[ pixels[index+2] ]++;
        }
    }

    normalizeHistogram (histogram, 0x10000);
}

void ImageProcessing::normalizeHistogram (Histogram& histogram, const int max_value)
{
    int max = 0;
    for (int i=0; i< max_value; i++)
    {
        if (histogram.red[i] > max)
            max = histogram.red[i];
        if (histogram.green[i] > max)
            max = histogram.green[i];
        if (histogram.blue[i] > max)
            max = histogram.blue[i];
    }

    max /= max_value;

    // TODO no inplace calc
    if (max > 0)
    {
        for (int i=0; i<max_value; i++)
        {
            histogram.red[i] /= max;
            histogram.green[i] /= max;
            histogram.blue[i] /= max;
        }
    }
}

void ImageProcessing::applyLut (Image* src, Image* dest, const Lut& lut)
{
    if (src->depth() == 8 && dest->depth() == 8)
        applyLut_8u( src, dest, lut);
    if (src->depth() == 16 && dest->depth() == 8)
        applyLut_16u8u( src, dest, lut);
}

void ImageProcessing::applyLut_8u (Image* src, Image* dest, const Lut& lut)
{
    unsigned char* src_pixels = src->pixels();
    unsigned char* dest_pixels = dest->pixels();

    // TODO assume image size and format are the same
    QSize size = src->size();
    unsigned int step = src->step();
    unsigned int channels = src->channels();

    int height = size.height();
    int width = size.width();

    for(int y=0; y < height; y++)
    {
        for(int x=0; x < width; x++)
        {
            unsigned int index = (y*step) + (x*channels);

            dest_pixels[index] =  lut.red[ src_pixels[index] ];
            dest_pixels[index+1] = lut.green[ src_pixels[index+1] ];
            dest_pixels[index+2] = lut.blue[ src_pixels[index+2] ];
        }
    }
}

void ImageProcessing::applyLut_16u8u (Image* src, Image* dest, const Lut& lut)
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

    for(int y=0; y < height; y++)
    {
        for(int x=0; x < width; x++)
        {
            //unsigned int src_index = (y*src_step) + (x*channels);
            unsigned int dest_index = (y*dest_step) + (x*channels);

            unsigned short int* src_pixel = reinterpret_cast <unsigned short int*> (src_pixels + y*src_step) + x*channels;

            dest_pixels[dest_index] =  lut.red[ src_pixel[0] ];
            dest_pixels[dest_index+1] = lut.green[ src_pixel[1] ];
            dest_pixels[dest_index+2] = lut.blue[ src_pixel[2] ];
        }
    }
}

Image ImageProcessing::fastScale (const Image& image, QSize minimumSize)
{
    QSize image_size = image.size();

    int scale = 1;

    while (((image_size.width()  >> scale) > minimumSize.width() ))
        scale++;

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

    Image dest_image (scaled_size, image.channels(), 16); // TODO depth decouple from channels

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
