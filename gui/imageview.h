#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QWidget>

class ImageView : public QWidget
{
    Q_OBJECT
public:
    explicit ImageView(QWidget *parent = 0);

    void setImage (QImage image);

    QImage scaledImage (void) {return m_scaled_image;}

protected:
    void paintEvent(QPaintEvent* event);
    void resizeEvent(QResizeEvent* event);

    void mouseMoveEvent ( QMouseEvent * event );

signals:

public slots:


private:
    void draw (void);
    void resizeImage (void);

    QImage m_orig_image;
    QImage m_scaled_image;

    double m_scaleFactor;
};

#endif // IMAGEVIEW_H
