#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QVector>
#include <QPen>
#include "wkv.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
        hipAngleSensor("hip_joint_angle", "rad"),
        hipGyroSensor("hip_joint_gyro", "rad/s")
{
    // setup widgets
    ui->setupUi(this);

    // Define pen stiles for plot traces
    QPen pen_angle;
    pen_angle.setWidth(1);
    pen_angle.setColor(QColor(50, 50, 220));
    QPen pen_gyro;
    pen_gyro.setWidth(1);
    pen_gyro.setColor(QColor(220, 50, 50));

    // Take care of 1st plot with complete/full data

    // General configuration of plotSensorFull Axes
    ui->plotSensorFull->yAxis->setRange(-3, 3);
    ui->plotSensorFull->xAxis->setLabel("time [s]");
    QCPGraph* plotFull_traceAngle = ui->plotSensorFull->addGraph();
    QCPGraph* plotFull_traceGyro = ui->plotSensorFull->addGraph();
    plotFull_traceAngle->setPen(pen_angle);
    plotFull_traceGyro->setPen(pen_gyro);


    // Update plot with full sensor data on Button click
    auto callback_fullPlot = [this, plotFull_traceAngle, plotFull_traceGyro]()
    {

        // Obtain sensor data in full time interval
        hipAngleSensor.acquireSensor();
        hipGyroSensor.acquireSensor();
        TimeSeries hipAngleAcq = hipAngleSensor.getSensorData().removeTimeOffset();
        TimeSeries hipGyroAcq = hipGyroSensor.getSensorData().removeTimeOffset();

        // Add Angle Sensor Data
        plotFull_traceAngle->setData(hipAngleAcq.getTimeStamps(), hipAngleAcq.getDataValue());
        plotFull_traceAngle->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 1));
        this->ui->plotSensorFull->xAxis->setRange(hipAngleAcq.getTimeStamps().first(), hipAngleAcq.getTimeStamps().last());

        // Add Gyro sensor Data
        plotFull_traceGyro->setData(hipGyroAcq.getTimeStamps(), hipGyroAcq.getDataValue());
        plotFull_traceGyro->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 1));
        this->ui->plotSensorFull->xAxis->setRange(hipGyroAcq.getTimeStamps().first(), hipGyroAcq.getTimeStamps().last());

        // Show data
        this->ui->plotSensorFull->replot();

    };

    // Connect the textChanged signal to the callback
    connect(ui->buttonPlotFull, &QPushButton::released, this, callback_fullPlot);


    // Take care of 2nd plot with zoom-in data

    // General configuration of plotSensorZoom Axes
    ui->plotSensorZoom->yAxis->setRange(-3, 3);
    ui->plotSensorZoom->xAxis->setLabel("time [s]");
    QCPGraph* plotZoom_traceAngle = ui->plotSensorZoom->addGraph();
    QCPGraph* plotZoom_traceGyro = ui->plotSensorZoom->addGraph();
    plotZoom_traceAngle->setPen(pen_angle);
    plotZoom_traceGyro->setPen(pen_gyro);


    // Update plot with Zoom-in of sensor data
    auto callback_zoomPlot = [this, plotZoom_traceAngle, plotZoom_traceGyro]()
    {

        // Read time interval limits from text boxes
        float timeBegin = ui->inputTimeBegin->text().toFloat();
        float timeEnd = ui->inputTimeEnd->text().toFloat();

        // Obtain sensor data in full time interval
        TimeSeries hipAngleAcq = hipAngleSensor.getSensorData().removeTimeOffset().slice(timeBegin, timeEnd);
        TimeSeries hipGyroAcq = hipGyroSensor.getSensorData().removeTimeOffset().slice(timeBegin, timeEnd);

        // Add Angle Sensor Data
        plotZoom_traceAngle->setData(hipAngleAcq.getTimeStamps(), hipAngleAcq.getDataValue());
        plotZoom_traceAngle->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 1));
        this->ui->plotSensorZoom->xAxis->setRange(hipAngleAcq.getTimeStamps().first(), hipAngleAcq.getTimeStamps().last());

        // Add Gyro sensor Data
        plotZoom_traceGyro->setData(hipGyroAcq.getTimeStamps(), hipGyroAcq.getDataValue());
        plotZoom_traceGyro->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 1));
        this->ui->plotSensorZoom->xAxis->setRange(hipGyroAcq.getTimeStamps().first(), hipGyroAcq.getTimeStamps().last());

        // Show data
        this->ui->plotSensorZoom->replot();
    };

    // Connect the textChanged signal to the callback
    connect(ui->buttonPlotZoom, &QPushButton::released, this, callback_zoomPlot);
}

MainWindow::~MainWindow()
{
    delete ui;
}
