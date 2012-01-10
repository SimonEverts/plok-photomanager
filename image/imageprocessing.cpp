#include "imageprocessing.h"

#include <QDebug>

ImageProcessing::ImageProcessing()
{
}

void ImageProcessing::createHistogram_8u (Image* image, Histogram& histogram)
{
    unsigned char* pixels = image->pixels();

    QSize size = image->size();
    unsigned int step = image->step();
    unsigned int channels = image->channels();

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

void ImageProcessing::createHistogram_16u (Image* image, Histogram& histogram)
{
    short int* pixels = reinterpret_cast <short int*> (image->pixels());

    QSize size = image->size();
    unsigned int step = image->step();
    unsigned int channels = image->channels();

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

void ImageProcessing::applyLut_16u (Image* src, Image* dest, const Lut& lut)
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
