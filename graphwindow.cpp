#include "graphwindow.h"
#include "ui_graphwindow.h"
#include "addnewdata.h"

GraphWindow::GraphWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GraphWindow)
{
    ui->setupUi(this);
    x_startval = 0;                         // X start value for the axis.
    y_startval = 0;                         // Y start value for the axis.
    size_crosses = 3;                       // Used for the size of the data points.
    ui->dockWidget->setMinimumWidth(240);   // Restricts dock width min.
    ui->dockWidget->setMaximumWidth(240);   // Restricts dock width max.
    scrollbars(false);                      // Disable scrollbars by default.
    setCentralWidget(ui->graphicsView);     // Sets graphics view as main widget.
    ui->graphicsView->setScene(scene);      // Sets the scene.
    line1 = new QGraphicsItemGroup();       // Line 1 item group. (linear and bilinear)
    line2 = new QGraphicsItemGroup();       // Line 2 item group. (only bilinear)
    x0_line = new QGraphicsItemGroup();     // Dotted line that shows at breakpoint.
    plot_item = new QGraphicsLineItem();    // Used to plot the line in segments.
    point_item = new QGraphicsLineItem();   // Used to plot the points X's.
    x0_item = new QGraphicsLineItem();      // Used for drawing the breakpoint line.
    plot_pen.setColor(Qt::black);           // Sets line colour.
    axis_pen->setColor(Qt::black);          // Sets axis colour.
    point_pen.setColor(Qt::black);          // Sets point colour.
    pen_thickness = 1;
    plot_pen.setWidth(pen_thickness);
    updateColours();
}

GraphWindow::~GraphWindow()
{
    delete ui;
}

void GraphWindow::contextMenuEvent(QContextMenuEvent *event)
{
    // Called when right click occurs on app.
    QMenu menu(this);
    QMenu* submenuAddData = menu.addMenu("Add Data");
    QMenu* submenuPlot = menu.addMenu("Plot Type");
    submenuPlot->addAction(ui->actionPlot);
    submenuAddData->addAction(ui->actionLinear_Regression);
    submenuPlot->addAction(ui->actionPlot_Bilinear_Line);
    submenuAddData->addAction(ui->actionBilinear_Regression);
    menu.addAction(ui->actionThickness);
    menu.addAction(ui->actionColour);
    menu.exec(event->globalPos());
}

void GraphWindow::closeEvent(QCloseEvent *event)
{
    // Handles close event, if X is pressed this prompt comes up.
    if (QMessageBox::Yes != QMessageBox::question(this, "Close Confirmation", "Are you sure you want to exit the program?", QMessageBox::Yes | QMessageBox::No))
    {
        event->ignore();
    }
}

void GraphWindow::scrollbars(bool active)
{
    // Used to toggle scrollbars on or off.
    if (active)
    {
        ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    }
    else
    {
        ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }
}

void GraphWindow::clearGraph()
{
    // Used to remove graph data depending on which data is currently present.
    if (!bilinear_data) scene->removeItem(line1);
    else
    {
        scene->removeItem(line1);
        scene->removeItem(line2);
        if (dotted_line) scene->removeItem(x0_line);
    }
    ui->graphicsView->update();
}

void GraphWindow::updateScene()
{
    // Updates scene.
    ui->graphicsView->update();
    ui->graphicsView->repaint();
}

void GraphWindow::on_actionEnable_Scrollbars_toggled(bool arg1)
{
    // Toggles scrollbars.
    scrollbars(arg1);
}

void GraphWindow::on_pushButton_inputfile_clicked()
{
    // Inputs data and if valid data is present populates the table.
    if(inputData(1))
        tableFillLinear();
}

bool GraphWindow::processData(QString line_read)
{
    // Processes data from line_read input.
    QStringList tokens; // splits the line into tokens, x and y.
    bool valid_double = true;
    // Splits the line using delimiter ",".
    tokens = line_read.split(QRegExp(","));
    // Error handler.
    if(tokens.size() == 1)
    {
        throwErrorBox("Invalid input data, x and y values must be provided.");
        return false;
    }
    // Iterates through the tokens, filling x and y vectors.
    for (int i = 0; i < tokens.size();i+=2)
    {
        try
        {

            x_data.push_back(tokens.at(i).toDouble(&valid_double));
            y_data.push_back(tokens.at(i+1).toDouble(&valid_double));
        }  catch (std::exception&)
        {
            throwErrorBox("Error when input data is converted to type double. Ensure only numbers are in input document");
            return false;
        }
        if (!valid_double)
        {
            throwErrorBox("Error when input data is converted to type double. Ensure only numbers are in input document");
            return false;
        }
    }
    return true;
}

