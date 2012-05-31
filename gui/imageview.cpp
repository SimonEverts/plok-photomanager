#include "imageview.h"

#include <QPainter>
#include <QPaintEngine>

#include <QMouseEvent>

#include <QDebug>

ImageView::ImageView(QWidget *parent) :
    QWidget(parent)
{
    setMouseTracking(true);
}

void ImageView::setImage (QImage image)
{
    m_orig_image = image;

    resizeImage ();

    update();
}

void ImageView::paintEvent(QPaintEvent *event)
{
    draw ();
}

void ImageView::resizeEvent(QResizeEvent *)
{
    resizeImage();

    update();
}

void ImageView::draw (void)
{
    QPainter painter(this);
    painter.setCompositionMode(QPainter::CompositionMode_Source);

    QBrush brush (Qt::SolidPattern);
    brush.setColor (Qt::darkGray);

    painter.fillRect(rect(), brush);

    painter.drawImage (rect(), m_scaled_image, rect());
}

void ImageView::resizeImage(void)
{
    QSize orig_image_size = m_orig_image.size();

    double hscale = double(rect().width()) / orig_image_size.width();
    double vscale = double(rect().height()) / orig_image_size.height();

    m_scaleFactor = (hscale < vscale) ? hscale : vscale;

    QSize scaled_image_size;
    scaled_image_size.setWidth( static_cast <int> (m_scaleFactor * orig_image_size.width() ));
    scaled_image_size.setHeight( static_cast <int> (m_scaleFactor * orig_image_size.height() ));

    // TODO Fast on preview
    m_scaled_image = m_orig_image.scaled (scaled_image_size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}

void ImageView::mouseMoveEvent ( QMouseEvent * event )
{
    QRgb pixel = m_scaled_image.pixel( event->pos() );

    qDebug() << qRed(pixel) << qGreen(pixel) << qBlue(pixel);
}
