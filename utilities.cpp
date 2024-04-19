#include <numeric>
#include <cmath>

#include <algorithm>
#include <QVector>
#include <QDebug>

void removeOffset(QVector<double> &vector, double offset){

    for (int i = 0; i < vector.size(); ++i) {
        vector[i] -= offset;
    }
}

std::pair<int, int> sliceTimeSeries(const QVector<double> &timeSeries, double timeBegin, double timeEnd) {

    // Find the lower bound for timeBegin
    auto it_begin = std::lower_bound(timeSeries.begin(), timeSeries.end(), timeBegin);
    int index_begin = std::distance(timeSeries.begin(), it_begin);

    // Find the upper bound for timeEnd
    auto it_end = std::upper_bound(timeSeries.begin(), timeSeries.end(), timeEnd);
    int index_end = std::distance(timeSeries.begin(), it_end);

    qDebug() << "Slicing timeseries" << index_begin << "(" << *it_begin << ")" << index_end << "(" << *it_end << ")\n";

    return std::make_pair(index_begin, index_end);
}


double computeMean(const QVector<double>& data) {
    return std::accumulate(data.begin(), data.end(), 0.0) / data.size();
}

double computeStdDev(const QVector<double>& data) {
    double mean = computeMean(data);
    double sq_sum = std::inner_product(data.begin(), data.end(), data.begin(), 0.0);
    double stdDev = std::sqrt(sq_sum / data.size() - mean * mean);
    return stdDev;
}