void GraphWindow::tableFillBilinear()
{
    // Used to update the table views with the input data.
    table_bilinear = new TableModel(this);
    table_bilinear->populateData(x_data, y_data);
    ui->tableView_2->setModel(table_bilinear);
    ui->tableView_2->setColumnWidth(30, 80);
    ui->tableView_2->horizontalHeader()->setVisible(true);
    ui->tableView_2->show();
}

void GraphWindow::tableFillLinear()
{
    // Used to update the table views with the input data.
    table_linear = new TableModel(this);
    table_linear->populateData(x_data, y_data);
    ui->tableView->setModel(table_linear);
    ui->tableView->setColumnWidth(30, 80);
    ui->tableView->horizontalHeader()->setVisible(true);
    ui->tableView->show();
}

void GraphWindow::plotBilinear()
{
    // This function plots the bilinear data.
    if (!valid_bilinear_data)
        return;
    // Line no describes how many lines have been attempted to fit the data.
    line_no = 1;
    // Sets the output text edit to contain R squared data.
    ui->textEdit_bilinea_data->setText("Calculating goodness of fit (R squared) across sample selections of points.");
    // These vectors are used to split the data vectors x_data and y_data so that the goodness of fit for two different
    // lines can be found. Eg if main vec contains 10, these sub vectors will contain say, 2 and 8. or 4 and 6. etc.
    QVector <double> subvec1_x, subvec1_y;
    QVector <double> subvec2_x, subvec2_y;
    // Used to create the 2D vector.
    r_squared.push_back(QVector <double> ());
    r_squared.push_back(QVector <double> ());
    // Offset value.
    int offset = 1;
    // Offset limits the vector to remove the calculation of r^2 when
    // 1 or 2 values are used to plot a line as yields incorrect results.
    // Eg the goodness of fit for a line between 2 points will be 1, we don't
    // want that data.
    for (int delim = offset; delim < x_data.size()-offset; delim++)
    {
        // Clears the vectors before the iteration.
        subvec1_x.clear();
        subvec1_y.clear();
        subvec2_x.clear();
        subvec2_y.clear();
        // The following two for loops are used to split the data into the sub vectors.
        for (int j = 0;j < delim ;j++)
        {
            subvec1_x.push_back(x_data[j]);
            subvec1_y.push_back(y_data[j]);
        }
        for (int j = delim;j < x_data.size() ;j++ )
        {
            subvec2_x.push_back(x_data[j]);
            subvec2_y.push_back(y_data[j]);
        }
        // The linear regression for the two sepparate lines is then calculated.
        // statistics is then used to calculate the goodness of fit and stores it
        // for later analysis.
        try
        {
            gsl_fit_linear(subvec1_x.data(), 1, subvec1_y.data(), 1, subvec1_x.size(), &c0[0], &c1[0], &cov00[0], &cov01[0], &cov11[0], &sumsq[0]);
            statistics(2, 0);
            gsl_fit_linear(subvec2_x.data(), 1, subvec2_y.data(), 1, subvec2_x.size(), &c0[1], &c1[1], &cov00[1], &cov01[1], &cov11[1], &sumsq[1]);
            statistics(2, 1);
        }  catch (std::exception&)
        {
            throwErrorBox("Error when performing gsl_fit_linear.");
            return;
        }

    }
    // This breakpoint is the array position of x0 once the data has been analysed in rsquComparator.
    int breakpoint = rsquComparator();
    // Using the break point, the correct data for each line is pushed into the vectors.
    for (int index = 0;index < breakpoint ;index++)
    {
        subvec1_x.push_back(x_data[index]);
        subvec1_y.push_back(y_data[index]);
    }
    for (int index = breakpoint;index < x_data.size() ;index++ )
    {
        subvec2_x.push_back(x_data[index]);
        subvec2_y.push_back(y_data[index]);
    }
    // The graph is cleared before it is plotted.
    clearGraph();
    // Since the correct breakpoint has been found, the new data is then used to calculate the two lines of best fit.
    try
    {
        gsl_fit_linear(subvec1_x.data(), 1, subvec1_y.data(), 1, subvec1_x.size(), &c0[0], &c1[0], &cov00[0], &cov01[0], &cov11[0], &sumsq[0]);
        statistics(2, 0);
        gsl_fit_linear(subvec2_x.data(), 1, subvec2_y.data(), 1, subvec2_x.size(), &c0[1], &c1[1], &cov00[1], &cov01[1], &cov11[1], &sumsq[1]);
        statistics(2, 1);
    }  catch (std::exception&)
    {
        x_data.clear();
        y_data.clear();
        throwErrorBox("Error when performing gsl_fit_linear.");
        return;
    }
    // This function is used to create the graph and plot the line.
    buildGraph(0);
    buildGraph(1);
    bilinear_data = true;
    valid_linear_data = false;
    valid_bilinear_data = true;
    // This clears the r_squared vector.
    r_squared[0].clear();
    r_squared[1].clear();
    // The following values are then displayed to the user.
    //c0 is b1 & b2 depending on the array.
    //c1 is a1 & a2 accordingly.
    x0 = (c0[1]-c0[0])/(c1[0] - c1[1]);
    ui->lcdNumber_breakpoint->display(x0);
    ui->lcdNumber_a1->display(c1[0]);
    ui->lcdNumber_b1->display(c0[0]);
    ui->lcdNumber_a2->display(c1[1]);
    ui->lcdNumber_b2->display(c0[1]);
}

