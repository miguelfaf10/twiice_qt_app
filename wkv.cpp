#include "wkv.h"
#include "python_binder.h"

#include <QVector>
#include <QString>
#include <QDebug>

// TimeSeries class

// Constructor for TimeSeries
TimeSeries::TimeSeries(QVector<double> timestamps_vector, QVector<double> datavalues_vector){
    if (timestamps_vector.size() == datavalues_vector.size()){
        timestamps = timestamps_vector;
        datavalues = datavalues_vector;
    }
}
TimeSeries::TimeSeries(){}

// Getter functions
QVector<double> TimeSeries::getTimeStamps(){
    return timestamps;
}

QVector<double> TimeSeries::getDataValue(){
    return datavalues;;
}

// Adds a new point to the time series
void TimeSeries::addPoint(double time, double value) {
    timestamps.push_back(time);
    datavalues.push_back(value);
}

// Resamples a time series and returns a new TimeSeries object
TimeSeries TimeSeries::resample(double start, double interval, int nPoints) {
    TimeSeries resampled;
    for (double k = 1; k <= nPoints; k += 1) {
        double newTime = start + k * interval;
        resampled.addPoint(newTime, interpolate(newTime));
    }
    return resampled;
}

// interpolates
double TimeSeries::interpolate(double newTime) {

    // find the closest two points for interpolation
    if (timestamps.empty()) return 0.0;

    // check boundaries
    if (newTime <= timestamps.first()) return datavalues.first();
    if (newTime >= timestamps.last()) return datavalues.last();

    auto lower = std::lower_bound(timestamps.begin(), timestamps.end(), newTime);
    int index = std::distance(timestamps.begin(), lower);

    // Interpolate linearly between index-1 and index
    double t1 = timestamps[index - 1], t2 = timestamps[index];
    double v1 = datavalues[index - 1], v2 = datavalues[index];

    // Linear interpolation formula
    return v1 + (v2 - v1) * (newTime - t1) / (t2 - t1);
}

// remove offset
TimeSeries TimeSeries::removeTimeOffset(){

    TimeSeries newTimeSeries = TimeSeries(timestamps, datavalues);

    double timeOffset = timestamps[0];

    for (int i = 0; i < timestamps.size(); ++i) {
        newTimeSeries.timestamps[i] -= timeOffset;
    }

    return newTimeSeries;
}

// Slice a timeseries based on two time instants
TimeSeries TimeSeries::slice(double timeBegin, double timeEnd) {

    // Find the lower bound for timeBegin
    auto it_begin = std::lower_bound(timestamps.begin(), timestamps.end(), timeBegin);
    int index_begin = std::distance(timestamps.begin(), it_begin);

    // Find the upper bound for timeEnd
    auto it_end = std::upper_bound(timestamps.begin(), timestamps.end(), timeEnd);
    int index_end = std::distance(timestamps.begin(), it_end);

    qDebug() << "Slicing timeseries" << index_begin << "(" << *it_begin << ")" << index_end << "(" << *it_end << ")\n";

    TimeSeries newTimeSeries = TimeSeries(timestamps.mid(index_begin, index_end), datavalues.mid(index_begin, index_end));

    return newTimeSeries;
}

// Class WKV

// Constructor for WKV
WKV::WKV(QString sensorName, QString unit) : sensorName(sensorName), unit(unit) {}

// Getter and setter functions
QString WKV::getSensorName()  {
    return sensorName;
}

QString WKV::getUnit() {
    return unit;
}

TimeSeries WKV::getSensorData(){
    return timeseries;
}

// Process sensor acquisition
void WKV::acquireSensor() {

    QString python_script = "generate_sensor_data.py";

    std::pair<QVector<double>, QVector<double>> output;

    qDebug() << "Acquiring data for sensor:" << getSensorName() << "\n";

    if (getSensorName() == "hip_joint_angle"){
        output = getSensorDataFromPython(python_script , "hip_joint_angle");
    }
    else if (getSensorName() == "hip_joint_gyro"){
        output = getSensorDataFromPython(python_script , "hip_joint_gyro");
    }
    else {
        qDebug() << "Sensor not supported\n";
    };

    if (output.first.size() == output.second.size()){
        timeseries = TimeSeries(output.first, output.second);
    }
    else{
        qDebug() << "Errror acquiring data from sensors." << "\n";
    }

}






