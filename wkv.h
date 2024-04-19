#ifndef WKV_H
#define WKV_H

#include <QVector>
#include <QString>


class WKV
{
    QString sensorName;
    QString unit;
    QVector<double> timeStamps;
    QVector<double> sensorData;

public:
    WKV(const QString sensorName,
        const QString unit);

    QString getSensorName();

    QString getUnit();

    QVector<double> getTimeStamps();

    QVector<double> getSensorData();

    void acquireSensor();
};

#endif // WKV_H