void GraphWindow::buildGraph(int array_index)
{
    x_plot.clear();
    y_plot.clear();
    // The start and end vectors determin how much of the line is drawn.
    // for example the bilinear graph cuts the lines at the point of intersection.
    // this is achieved by setting the variables suitably.
    double start = 0;
    double end = 0;
    if (limit_line == true && array_index == 0)
    {
        // Used for cutting line at intersection.
        start = x_startval;
        end = x0;
    }
    else if (limit_line == true && array_index == 1)
    {
        // Used for cutting line at intersection.
        start = x0;
        end = x_data.back()+offset;
    }
    else
    {
        // Used for default line drawing.
        start = x_startval;
        end = x_data.back()+offset;
    }
    // This for loop iterates across the graph and calculates the values so that the line
    // can be drawn.
    for(double i = start; i < end; i+= 0.01)
    {
        if (const_term) y_plot.push_back((i*c1[array_index]) + c0[array_index]);
        else y_plot.push_back((i*c1[array_index]));
        x_plot.push_back(i);
    }
    if (!array_index) line1 = new QGraphicsItemGroup();
    else line2 = new QGraphicsItemGroup();
    // Creates a suitable axis for the line data.
    createAxis(x_data.back()+offset, y_data.back()+offset, x_startval, y_startval);
    for (int i = 1; i < x_plot.size(); i++)
    {
        // Plots the fitted line.
        plot_item = new QGraphicsLineItem(x_pix_to_data*x_plot[i-1], -1*(y_pix_to_data*y_plot[i-1]), x_pix_to_data*x_plot[i], -1*(y_pix_to_data*y_plot[i]));
        // Sets the line colour.
        plot_item->setPen(plot_pen);
        if (!array_index) line1->addToGroup(plot_item);
        else line2->addToGroup(plot_item);
    }
    for (int i = 0; i < x_data.size(); i++)
    {
        // Plots the points and sets the colour they will take.
        // note the size_crosses variable which is used to adjust the crosses.
        point_item = new QGraphicsLineItem(x_pix_to_data*x_data[i]-size_crosses, -1*(y_pix_to_data*y_data[i])-size_crosses, x_pix_to_data*x_data[i]+size_crosses, -1*(y_pix_to_data*y_data[i])+size_crosses);
        point_item->setPen(point_pen);
        if (!array_index) line1->addToGroup(point_item);
        else line2->addToGroup(point_item);
        point_item = new QGraphicsLineItem(x_pix_to_data*x_data[i]-size_crosses, -1*(y_pix_to_data*y_data[i])+size_crosses, x_pix_to_data*x_data[i]+size_crosses, -1*(y_pix_to_data*y_data[i])-size_crosses);
        point_item->setPen(point_pen);
        if (!array_index) line1->addToGroup(point_item);
        else line2->addToGroup(point_item);
    }
    // Adds line to scene.
    if (!array_index) scene->addItem(line1);
    else scene->addItem(line2);
    // Translates view to center on graph.
    ui->graphicsView->translate(100, 0);
}

