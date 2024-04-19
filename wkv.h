#ifndef WKV_H
#define WKV_H

#include <QVector>
#include <QString>


class TimeSeries {

    QVector<double> timestamps;
    QVector<double> datavalues;

    public:

    TimeSeries();
    TimeSeries(QVector<double>timestamps_vector, QVector<double> datavalues_vector);

    TimeSeries removeTimeOffset();

    TimeSeries slice(double timeBegin, double timeEnd);

    TimeSeries resample(double start, double interval, int nPoints);

    void addPoint(double time, double value);

    double interpolate(double newTime);

    QVector<double> getTimeStamps();

    QVector<double> getDataValue();

};

class WKV
{
    QString sensorName;
    QString unit;
    TimeSeries timeseries;

public:
    WKV(const QString sensorName,
        const QString unit);

    QString getSensorName();

    QString getUnit();

    TimeSeries getSensorData();

    void acquireSensor();
};

#endif // WKV_H






