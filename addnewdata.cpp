#include "addnewdata.h"
#include "ui_addnewdata.h"

AddNewData::AddNewData(double start_value, double end_value, int number_of_values, int function_order, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddNewData)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    ui->doubleSpinBox_startval->setValue(start_value);
    ui->doubleSpinBox_endval->setValue(end_value);
    ui->spinBox_numofval->setValue(number_of_values);
    ui->spinBox_functorder->setValue(function_order);

}

AddNewData::~AddNewData()
{
    delete ui;
}

void AddNewData::closeEvent(QCloseEvent *)
{
    start_value = 0;
    end_value = 0;
    number_of_values = 0;
    function_order = 0;
}

void AddNewData::on_buttonBox_accepted()
{
    this->start_value = ui->doubleSpinBox_startval->value();
    this->end_value = ui->doubleSpinBox_endval->value();
    this->number_of_values = ui->spinBox_numofval->value();
    this->function_order = ui->spinBox_functorder->value();
    index = ui->comboBox_functype->currentIndex();
}

void AddNewData::on_buttonBox_rejected()
{
    start_value = 0;
    end_value = 0;
    number_of_values = 0;
    function_order = 0;
}
