#ifndef CHANGETHICKNESS_H
#define CHANGETHICKNESS_H

#include <QDialog>

namespace Ui {
class ChangeThickness;
}

class ChangeThickness : public QDialog
{
    Q_OBJECT

public:
    explicit ChangeThickness(int current_val, int mode, QWidget *parent = nullptr);
    ~ChangeThickness();
    int thickness;
    int type;
private slots:
    void on_spinBox_thickness_valueChanged(int arg1);

    void on_pushButton_default_clicked();

private:
    Ui::ChangeThickness *ui;
};

#endif // CHANGETHICKNESS_H
