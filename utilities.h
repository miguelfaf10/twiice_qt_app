#ifndef UTILITIES_H
#define UTILITIES_H

#include <QVector>

void removeOffset(QVector<double> &vector, double offset);

std::pair<int, int> sliceTimeSeries(const QVector<double> &timeSeries, double timeBegin, double timeEnd);

double computeMean(const QVector<double>& data);

double computeStdDev(const QVector<double>& data);

#endif // UTILITIES_H
