#include "changethickness.h"
#include "ui_changethickness.h"

ChangeThickness::ChangeThickness(int current_val, int mode, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangeThickness)
{
    type = mode;
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    thickness = current_val;
    ui->spinBox_thickness->setValue(thickness);
}

ChangeThickness::~ChangeThickness()
{
    delete ui;
}

void ChangeThickness::on_spinBox_thickness_valueChanged(int arg1)
{
    thickness = arg1;
}

void ChangeThickness::on_pushButton_default_clicked()
{
    if (type) thickness = 3;
    else thickness = 1;
    ui->spinBox_thickness->setValue(thickness);
}
