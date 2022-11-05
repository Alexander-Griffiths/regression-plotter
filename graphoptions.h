#ifndef GRAPHOPTIONS_H
#define GRAPHOPTIONS_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class GraphOptions;
}

class GraphOptions : public QDialog
{
    Q_OBJECT

public:
    explicit GraphOptions(QWidget *parent = nullptr);
    ~GraphOptions();
    void defaultValues();
    void closeEvent(QCloseEvent *);
    qreal x_pix_min, x_pix_max;
    qreal y_pix_min, y_pix_max;
    int x_ticks, y_ticks;
    int tick_increment;
    int x_startval, y_startval;
private slots:
    void on_buttonBox_accepted();

private:
    Ui::GraphOptions *ui;

};

#endif // GRAPHOPTIONS_H
