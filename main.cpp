#include "graphwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GraphWindow w;
    a.setWindowIcon(QIcon(":/icons/img/scattericon.png"));
    w.setWindowState(Qt::WindowMaximized);
    w.show();
    return a.exec();
}