void GraphWindow::x0DottedLine()
{
    // This function draws or removes the breakpoint line.
    if (!bilinear_data)
        return;
    else if (dotted_line)
    {
        // Print it.
        for (int i = 0; i < y_data.back()+offset; i++)
        {
            // Plots the fitted line.
            x0_item = new QGraphicsLineItem(x_pix_to_data*x0, -1*(y_pix_to_data*i)-5, x_pix_to_data*x0, -1*(y_pix_to_data*i));
            x0_item->setPen(plot_pen);
            x0_line->addToGroup(x0_item);
        }
        scene->addItem(x0_line);
    }
    else
    {
        // Remove it.
        scene->removeItem(x0_line);
        updateScene();
    }


}

void GraphWindow::throwErrorBox(QString error_msg)
{
    // Used for handling error messages.
    // This function has been used to prevent repeating
    // code each time an error box is needed.
    error_box->setWindowTitle("Error");
    error_box->setIcon(QMessageBox::Critical);
    error_box->setText(error_msg);
    error_box->setButtonText(0, "OK");
    error_box->exec();
}

void GraphWindow::plotLinear()
{
    // Plots the linear data.
    if (!valid_linear_data)
        return;
    clearGraph();
    try
    {
       if (const_term) gsl_fit_linear(x_data.data(), 1, y_data.data(), 1, x_data.size(), &c0[0], &c1[0], &cov00[0], &cov01[0], &cov11[0], &sumsq[0]);
       else gsl_fit_mul(x_data.data(), 1, y_data.data(), 1, x_data.size(), &c1[0], &cov11[0], &sumsq[0]);
    }  catch (std::exception&)
    {
        x_data.clear();
        y_data.clear();
        throwErrorBox("Error when performing gsl_fit_linear.");
        return;
    }
    // Creates the graph.
    buildGraph(0);
    // Updates displays.
    ui->lcdNumber_a->display(c1[0]);
    if (const_term) ui->lcdNumber_b->display(c0[0]);
    else ui->lcdNumber_b->display(0);
    // Translates view to center on graph.
    ui->graphicsView->translate(100, 0);
    // Calculates the r squared.
    statistics(1, 0);
    valid_bilinear_data = false;
    bilinear_data = false;
}


void GraphWindow::createAxis(int num_x_ticks, int num_y_ticks, int start_x, int start_y)
{
    // Deltes the current axis if one is present.
    if (!first_pass)
    {
        scene->removeItem(x_axis);
        delete x_axis;
        scene->removeItem(y_axis);
        delete y_axis;
        scene->update();
    }
    // Creates new axis with a margin of 100 pix.
    int margin = 100;
    x_axis =  new QGraphicsAxisItem('x', 0, ui->graphicsView->width() - margin*2, num_x_ticks, 1, start_x, axis_colour);
    y_axis = new QGraphicsAxisItem('y', 0, ui->graphicsView->height() - margin, num_y_ticks, 1, start_y, axis_colour);
    scene->addItem(x_axis);
    scene->addItem(y_axis);
    x_pix_to_data = x_axis->pixel_to_data();
    y_pix_to_data = y_axis->pixel_to_data();
    // Used to detect if this is the first pass of the program.
    first_pass = false;
}

