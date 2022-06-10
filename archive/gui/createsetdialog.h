#ifndef CREATESETDIALOG_H
#define CREATESETDIALOG_H

#include <QWidget>

namespace Ui {
class CreateSetDialog;
}

class SetDao;
class Thumbnailer;

class CreateSetDialog : public QWidget
{
    Q_OBJECT
    
public:
    explicit CreateSetDialog(SetDao* setDao, Thumbnailer* thumbnailer, QWidget *parent = 0);
    ~CreateSetDialog();
    
    void show (void);

    void setName (QString name);
    void setPath (QString path);

private slots:
    void on_cancelButton_clicked();

    void on_browseButton_clicked();

    void on_createButton_clicked();

signals:
    void setCreated (void);

private:
    Ui::CreateSetDialog *ui;

    SetDao* m_setDao;

    Thumbnailer* m_imageThumbnailer;
};

#endif // CREATESETDIALOG_H
