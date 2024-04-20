#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QVector>
#include <QPen>

#include "wkv.h"
#include "utilities.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
      hipAngleSensor("hip_joint_angle", "rad"),
      hipGyroSensor("hip_joint_gyro", "rad/s")
{
    // setup widgets
    ui->setupUi(this);
    setupGraphs();
}

void MainWindow::setupGraphs()
{
    // Define pen stiles for plot traces
    QPen pen_angle;
    pen_angle.setWidth(1);
    pen_angle.setColor(QColor(50, 50, 220));
    QPen pen_gyro;
    pen_gyro.setWidth(1);
    pen_gyro.setColor(QColor(220, 50, 50));
    QPen pen_corr;
    pen_corr.setColor(QColor(10, 10, 10));

    // Take care of 1st plot with complete/full data

    // General configuration of plotSensorFull Axes
    ui->plotSensorFull->yAxis->setRange(-3, 5);
    ui->plotSensorFull->xAxis->setLabel("time [s]");
    ui->plotSensorFull->legend->setVisible(true);
    ui->plotSensorFull->addGraph();
    ui->plotSensorFull->addGraph();
    ui->plotSensorFull->graph(0)->setPen(pen_angle);
    ui->plotSensorFull->graph(1)->setPen(pen_gyro);
    ui->plotSensorFull->graph(0)->setName(hipAngleSensor.getSensorName() + " [" + hipAngleSensor.getUnit() + "]");
    ui->plotSensorFull->graph(1)->setName(hipGyroSensor.getSensorName() + " [" + hipGyroSensor.getUnit() + "]");

    // General configuration of plotSensorZoom Axes
    ui->plotSensorZoom->yAxis->setRange(-3, 3);
    ui->plotSensorZoom->xAxis->setLabel("time [s]");
    ui->plotSensorZoom->addGraph();
    ui->plotSensorZoom->addGraph();
    ui->plotSensorZoom->graph(0)->setPen(pen_angle);
    ui->plotSensorZoom->graph(1)->setPen(pen_gyro);

    // General configuration of plotSensorResampled Axes
    ui->plotSensorResampled->yAxis->setRange(-3, 3);
    ui->plotSensorResampled->xAxis->setLabel("time [s]");
    ui->plotSensorResampled->addGraph();
    ui->plotSensorResampled->addGraph();
    ui->plotSensorResampled->graph(0)->setPen(pen_angle);
    ui->plotSensorResampled->graph(1)->setPen(pen_gyro);

    // General configuration of plotSensorResampled Axes
    ui->plotSensorRegression->xAxis->setRange(-1, 1);
    ui->plotSensorRegression->yAxis->setRange(-3, 3);
    ui->plotSensorRegression->xAxis->setLabel("[rad]");
    ui->plotSensorRegression->yAxis->setLabel("[rad/s]");
    ui->plotSensorRegression->addGraph();
    ui->plotSensorRegression->addGraph();
    ui->plotSensorRegression->graph(0)->setPen(pen_corr);

    connect(ui->buttonAcquire, &QPushButton::released, this, &MainWindow::onButtonAcquire);
    connect(ui->buttonZoom, &QPushButton::released, this, &MainWindow::onButtonZoom);
    connect(ui->buttonResample, &QPushButton::released, this, &MainWindow::onButtonResample);
    connect(ui->buttonRegression, &QPushButton::released, this, &MainWindow::onButtonRegression);
}

// Update plot with full sensor data on Button click
void MainWindow::onButtonAcquire()
{
    // Obtain sensor data in full time interval
    hipAngleSensor.acquireSensor();
    hipGyroSensor.acquireSensor();
    TimeSeries hipAngleAcq = hipAngleSensor.getSensorData().removeTimeOffset();
    TimeSeries hipGyroAcq = hipGyroSensor.getSensorData().removeTimeOffset();

    // Add Angle Sensor Data
    ui->plotSensorFull->graph(0)->setData(hipAngleAcq.getTimeStamps(), hipAngleAcq.getDataValue());
    ui->plotSensorFull->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 3));
    ui->plotSensorFull->xAxis->setRange(hipAngleAcq.getTimeStamps().first(), hipAngleAcq.getTimeStamps().last());

    // Add Gyro sensor Data
    ui->plotSensorFull->graph(1)->setData(hipGyroAcq.getTimeStamps(), hipGyroAcq.getDataValue());
    ui->plotSensorFull->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 3));
    ui->plotSensorFull->xAxis->setRange(hipGyroAcq.getTimeStamps().first(), hipGyroAcq.getTimeStamps().last());

    // Show data
    ui->plotSensorFull->replot();
};

