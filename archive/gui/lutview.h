#ifndef LUTVIEW_H
#define LUTVIEW_H

#include <QWidget>

#include "image/imageprocessing.h"

class LutView : public QWidget
{
    Q_OBJECT
public:
    explicit LutView(QWidget *parent = 0);

    void draw (void);
    void clear (void);

    QSize sizeHint (void) const {return QSize(255, 255);}

    Lut lut (void) {return m_lut;}
    void setLut (const Lut& lut) {m_lut = lut; update();}

protected:
    void paintEvent (QPaintEvent * event);

private:
    Lut m_lut;
signals:
    
public slots:
    
};

#endif // LUTVIEW_H
