#ifndef IMAGEEDITOR_H
#define IMAGEEDITOR_H

#include <QWidget>
#include <QString>
#include <QImage>

#include "model/capture.h"
#include "model/picture.h"

class ImageProvider;
class PictureDao;

namespace Ui {
class ImageEditor;
}

class ImageEditor : public QWidget
{
    Q_OBJECT
    
public:
    explicit ImageEditor(ImageProvider* imageProvider, PictureDao* pictureDao, QWidget *parent = 0);
    ~ImageEditor();

    void blockSignals (bool block);
    
    void setCapture (Capture capture);

private slots:

    void updateHistogram ( const Image& image );
    void updateLut ( void );

    void loadPicture (void);

    void guiChanged (void);

    void on_imageDeveloper_currentIndexChanged(const int &currentIndex);

    void on_saveButton_clicked();

private:
    void loadGUI_pictureProperties (PictureProperties pictureAdjustments);

    Picture& currentPicture (void);
    Image& currentImage (void);

    Ui::ImageEditor *ui;

    Capture m_capture;
    QString m_currentPicture;

    Image m_workJpegMaster;
    Image m_workRawMaster;

    ImageProvider* m_imageProvider;
    PictureDao* m_pictureDao;
};

#endif // IMAGEEDITOR_H
