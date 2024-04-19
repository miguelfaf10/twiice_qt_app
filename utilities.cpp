#include <numeric>
#include <cmath>

#include <algorithm>
#include <QVector>
#include <QDebug>

double computeMean(const QVector<double>& data) {
    return std::accumulate(data.begin(), data.end(), 0.0) / data.size();
}

double computeStdDev(const QVector<double>& data) {
    double mean = computeMean(data);
    double sq_sum = std::inner_product(data.begin(), data.end(), data.begin(), 0.0);
    double stdDev = std::sqrt(sq_sum / data.size() - mean * mean);
    return stdDev;
}
