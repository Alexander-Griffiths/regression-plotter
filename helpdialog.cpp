#include "helpdialog.h"
#include "ui_helpdialog.h"

HelpDialog::HelpDialog(int image_count, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HelpDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    num_of_images = image_count;
    help_images.resize(num_of_images);
    pre_index = 0;
    index = 0;
}

HelpDialog::~HelpDialog()
{
    delete ui;
}

void HelpDialog::setImage(QString path)
{
    help_images[pre_index] = QPixmap(path);
    pre_index++;
}

void HelpDialog::onLaunch()
{
    ui->label->setPixmap(help_images[0]);
    if (index == num_of_images-1)
        ui->pushButton->setVisible(false);
}

void HelpDialog::on_pushButton_clicked()
{
    index++;
    ui->label->setPixmap(help_images[index]);
    if (index == num_of_images-1)
        ui->pushButton->setVisible(false);
}
