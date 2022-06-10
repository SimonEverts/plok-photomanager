#ifndef HISTOGRAMVIEW_H
#define HISTOGRAMVIEW_H

#include <QWidget>

#include "image/imageprocessing.h"

class HistogramView : public QWidget
{
    Q_OBJECT
public:
    explicit HistogramView(QWidget *parent = 0);

    void draw (void);
    void clear (void);

    QSize sizeHint (void) const {return QSize(254, 250);}

    Histogram histogram (void) {return m_histogram;}
    void setHistogram (const Histogram& histogram) {m_histogram = histogram; update();}

protected:
    void paintEvent (QPaintEvent * event);

private:
    Histogram m_histogram;

signals:
    
public slots:
    
};

#endif // HISTOGRAMVIEW_H
