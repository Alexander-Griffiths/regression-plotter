#ifndef ADDNEWDATA_H
#define ADDNEWDATA_H

#include <QDialog>

namespace Ui {
class AddNewData;
}

class AddNewData : public QDialog
{
    Q_OBJECT

public:
    explicit AddNewData(double start_value, double end_value, int number_of_values, int function_order, QWidget *parent = nullptr);
    ~AddNewData();
    void closeEvent(QCloseEvent *event);

    // Variables.
    double start_value;
    double end_value;
    int number_of_values;
    int function_order;
    int index;

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::AddNewData *ui;

};

#endif // ADDNEWDATA_H
