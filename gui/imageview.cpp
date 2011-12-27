#include "imageview.h"

#include <QPainter>
#include <QPaintEngine>

ImageView::ImageView(QWidget *parent) :
    QWidget(parent)
{
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

    // TODO use paintEngine here?

#ifdef WIN32
    painter.paintEngine()->drawImage (rect(), m_scaled_image, rect());
#else
    painter.drawImage (rect(), m_scaled_image, rect());
#endif
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