// Update plot with Zoomed-in sensor data
void MainWindow::onButtonZoom()
{

    // Read time interval limits from text boxes
    float timeBegin = ui->inputTimeBegin->text().toFloat();
    float timeEnd = ui->inputTimeEnd->text().toFloat();

    // Obtain sensor data in full time interval
    TimeSeries hipAngleAcq = hipAngleSensor.getSensorData().removeTimeOffset().slice(timeBegin, timeEnd);
    TimeSeries hipGyroAcq = hipGyroSensor.getSensorData().removeTimeOffset().slice(timeBegin, timeEnd);

    // Update Zoom plot

    // Add Angle Sensor Data
    ui->plotSensorZoom->graph(0)->setData(hipAngleAcq.getTimeStamps(), hipAngleAcq.getDataValue());
    ui->plotSensorZoom->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 3));
    // Add Gyro sensor Data
    ui->plotSensorZoom->graph(1)->setData(hipGyroAcq.getTimeStamps(), hipGyroAcq.getDataValue());
    ui->plotSensorZoom->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 3));
    ui->plotSensorZoom->xAxis->setRange(hipGyroAcq.getTimeStamps().first(), hipGyroAcq.getTimeStamps().last());
    // Update plot
    ui->plotSensorZoom->replot();
};

// Update plot with resampled data
void MainWindow::onButtonResample()
{
    // Read time interval limits from text boxes
    float timeBegin = ui->inputTimeBegin->text().toFloat();
    float timeEnd = ui->inputTimeEnd->text().toFloat();

    // Obtain resampled sensor data in zoomin time interval
    TimeSeries hipAngleAcqResampled = hipAngleSensor.getSensorData().removeTimeOffset().slice(timeBegin, timeEnd).resample100Hz(timeBegin);
    TimeSeries hipGyroAcqResampled = hipGyroSensor.getSensorData().removeTimeOffset().slice(timeBegin, timeEnd).resample100Hz(timeBegin);

    // Add Angle Sensor Data
    ui->plotSensorResampled->graph(0)->setData(hipAngleAcqResampled.getTimeStamps(), hipAngleAcqResampled.getDataValue());
    ui->plotSensorResampled->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 3));
    // Add Gyro sensor Data
    ui->plotSensorResampled->graph(1)->setData(hipGyroAcqResampled.getTimeStamps(), hipGyroAcqResampled.getDataValue());
    ui->plotSensorResampled->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 3));
    ui->plotSensorResampled->xAxis->setRange(hipGyroAcqResampled.getTimeStamps().first(), hipGyroAcqResampled.getTimeStamps().last());
    // Update plot
    ui->plotSensorResampled->replot();

    // Update maxima list

    // Empty existing list
    ui->listMaxima->clear();
    QVector<std::pair<double, double>> MaximaList = hipAngleAcqResampled.findLocalMaxima();
    // Populated with newly calculated maxima
    for (std::pair<double, double> maximum : MaximaList)
    {
        ui->listMaxima->addItem(QString("t = %1s; angle = %2rad").arg(maximum.first, 0, 'f', 2).arg(maximum.second, 0, 'f', 2));
    }
};

// Update plot with resampled data
void MainWindow::onButtonRegression()
{
    // Read time interval limits from text boxes
    float timeBegin = ui->inputTimeBegin->text().toFloat();
    float timeEnd = ui->inputTimeEnd->text().toFloat();

    // Obtain resampled sensor data in zoomin time interval
    TimeSeries hipAngleAcqResampled = hipAngleSensor.getSensorData().removeTimeOffset().slice(timeBegin, timeEnd).resample100Hz(timeBegin);
    TimeSeries hipGyroAcqResampled = hipGyroSensor.getSensorData().removeTimeOffset().slice(timeBegin, timeEnd).resample100Hz(timeBegin);

    // Update scatter and regression plot

    // Add scatter plot
    ui->plotSensorRegression->graph(0)->setData(hipAngleAcqResampled.getDataValue(), hipGyroAcqResampled.getDataValue());
    ui->plotSensorRegression->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->plotSensorRegression->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 3));

    // Compute linear regression
    std::pair<double, double> regressionParam = linearRegression(hipGyroAcqResampled.getDataValue(), hipAngleAcqResampled.getDataValue());
    double beta = regressionParam.first;
    double alfa = regressionParam.second;

    // Add linear regression line
    QVector<double> x{-1, 1};
    QVector<double> y{x[0] * beta + alfa, x[1] * beta + alfa};
    ui->plotSensorRegression->graph(1)->setData(x, y);
    ui->plotSensorRegression->replot();

    // Print regression parameters
    ui->listRegression->clear();
    ui->listRegression->addItem(QString("beta = %1").arg(beta, 0, 'f', 2));
    ui->listRegression->addItem(QString("alfa = %1").arg(alfa, 0, 'f', 2));
};

MainWindow::~MainWindow()
{
    delete ui;
}