void GraphWindow::statistics(int mode, int index)
{
    // This function is used to calculate the goodness of fit.
    int n = x_data.size();
    // This totals up y vector.
    double running_total = 0;
    for (int i = 0; i < y_data.size(); i++)
        running_total = running_total + y_data[i];
    // Finds average y using total.
    double y_ave = running_total/n;
    // Finds the degree of liberty which is used for calculating the adjusted r squared value.
    double deg_lib = n-2;
    // This totals up the SCT. See equation @https://en.wikipedia.org/wiki/Segmented_regression
    double sq_total = 0;
    for (int i = 0;i <y_data.size(); i++)
        sq_total = sq_total + pow(y_data[i] - y_ave, 2);
    // Finds r squared based on the equation found @https://en.wikipedia.org/wiki/Segmented_regression
    // scroll and look for the Cd equation (coefficient of determination for all data).
    double Rsqu = 1 - sumsq[index]/sq_total;
    // Finds the adjusted r squared value.
    double adjus_Rsqu = 1 - (double)(n-1) / deg_lib*(1-Rsqu);
    // Sends output to screen.
    if (mode == 1)
    {
        ui->textEdit->setText("Data:");
        ui->textEdit->append("R-squared:");
        ui->textEdit->append(QString::number(Rsqu));
        ui->textEdit->append("Adjusted R-squared:");
        ui->textEdit->append(QString::number(adjus_Rsqu));
    }
    else
    {
        r_squared[index].push_back(Rsqu);
        ui->textEdit_bilinea_data->append("Analysing potential line ");
        ui->textEdit_bilinea_data->append(QString::number(line_no));
        ui->textEdit_bilinea_data->append("R squared: ");
        ui->textEdit_bilinea_data->append(QString::number(Rsqu));
        ui->textEdit_bilinea_data->append("Adjusted R-squared:");
        ui->textEdit_bilinea_data->append(QString::number(adjus_Rsqu));
    }
    line_no++;
}

void GraphWindow::changeLineColour()
{
    // Changes line colour.
    line_colour = QColorDialog::getColor(Qt::black, this, tr("Pick Colour Of Line"));
    plot_pen.setColor(line_colour);
    updateColours();
    // Used to auto update the colours if bool is true.
    if (first_pass)
        return;
    else if (auto_change == true && bilinear_data == false)
        plotLinear();
    else if (auto_change == true && bilinear_data == true)
        plotBilinear();
}

void GraphWindow::changeAxisColour()
{
    // Changes axis colour using Qt dialog.
    axis_colour = QColorDialog::getColor(Qt::black, this, tr("Pick Colour Of Axis"));
    axis_pen->setColor(axis_colour);
    updateColours();
    // Used to auto update the colours if bool is true.
    if (first_pass)
        return;
    else if (auto_change == true && bilinear_data == false)
        plotLinear();
    else if (auto_change == true && bilinear_data == true)
        plotBilinear();
}

void GraphWindow::graphOptions()
{
    // Function used to produce a custom axis. Not recommended for regular use.
    // Calls the graph options ui.
    GraphOptions edit(this);
    int ret_code = edit.exec();
    if (!ret_code)
        return;
    if (!first_pass)
    {
        scene->removeItem(x_axis);
        delete x_axis;
        scene->removeItem(y_axis);
        delete y_axis;
        scene->update();
    }
    ui->graphicsView->update();
    // Makes new axis.
    x_axis =  new QGraphicsAxisItem('x', edit.x_pix_min, edit.x_pix_max, edit.x_ticks, edit.tick_increment, edit.x_startval, axis_colour);
    y_axis = new QGraphicsAxisItem('y', edit.y_pix_min, edit.y_pix_max, edit.y_ticks, edit.tick_increment, edit.y_startval, axis_colour);
    scene->addItem(x_axis);
    scene->addItem(y_axis);
    ui->graphicsView->update();
    first_pass = false;
}

void GraphWindow::updateColours()
{
    // This sets the labels in the toolbox to change to the current colour of the line or axis.
    line_pixmap.fill(line_colour);
    ui->label_linecolour->setPixmap(line_pixmap);
    ui->label_linecolour_2->setPixmap(line_pixmap);
    axis_pixmap.fill(axis_colour);
    ui->label_axiscolour->setPixmap(axis_pixmap);
    ui->label_axiscolour_2->setPixmap(axis_pixmap);
}

