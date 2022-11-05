#include "adddata.h"
#include "ui_adddata.h"

AddData::AddData(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddData)
{
    ui->setupUi(this);
}

AddData::~AddData()
{
    delete ui;
}

void AddData::on_pushButton_Plot_clicked()
{
    start_value = ui->doubleSpinBox_startval->value();
    end_value = ui->doubleSpinBox_endval->value();
    number_of_values = ui->spinBox_num_val->value();
    function_order = ui->spinBox_func_order->value();
}
