#ifndef HELPDIALOG_H
#define HELPDIALOG_H

#include <QWidget>

namespace Ui {
class HelpDialog;
}

class HelpDialog : public QWidget
{
    Q_OBJECT

public:
    explicit HelpDialog(int image_count, QWidget *parent = nullptr);
    ~HelpDialog();
    void setImage(QString path);
    void onLaunch();

private slots:
    void on_pushButton_clicked();

private:
    Ui::HelpDialog *ui;
    QVector <QPixmap> help_images;
    int num_of_images;
    int index, pre_index;
};

#endif // HELPDIALOG_H
