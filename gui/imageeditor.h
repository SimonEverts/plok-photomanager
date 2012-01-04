#ifndef IMAGEEDITOR_H
#define IMAGEEDITOR_H

#include <QWidget>
#include <QString>
#include <QImage>

#include "model/capture.h"

class ImageProvider;

namespace Ui {
class ImageEditor;
}

class ImageEditor : public QWidget
{
    Q_OBJECT
    
public:
    explicit ImageEditor(ImageProvider* imageProvider, QWidget *parent = 0);
    ~ImageEditor();
    
    void setCapture (Capture capture);

private slots:

    void updateHistogram ( QImage image );
    void updateLut ( void );

    void on_imageDeveloper_currentIndexChanged(const int &currentIndex);

    void on_brightnessSlider_valueChanged(int value);

private:
    Ui::ImageEditor *ui;

    Capture m_capture;
    QString m_currentPicture;

    QImage m_workImage;

    ImageProvider* m_imageProvider;
};

#endif // IMAGEEDITOR_H
