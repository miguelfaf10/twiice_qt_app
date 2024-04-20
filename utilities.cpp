#include <numeric>
#include <algorithm>
#include <QVector>
#include <QDebug>

double computeMean(const QVector<double>& data) {
    return std::accumulate(data.begin(), data.end(), 0.0) / data.size();
}

QVector<double> subtractMeanFromVector(const QVector<double>& data){

    QVector<double> result(data.size());
    double mean = computeMean(data);

    std::transform(data.begin(), data.end(), result.begin(),
                   [mean](double value) { return value - mean; });

    return result;
}

std::pair<double, double> linearRegression(QVector<double> depVar, QVector<double> indVar)
{

    QVector<double> deltaDepVar = subtractMeanFromVector(depVar);
    QVector<double> deltaIndVar = subtractMeanFromVector(indVar);

    long double aux1 = std::inner_product(deltaIndVar.begin(), deltaIndVar.end(), deltaDepVar.begin(), 0.0);
    long double aux2 = std::inner_product(deltaIndVar.begin(), deltaIndVar.end(), deltaIndVar.begin(), 0.0);

    long double beta = aux1 / aux2;
    long double alfa = computeMean(depVar) - beta * computeMean(indVar);

    return std::pair(beta, alfa);

}
