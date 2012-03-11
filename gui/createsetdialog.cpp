#include "createsetdialog.h"
#include "ui_createsetdialog.h"

#include <QFileDialog>

#include "model/setdao.h"

#include "image/thumbnailer.h"

CreateSetDialog::CreateSetDialog(SetDao* setDao, Thumbnailer* thumbnailer, QWidget *parent) :
    QWidget(parent, Qt::Tool),
    ui(new Ui::CreateSetDialog),
    m_setDao (setDao),
    m_imageThumbnailer (thumbnailer)
{
    ui->setupUi(this);
}

CreateSetDialog::~CreateSetDialog()
{
    delete ui;
}

void CreateSetDialog::show (void)
{
    QWidget::show();
}

void CreateSetDialog::setName (QString name)
{
    ui->nameEdit->setText( name );
}
void CreateSetDialog::setPath (QString path)
{
    ui->pathEdit->setText( path );
}

void CreateSetDialog::on_cancelButton_clicked()
{
//    ui->nameEdit->clear();
//    ui->pathEdit->clear();
//    ui->importCheckBox->setChecked(true);

    hide();
}

void CreateSetDialog::on_browseButton_clicked()
{
    ui->pathEdit->setText( QFileDialog::getExistingDirectory(this, "Open directory", "", QFileDialog::ShowDirsOnly) );
}

void CreateSetDialog::on_createButton_clicked()
{
    Set set (ui->nameEdit->text(), ui->pathEdit->text(), ui->dateEdit->text());
    m_setDao->create( set );

    // Generate thumnbails
    if (m_imageThumbnailer)
        m_imageThumbnailer->generateThumbnails (ui->pathEdit->text(), "");

    hide();

    emit setCreated ();
}
