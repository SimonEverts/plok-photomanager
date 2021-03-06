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

    int* red = m_histogram.red();
    int* green = m_histogram.green();
    int* blue = m_histogram.blue();

    if (!red || !green || !blue)
        return;

    int max_value = 1 << m_histogram.depth();
    if (!max_value)
        return;

    float x_step = float(rect().width()) / max_value;

    int height = rect().height();
    float y_step = float(height) / 255;

    painter.setPen(Qt::GlobalColor(Qt::red));
    for (int i=0; i<max_value; i++)
    {
        painter.drawLine (static_cast <int> (i*x_step), height-(red[i]*y_step), static_cast <int>((i+1)*x_step), height-(red[i+1]*y_step));
    }

    painter.setPen(Qt::GlobalColor(Qt::green));
    for (int i=0; i<max_value; i++)
    {
        painter.drawLine (static_cast <int> (i*x_step), height-(green[i]*y_step), static_cast <int>((i+1)*x_step), height-(green[i+1]*y_step));
    }

    painter.setPen(Qt::GlobalColor(Qt::blue));
    for (int i=0; i<max_value; i++)
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
