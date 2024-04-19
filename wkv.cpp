#include "wkv.h"
#include "python_binder.h"


// Constructor
WKV::WKV(QString sensorName, QString unit) : sensorName(sensorName), unit(unit) {}

// Getter functions
QString WKV::getSensorName()  {
    return sensorName;
}

QString WKV::getUnit() {
    return unit;
}

QVector<double> WKV::getTimeStamps() {
    return timeStamps;
}

QVector<double> WKV::getSensorData() {
    return sensorData;
}

void WKV::acquireSensor() {

    QString python_script = "generate_sensor_data.py";

    std::pair<QVector<double>, QVector<double>> output;

    if (getSensorName() == "hip_joint_angle"){
        output = getSensorDataFromPython(python_script , "hip_joint_angle");
    }
    else if (getSensorName() == "hip_joint_gyro"){
        output = getSensorDataFromPython(python_script , "hip_joint_gyro");
    };


    timeStamps = output.first;
    sensorData = output.second;

}






