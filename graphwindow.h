#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H

#include <QMainWindow>
#include <QContextMenuEvent>
#include <QMenu>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>
#include <QVector>
#include <QList>
#include <QGraphicsLineItem>
#include <QGraphicsItemGroup>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QColorDialog>
#include <QPen>
#include <QTableView>
#include <gsl/gsl_sf_bessel.h>
#include <gsl/gsl_fit.h>
#include <gsl/gsl_vector.h>
#include "qgraphicsaxisitem.h"
#include "adddata.h"
#include "graphoptions.h"
#include "tablemodel.h"
#include "changethickness.h"
#include "helpdialog.h"


QT_BEGIN_NAMESPACE
namespace Ui { class GraphWindow; }
QT_END_NAMESPACE

class GraphWindow : public QMainWindow
{
    Q_OBJECT

public:
    GraphWindow(QWidget *parent = nullptr);
    ~GraphWindow();
    // Handles right click events on graph.
    void contextMenuEvent(QContextMenuEvent *event);
    // Close event.
    void closeEvent(QCloseEvent *event);
    // Removes scroll bars.
    void scrollbars(bool active);
    // Clears the current graph.
    void clearGraph();
    // Update graph/scene.
    void updateScene();
    // Checks input data is valid and adds to vectors.
    bool processData(QString line_read);
    // Adds data to tables depending on which data has been entered.
    void tableFillLinear();
    void tableFillBilinear();
    // Calculates and plots the lines accordingly.
    void plotLinear();
    void plotBilinear();
    // Creates the axis, depending on the data.
    void createAxis(int num_x_ticks, int num_y_ticks, int start_x, int start_y);
    // Calculates the statistics associated with the lines, R values etc.
    void statistics(int mode, int index);
    // Changes colour of the item accordingly.
    void changeLineColour();
    void changeAxisColour();
    // Used for generating a custom graph, not recommended, instead let graph generate automatically.
    void graphOptions();
    // Updates the colour display boxes in the tool bar.
    void updateColours();
    // Used to spawn the File Ui for entering a file.
    bool inputData(int mode);
    // Used to compare r squared values for the bilinear lines.
    int rsquComparator();
    // Creates the graph and plots it.
    void buildGraph(int array_index);
    // Used for the breakpoint line.
    void x0DottedLine();
    // Error handling
    void throwErrorBox(QString error_msg);

private slots:
    void on_actionData_triggered();
    void on_actionEnable_Scrollbars_toggled(bool arg1);
    void on_pushButton_clear_clicked();
    void on_pushButton_inputfile_clicked();
    void on_actionColour_triggered();
    void on_pushButton_plot_clicked();
    void on_pushButton_clear_2_clicked();
    void on_pushButton_graphoptions_clicked();
    void on_actionCurrent_Plot_Toolbar_toggled(bool arg1);    
    void on_actionLinear_Regression_triggered();
    void on_pushButton_options_clicked();
    void on_pushButton_data_clicked();
    void on_checkBox_clicked(bool checked);
    void on_actionColour_axis_triggered();
    void on_actionGraph_Options_triggered();
    void on_actionData_Points_Colour_triggered();
    void on_actionData_Points_Size_triggered();
    void on_toolButton_axiscolour_clicked();
    void on_toolButton_linecolour_clicked();
    void on_actionThickness_triggered();
    void on_actionConstant_Term_toggled(bool arg1);
    void on_actionClear_Graph_triggered();
    void on_actionPlot_triggered();
    void on_pushButton_refresh_clicked();
    void on_pushButton_usage_clicked();
    void on_pushButton_inputbilineardata_clicked();
    void on_pushButton_plotbilineardata_clicked();
    void on_checkBox_autoupdate_toggled(bool checked);
    void on_pushButton_usage_2_clicked();
    void on_pushButton_refresh_2_clicked();
    void on_pushButton_clear_3_clicked();
    void on_checkBox_autoupdate_2_toggled(bool checked);
    void on_toolButton_linecolour_2_clicked();
    void on_toolButton_axiscolour_2_clicked();
    void on_checkBox_2_toggled(bool checked);
    void on_actionBilinear_Regression_triggered();
    void on_actionPlot_Bilinear_Line_triggered();
    void on_actionCut_Line_at_x0_triggered(bool checked);
    void on_actionLinear_triggered();
    void on_actionBilinear_triggered();
    void on_checkBox_3_clicked(bool checked);

private:
    // Vaiables for axis.
    int offset = 10;
    qreal x_pix_min, x_pix_max;
    qreal y_pix_min, y_pix_max;
    int x_ticks, y_ticks;
    int tick_increment;
    int x_startval, y_startval;
    int size_crosses;
    // Variables for linear regression plot.
    double c0[2], c1[2], cov00[2], cov01[2], cov11[2], sumsq[2];
    double x0;
    QVector <QVector <double> > r_squared;
    bool bilinear_data = false;
    bool limit_line = false;
    //
    bool auto_change = false;
    QPixmap line_pixmap = *new QPixmap(16, 16);
    QPixmap axis_pixmap = *new QPixmap(16, 16);
    // Variables for file.
    QString file_path;
    bool valid_linear_data = false;
    bool valid_bilinear_data = false;
    // Pen colour.
    QColor line_colour;
    QColor axis_colour;
    QColor points_colour;
    // Main user interface / main window.
    Ui::GraphWindow *ui;
    int line_no = 1;
    // Creates graphics scene for axis.
    QGraphicsScene *scene = new QGraphicsScene(this);
    TableModel *table_linear = new TableModel(this);
    TableModel *table_bilinear = new TableModel(this);
    // Creates axis.
    bool dotted_line = true;
    bool first_pass = true;
    QGraphicsAxisItem *x_axis;
    QGraphicsAxisItem *y_axis;
    QGraphicsItemGroup *line1;
    QGraphicsItemGroup *line2;
    QGraphicsItemGroup *x0_line;
    QGraphicsLineItem *plot_item;
    QGraphicsLineItem *point_item;
    QGraphicsLineItem *x0_item;
    bool const_term = true;
    QPen plot_pen = *new QPen();
    QPen point_pen = *new QPen();
    QPen *axis_pen = new QPen();
    int pen_thickness;
    // Pixels to data.
    qreal x_pix_to_data, y_pix_to_data;
    // Data vectors.
    QVector<double> x_data, y_data;
    QVector <double> x_plot, y_plot;
    // Error handling.
    QMessageBox *error_box = new QMessageBox();

};
#endif // GRAPHWINDOW_H
