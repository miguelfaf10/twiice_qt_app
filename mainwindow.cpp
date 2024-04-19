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
    ui->plotSensorFull->yAxis->setRange(-3, 5);
    ui->plotSensorFull->xAxis->setLabel("time [s]");
    ui->plotSensorFull->legend->setVisible(true);

    QCPGraph* plotFull_traceAngle = ui->plotSensorFull->addGraph();
    QCPGraph* plotFull_traceGyro = ui->plotSensorFull->addGraph();

    plotFull_traceAngle->setPen(pen_angle);
    plotFull_traceGyro->setPen(pen_gyro);
    plotFull_traceAngle->setName(hipAngleSensor.getSensorName() + " [" + hipAngleSensor.getUnit() + "]");
    plotFull_traceGyro->setName(hipGyroSensor.getSensorName() + " [" + hipGyroSensor.getUnit() + "]");

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
        plotFull_traceAngle->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 3));
        this->ui->plotSensorFull->xAxis->setRange(hipAngleAcq.getTimeStamps().first(), hipAngleAcq.getTimeStamps().last());

        // Add Gyro sensor Data
        plotFull_traceGyro->setData(hipGyroAcq.getTimeStamps(), hipGyroAcq.getDataValue());
        plotFull_traceGyro->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 3));
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


    // Plot Zoom-in sensor data
    auto callback_zoomPlot = [this, plotZoom_traceAngle, plotZoom_traceGyro]()
    {
        // Read time interval limits from text boxes
        float timeBegin = ui->inputTimeBegin->text().toFloat();
        float timeEnd = ui->inputTimeEnd->text().toFloat();

        // Obtain sensor data in full time interval
        TimeSeries hipAngleAcq = hipAngleSensor.getSensorData().removeTimeOffset().slice(timeBegin, timeEnd);
        TimeSeries hipGyroAcq = hipGyroSensor.getSensorData().removeTimeOffset().slice(timeBegin, timeEnd);

        // Update Zoom plot

        // Add Angle Sensor Data
        plotZoom_traceAngle->setData(hipAngleAcq.getTimeStamps(), hipAngleAcq.getDataValue());
        plotZoom_traceAngle->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 3));
        // Add Gyro sensor Data
        plotZoom_traceGyro->setData(hipGyroAcq.getTimeStamps(), hipGyroAcq.getDataValue());
        plotZoom_traceGyro->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 3));
        this->ui->plotSensorZoom->xAxis->setRange(hipGyroAcq.getTimeStamps().first(), hipGyroAcq.getTimeStamps().last());
        // Update plot
        this->ui->plotSensorZoom->replot();

    };
    // Connect the button signal to the callback
    connect(ui->buttonPlotZoom, &QPushButton::released, this, callback_zoomPlot);


    // Take care of 3rd plot with resampled data

    // General configuration of plotSensorResampled Axes
    ui->plotSensorResampled->yAxis->setRange(-3, 3);
    ui->plotSensorResampled->xAxis->setLabel("time [s]");
    QCPGraph* plotResampled_traceAngle = ui->plotSensorResampled->addGraph();
    QCPGraph* plotResampled_traceGyro = ui->plotSensorResampled->addGraph();
    plotResampled_traceAngle->setPen(pen_angle);
    plotResampled_traceGyro->setPen(pen_gyro);


    // Resample zoom-in signal and update its plot maxima table
    auto callback_resampledPlot = [this, plotResampled_traceAngle, plotResampled_traceGyro]()
    {
        // Read time interval limits from text boxes
        float timeBegin = ui->inputTimeBegin->text().toFloat();
        float timeEnd = ui->inputTimeEnd->text().toFloat();

        // Obtain resampled sensor data in zoomin time interval
        TimeSeries hipAngleAcqResampled = hipAngleSensor.getSensorData().removeTimeOffset().slice(timeBegin, timeEnd).resample100Hz(timeBegin);
        TimeSeries hipGyroAcqResampled = hipGyroSensor.getSensorData().removeTimeOffset().slice(timeBegin, timeEnd).resample100Hz(timeBegin);

        // Update resampled plot

        // Add Angle Sensor Data
        plotResampled_traceAngle->setData(hipAngleAcqResampled .getTimeStamps(), hipAngleAcqResampled .getDataValue());
        plotResampled_traceAngle->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 3));
        // Add Gyro sensor Data
        plotResampled_traceGyro->setData(hipGyroAcqResampled.getTimeStamps(), hipGyroAcqResampled.getDataValue());
        plotResampled_traceGyro->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 3));
        this->ui->plotSensorResampled->xAxis->setRange(hipGyroAcqResampled.getTimeStamps().first(), hipGyroAcqResampled.getTimeStamps().last());
        // Update plot
        this->ui->plotSensorResampled->replot();

        // Update maxima list

        // Empty existing list
        ui->listMaxima->clear();
        QVector<std::pair<double, double>> MaximaList = hipAngleAcqResampled .findLocalMaxima();
        // Populated with newly calculated maxima
        for (std::pair<double, double> maximum: MaximaList) {
            ui->listMaxima->addItem(QString("t = %1s; angle = %2rad").arg(maximum.first, 0, 'f', 2).arg(maximum.second, 0, 'f', 2));
        }
    };

    // Connect the respective button signal to the callback
    connect(ui->buttonPlotResampled, &QPushButton::released, this, callback_resampledPlot);


}

MainWindow::~MainWindow()
{
    delete ui;
}