bool GraphWindow::inputData(int mode)
{
    // mode 1 is linear, mode 2 is bilinear.
    bool val_data = false;
    x_data.clear();
    y_data.clear();
    // Gets file path from file Ui.
    file_path = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath(), tr("File(*.csv)"));
    // Opens file accordingly.
    QFile file(file_path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        // Handles invalid file paths such as clicking X on ui.
        valid_linear_data = false;
        valid_bilinear_data = false;
        return false;
    }
    // Following try catch reads first line from file.
    QString line;
    try
    {
        line = file.readLine();
    }  catch (std::exception&)
    {
        throwErrorBox("Read line function failed.");
        return false;
    }
    // While loop iterates until EOF is read.
    while(!line.isNull())
    {
        // This function is used to split the input line.
        val_data = processData(line); // Returns true if valid.
        // Error checking for valid data.
        if(!val_data) break;
        // Reads next line.
        line = file.readLine();
    }
    // Error handling.
    if (!val_data)
    {
        x_data.clear();
        y_data.clear();
        return false;
    }
    // Sets variables according to mode.
    if(mode == 1) valid_linear_data = true;
    else valid_bilinear_data = true;
    return true;
}

int GraphWindow::rsquComparator()
{
    // This function is used to compare all the R squared values which have been
    // calculated, it determines which two R squared values overall have the best
    // goodness of fit value.
    double smallest_val = 1.0;
    int smallest_index = 0;
    double total[r_squared[0].size()];
    for (int i = 0; i < r_squared[0].size(); i ++)
    {
        total[i] = r_squared[0][i] - r_squared[1][i];
        try {total[i] = fabs(total[i]);}
        catch (std::exception&)
        {
            throwErrorBox("Error Occurred when finding absolute value of double.");
            return 0;
        }
        if (total[i] < smallest_val)
        {
            // If the current R squared value is smaller than
            // the smallest value, update the variable.
            total[i] = smallest_val;
            // This is used to determine where the breakpoint exists.
            smallest_index = i;
        }
    }
    // Returns the breakpoint.
    return smallest_index;
}

void GraphWindow::on_actionColour_triggered()
{
    changeLineColour();
}

void GraphWindow::on_pushButton_plot_clicked()
{
    plotLinear();
}

void GraphWindow::on_pushButton_clear_2_clicked()
{
    clearGraph();
}

void GraphWindow::on_pushButton_graphoptions_clicked()
{
    graphOptions();
}

void GraphWindow::on_actionCurrent_Plot_Toolbar_toggled(bool arg1)
{
    ui->dockWidget->setVisible(arg1);
}

void GraphWindow::on_actionLinear_Regression_triggered()
{
    on_pushButton_inputfile_clicked();
}

void GraphWindow::on_pushButton_data_clicked()
{
    // Used for bessel feature which has been removed.
}

void GraphWindow::on_pushButton_options_clicked()
{
    // Used for bessel feature which has been removed.
}

void GraphWindow::on_pushButton_clear_clicked()
{
    // Used for bessel feature which has been removed.
}

void GraphWindow::on_actionData_triggered()
{
    // Used for bessel feature which has been removed.
}

void GraphWindow::on_checkBox_clicked(bool checked)
{
    const_term = checked;
}

void GraphWindow::on_actionColour_axis_triggered()
{
    changeAxisColour();
}

void GraphWindow::on_actionGraph_Options_triggered()
{
    graphOptions();
}

void GraphWindow::on_actionData_Points_Colour_triggered()
{
    // Used to change the colour of the data points.
    points_colour = QColorDialog::getColor(Qt::black, this, tr("Pick Colour Of Points"));
    point_pen.setColor(points_colour);
    if (first_pass)
        return;
    else if (auto_change == true && bilinear_data == false)
        plotLinear();
    else if (auto_change == true && bilinear_data == true)
        plotBilinear();
}

void GraphWindow::on_actionData_Points_Size_triggered()
{
    // Used to change the size of the data points.
    ChangeThickness edit(size_crosses, 1, this);
    edit.exec();
    size_crosses = edit.thickness;
    if (first_pass)
        return;
    else if (auto_change == true && bilinear_data == false)
        plotLinear();
    else if (auto_change == true && bilinear_data == true)
        plotBilinear();
}

void GraphWindow::on_toolButton_axiscolour_clicked()
{
    changeAxisColour();
}

void GraphWindow::on_toolButton_linecolour_clicked()
{
    changeLineColour();
}

