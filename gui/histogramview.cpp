#include "histogramview.h"

#include <QPainter>

HistogramView::HistogramView(QWidget *parent) :
    QWidget(parent)
{
}

void HistogramView::draw (void)
{
    QPainter painter(this);

    painter.setCompositionMode(QPainter::CompositionMode_Source);

    QBrush brush (Qt::SolidPattern);
    brush.setColor(Qt::black);

    painter.fillRect(rect(), brush);

    int* red = m_histogram.red;
    int* green = m_histogram.green;
    int* blue = m_histogram.blue;

    float x_step = float(rect().width()) / 255;

    int height = rect().height();
    float y_step = float(height) / 255;

  //m_histogram.normalize();

    /*int red_sum = 0;
    int green_sum = 0;
    int blue_sum = 0;

    for (int i=0; i<127; i++)
        red_sum += red[i];

    for (int i=0; i<127; i++)
        green_sum += green[i];

    for (int i=0; i<127; i++)
        blue_sum += blue[i];

    int sum = red_sum;
    if (green_sum > sum)
        sum = green_sum;
    if (blue_sum > sum)
        sum = blue_sum;

    if (!sum)
        return;

    for (int i=0; i<127; i++)
    {
        red[i] = (red[i]*127*10) / sum;
        green[i] = (green[i]*127*10) / sum;
        blue[i] = (blue[i]*127*10) / sum;
    }*/

    /*int red_max = 0;
    int green_max = 0;
    int blue_max = 0;

    for (int i=0; i<127; i++)
    {
        if (red_max < red[i])
            red_max = red[i];
    }

    for (int i=0; i<127; i++)
    {
        if (green_max < green[i])
            green_max = green[i];
    }

    for (int i=0; i<127; i++)
    {
        if (blue_max < blue[i])
            blue_max = blue[i];
    }

    int max = red_max;
    if (green_max > max)
        max = green_max;
    if (blue_max > max)
        max = blue_max;*/

    painter.setPen(Qt::GlobalColor(Qt::red));
    for (int i=0; i<255; i++)
    {
        painter.drawLine (static_cast <int> (i*x_step), height-(red[i]*y_step), static_cast <int>((i+1)*x_step), height-(red[i+1]*y_step));
    }

    painter.setPen(Qt::GlobalColor(Qt::green));
    for (int i=0; i<255; i++)
    {
        painter.drawLine (static_cast <int> (i*x_step), height-(green[i]*y_step), static_cast <int>((i+1)*x_step), height-(green[i+1]*y_step));
    }

    painter.setPen(Qt::GlobalColor(Qt::blue));
    for (int i=0; i<255; i++)
    {
        painter.drawLine (static_cast <int> (i*x_step), height-(blue[i]*y_step), static_cast <int>((i+1)*x_step), height-(blue[i+1]*y_step));
    }
}

void HistogramView::clear ()
{
    QPainter painter(this);

    painter.setCompositionMode(QPainter::CompositionMode_Source);

    QBrush brush (Qt::SolidPattern);
    brush.setColor(Qt::black);

    painter.fillRect(rect(), brush);

    update();
}

void HistogramView::paintEvent (QPaintEvent * event)
{
    clear();
    draw();

    Q_UNUSED (event);
}
