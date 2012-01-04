#include "imageprocessing.h"

#include <QDebug>

ImageProcessing::ImageProcessing()
{
}

void ImageProcessing::createHistogram (QImage* image, Histogram& histogram)
{
    unsigned char* pixels = image->bits();

    QSize size = image->size();
    unsigned int step = image->bytesPerLine();
    unsigned int channels = 4;

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

    int max = 0;
    for (int i=0; i<255; i++)
    {
        if (histogram.red[i] > max)
            max = histogram.red[i];
        if (histogram.green[i] > max)
            max = histogram.green[i];
        if (histogram.blue[i] > max)
            max = histogram.blue[i];
    }

    max /= 255;

    // TODO no inplace calc
    if (max > 0)
    {
        for (int i=0; i<255; i++)
        {
            histogram.red[i] /= max;
            histogram.green[i] /= max;
            histogram.blue[i] /= max;
        }
    }
}

void ImageProcessing::adjustBrightness (QImage* src, QImage* dest, int brightnessOffset)
{
    unsigned char* src_pixels = src->bits();
    unsigned char* dest_pixels = dest->bits();

    // TODO assume image size and format are the same
    QSize size = src->size();
    unsigned int step = src->bytesPerLine();
    unsigned int channels = 4;

    int height = size.height();
    int width = size.width();

    for(int y=0; y < height; y++)
    {
        for(int x=0; x < width; x++)
        {
            unsigned int index = (y*step) + (x*channels);

            dest_pixels[index] = src_pixels[index] + brightnessOffset;
            dest_pixels[index+1] = src_pixels[index+1] + brightnessOffset;
            dest_pixels[index+2] = src_pixels[index+2] + brightnessOffset;
        }
    }
}

void ImageProcessing::applyLut(QImage* src, QImage* dest, const Lut& lut)
{
    unsigned char* src_pixels = src->bits();
    unsigned char* dest_pixels = dest->bits();

    // TODO assume image size and format are the same
    QSize size = src->size();
    unsigned int step = src->bytesPerLine();
    unsigned int channels = 4;

    int height = size.height();
    int width = size.width();

    for(int y=0; y < height; y++)
    {
        for(int x=0; x < width; x++)
        {
            unsigned int index = (y*step) + (x*channels);

            dest_pixels[index] =  lut.red[ src_pixels[index] ];
            dest_pixels[index+1] = lut.red[ src_pixels[index+1] ];
            dest_pixels[index+2] = lut.red[ src_pixels[index+2] ];
        }
    }
}
