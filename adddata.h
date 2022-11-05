#ifndef ADDDATA_H
#define ADDDATA_H

#include <QWidget>

namespace Ui {
class AddData;
}

class AddData : public QWidget
{
    Q_OBJECT

public:
    explicit AddData(QWidget *parent = nullptr);
    ~AddData();
    // Variables.
    double start_value = 0;
    double end_value = 0;
    int number_of_values = 0;
    int function_order = 1;

private slots:
    void on_pushButton_Plot_clicked();

private:
    Ui::AddData *ui;
};

#endif // ADDDATA_H
