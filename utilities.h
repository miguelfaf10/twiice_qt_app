#ifndef UTILITIES_H
#define UTILITIES_H

#include <QVector>

void removeOffset(QVector<double> &vector, double offset);

std::pair<int, int> sliceTimeSeries(const QVector<double> &timeSeries, double timeBegin, double timeEnd);

double computeMean(const QVector<double>& data);

std::pair<double, double> linearRegression(QVector<double> indVar, QVector<double> depVar);

#endif // UTILITIES_H
