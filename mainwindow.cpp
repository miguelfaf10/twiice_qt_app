#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QVector>

#include "python_binder.h"




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // generate some data:
    QVector<double> x(101), y(101); // initialize with entries 0..100
    for (int i = 0; i < 101; ++i)
    {
        x[i] = i / 50.0 - 1; // x goes from -1 to 1
        y[i] = x[i] * x[i];  // let's plot a quadratic function
    }

    // setup widgets

    // Plot full data widget
    ui->plotSensorFull->addGraph();
    ui->plotSensorFull->graph(0)->setData(x, y);
    ui->plotSensorFull->xAxis->setLabel("x");
    ui->plotSensorFull->yAxis->setLabel("y");
    ui->plotSensorFull->xAxis->setRange(-1, 1);
    ui->plotSensorFull->yAxis->setRange(0, 1);
    ui->plotSensorFull->replot();

    // Plot zoom data widget
    ui->plotSensorFull->addGraph();
    ui->plotSensorFull->graph(0)->setData(x, y);
    ui->plotSensorFull->xAxis->setLabel("x");
    ui->plotSensorFull->yAxis->setLabel("y");
    ui->plotSensorFull->xAxis->setRange(-1, 1);
    ui->plotSensorFull->yAxis->setRange(0, 1);
    ui->plotSensorFull->replot();


    getVectorDataFromPython();

}

MainWindow::~MainWindow()
{
    delete ui;
}
