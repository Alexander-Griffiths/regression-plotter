#include "graphoptions.h"
#include "ui_graphoptions.h"

GraphOptions::GraphOptions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GraphOptions)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    // Axis variables
    // Axis variables
    this->x_pix_min = -400;
    this->x_pix_max = 400;
    this->y_pix_min = -400;
    this->y_pix_max = 400;
    this->x_startval = -5;
    this->y_startval = -5;
    this->tick_increment = 1;
    this->x_ticks = 10;
    this->y_ticks = 10;
    defaultValues();

}

GraphOptions::~GraphOptions()
{
    delete ui;
}

void GraphOptions::defaultValues()
{

    ui->spinBox_xmin->setValue(x_pix_min);
    ui->spinBox_xmax->setValue(x_pix_max);
    ui->spinBox_ymin->setValue(y_pix_min);
    ui->spinBox_ymax->setValue(y_pix_max);
    ui->spinBox_xstart->setValue(x_startval);
    ui->spinBox_ystart->setValue(y_startval);
    ui->spinBox_increments->setValue(tick_increment);
    ui->spinBox_xnumticks->setValue(x_ticks);
    ui->spinBox_ynumticks->setValue(y_ticks);

}

void GraphOptions::closeEvent(QCloseEvent *)
{
    x_pix_min = ui->spinBox_xmin->value();
    x_pix_max = ui->spinBox_xmax->value();
    y_pix_max = ui->spinBox_ymax->value();
    y_pix_min = ui->spinBox_ymin->value();
    x_ticks = ui->spinBox_xnumticks->value();
    y_ticks = ui->spinBox_ynumticks->value();
    tick_increment = ui->spinBox_increments->value();
    x_startval = ui->spinBox_xstart->value();
    y_startval = ui->spinBox_ystart->value();
}

void GraphOptions::on_buttonBox_accepted()
{
    x_pix_min = ui->spinBox_xmin->value();
    x_pix_max = ui->spinBox_xmax->value();
    y_pix_max = ui->spinBox_ymax->value();
    y_pix_min = ui->spinBox_ymin->value();
    x_ticks = ui->spinBox_xnumticks->value();
    y_ticks = ui->spinBox_ynumticks->value();
    tick_increment = ui->spinBox_increments->value();
    x_startval = ui->spinBox_xstart->value();
    y_startval = ui->spinBox_ystart->value();
}
