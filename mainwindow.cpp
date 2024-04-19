#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QVector>
#include <QPen>
#include "wkv.h"
#include "utilities.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    // setup widgets
    ui->setupUi(this);


    WKV hip_angle_sensor = WKV("hip_joint_angle", "rad");
    WKV hip_gyro_sensor = WKV("hip_joint_gyro", "rad");

    hip_angle_sensor.acquireSensor();
    hip_gyro_sensor.acquireSensor();

    // Obtain sensor data in full time interval
    QVector<double> t_angle = hip_angle_sensor.getTimeStamps();
    QVector<double> y_angle = hip_angle_sensor.getSensorData();
    removeOffset(t_angle, t_angle.first());

    qDebug() << "Standard Deviation of t_angle: " << computeStdDev(t_angle) << "\n";

    QVector<double> t_gyro = hip_gyro_sensor.getTimeStamps();
    QVector<double> y_gyro = hip_gyro_sensor.getSensorData();
    removeOffset(t_gyro, t_gyro.first());

    qDebug() << "Standard Deviation of t_gyro: " << computeStdDev(t_gyro) << "\n";

    // Plot full data widget
    QPen pen_angle;
    pen_angle.setWidth(1);
    pen_angle.setColor(QColor(50, 50, 220));

    ui->plotSensorFull->addGraph();
    ui->plotSensorFull->graph(0)->setData(t_angle, y_angle);
    ui->plotSensorFull->graph(0)->setPen(pen_angle);
    ui->plotSensorFull->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 2));
    ui->plotSensorFull->xAxis->setLabel("time [s]");
    ui->plotSensorFull->yAxis->setLabel("y");
    ui->plotSensorFull->xAxis->setRange(t_angle.first(), t_angle.last());
    ui->plotSensorFull->yAxis->setRange(-3, 3);
    ui->plotSensorFull->replot();

    QPen pen_gyro;
    pen_gyro.setWidth(1);
    pen_gyro.setColor(QColor(220, 50, 50));

    ui->plotSensorFull->addGraph();
    ui->plotSensorFull->graph(1)->setData(t_gyro, y_gyro);
    ui->plotSensorFull->graph(1)->setPen(pen_gyro);
    ui->plotSensorFull->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 2));
    ui->plotSensorFull->xAxis->setLabel("time [s]");
    ui->plotSensorFull->yAxis->setLabel("y");
    ui->plotSensorFull->xAxis->setRange(t_gyro.first(), t_gyro.last());
    ui->plotSensorFull->yAxis->setRange(-3, 3);
    ui->plotSensorFull->replot();

    ui->plotSensorZoom->addGraph();
    ui->plotSensorZoom->graph(0)->setPen(pen_angle);
    ui->plotSensorZoom->xAxis->setLabel("time [s]");
    ui->plotSensorZoom->yAxis->setLabel("y");

    ui->plotSensorZoom->addGraph();
    ui->plotSensorZoom->graph(1)->setPen(pen_gyro);
    ui->plotSensorZoom->xAxis->setLabel("time [s]");
    ui->plotSensorZoom->yAxis->setLabel("y");

    // Update plot with Zoom-in of sensor data
    auto callback_zoomin = [=]()
    {
        // This code will be executed when the text changes
        qDebug() << "Input Zoom time begin: " << ui->inputTimeBegin->text() << "\n";
        qDebug() << "Input Zoom Time end: " << ui->inputTimeEnd->text() << "\n";

        // Read time interval limits from text boxes
        float timeBegin = ui->inputTimeBegin->text().toFloat();
        float timeEnd = ui->inputTimeEnd->text().toFloat();

        // Obtain sensor data in zoom-in interval
        std::pair<int, int> zoom_idxs;

        zoom_idxs = sliceTimeSeries(t_angle, timeBegin, timeEnd);
        int zoom_idx_begin = zoom_idxs.first;
        int zoom_idx_end = zoom_idxs.second;
        int zoom_idx_range = zoom_idx_end - zoom_idx_begin;
        QVector<double> t_angle_zoom = t_angle.mid(zoom_idx_begin, zoom_idx_range);
        QVector<double> y_angle_zoom = y_angle.mid(zoom_idx_begin, zoom_idx_range);

        zoom_idxs = sliceTimeSeries(t_gyro, timeBegin, timeEnd);
        zoom_idx_begin = zoom_idxs.first;
        zoom_idx_end = zoom_idxs.second;
        zoom_idx_range = zoom_idx_end - zoom_idx_begin;
        QVector<double> t_gyro_zoom = t_gyro.mid(zoom_idx_begin, zoom_idx_range);
        QVector<double> y_gyro_zoom = y_gyro.mid(zoom_idx_begin, zoom_idx_range);

        // Plot zoom data widget
        ui->plotSensorZoom->graph(0)->setData(t_angle_zoom, y_angle_zoom);
        ui->plotSensorZoom->xAxis->setRange(t_angle_zoom.first(), t_angle_zoom.last());
        ui->plotSensorZoom->yAxis->setRange(-3, 3);
        ui->plotSensorZoom->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 2));
        ui->plotSensorZoom->replot();

        ui->plotSensorZoom->graph(1)->setData(t_gyro_zoom, y_gyro_zoom);
        ui->plotSensorZoom->xAxis->setRange(t_gyro_zoom.first(), t_gyro_zoom.last());
        ui->plotSensorZoom->yAxis->setRange(-3, 3);
        ui->plotSensorZoom->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 2));
        ui->plotSensorZoom->replot();
    };

    // Connect the textChanged signal to the callback
    connect(ui->buttonPlotZoom, &QPushButton::released, this, callback_zoomin);
}

MainWindow::~MainWindow()
{
    delete ui;
}