void GraphWindow::on_actionThickness_triggered()
{
    // Changes thickness of line.
    ChangeThickness edit(pen_thickness, 0, this);
    edit.exec();
    pen_thickness = edit.thickness;
    plot_pen.setWidth(pen_thickness);
    if (first_pass)
        return;
    else if (auto_change == true && bilinear_data == false)
        plotLinear();
    else if (auto_change == true && bilinear_data == true)
        plotBilinear();
}

void GraphWindow::on_actionConstant_Term_toggled(bool arg1)
{
    const_term = arg1;
    if (const_term) ui->checkBox->setCheckState(Qt::Checked);
    else ui->checkBox->setCheckState(Qt::Unchecked);
}

void GraphWindow::on_actionClear_Graph_triggered()
{
    clearGraph();
}

void GraphWindow::on_actionPlot_triggered()
{
    plotLinear();
}

void GraphWindow::on_pushButton_refresh_clicked()
{
    updateScene();
}

void GraphWindow::on_pushButton_usage_clicked()
{
    // Shows the help dialog pictures for linear data.
    HelpDialog *dialog = new HelpDialog(4 , this);
    dialog->setImage(":/help/help/linear_help1.PNG");
    dialog->setImage(":/help/help/linear_help2.PNG");
    dialog->setImage(":/help/help/linear_help3.PNG");
    dialog->setImage(":/help/help/linear_help4.PNG");
    dialog->onLaunch();
    dialog->show();
}

void GraphWindow::on_pushButton_inputbilineardata_clicked()
{
    if(inputData(2))
        tableFillBilinear();
}

void GraphWindow::on_pushButton_plotbilineardata_clicked()
{
    // Plots the bilinear data.
    plotBilinear();
    // Prints the x0 line.
    x0DottedLine();
}

void GraphWindow::on_checkBox_autoupdate_toggled(bool checked)
{
    auto_change = checked;
}

void GraphWindow::on_pushButton_usage_2_clicked()
{
    // Shows the help dialog pictures for bilinear graph usage.
    HelpDialog *dialog = new HelpDialog(7, this);
    dialog->setImage(":/help/help/bilinear_help1.PNG");
    dialog->setImage(":/help/help/bilinear_help2.PNG");
    dialog->setImage(":/help/help/bilinear_help3.PNG");
    dialog->setImage(":/help/help/bilinear_help4.PNG");
    dialog->setImage(":/help/help/bilinear_output1.PNG");
    dialog->setImage(":/help/help/bilinear_output2.PNG");
    dialog->setImage(":/help/help/bilinear_output3.PNG");
    dialog->onLaunch();
    dialog->show();
}

void GraphWindow::on_pushButton_refresh_2_clicked()
{
    on_pushButton_refresh_clicked();
}

void GraphWindow::on_pushButton_clear_3_clicked()
{
    clearGraph();
}

void GraphWindow::on_checkBox_autoupdate_2_toggled(bool checked)
{
    auto_change = checked;
}

void GraphWindow::on_toolButton_linecolour_2_clicked()
{
    changeLineColour();
}

void GraphWindow::on_toolButton_axiscolour_2_clicked()
{
    changeAxisColour();
}

void GraphWindow::on_checkBox_2_toggled(bool checked)
{
    dotted_line = checked;
    x0DottedLine();
}

void GraphWindow::on_actionBilinear_Regression_triggered()
{
    if(inputData(2))
        tableFillBilinear();
}

void GraphWindow::on_actionPlot_Bilinear_Line_triggered()
{
    plotBilinear();
    x0DottedLine();
}

void GraphWindow::on_actionCut_Line_at_x0_triggered(bool checked)
{
    limit_line = checked;
}

void GraphWindow::on_actionLinear_triggered()
{
    on_pushButton_usage_clicked();
}

void GraphWindow::on_actionBilinear_triggered()
{
    on_pushButton_usage_2_clicked();
}

void GraphWindow::on_checkBox_3_clicked(bool checked)
{
    // Used to either show or not show the x0 breakpoint line.
    limit_line = checked;
    if (first_pass)
        return;
    else if (bilinear_data == false)
        plotLinear();
    else if (bilinear_data == true)
        plotBilinear();
}
