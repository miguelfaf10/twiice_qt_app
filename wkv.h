#ifndef WKV_H
#define WKV_H

#include <string>
#include <vector>
#include <utility>


class WKV
{
    std::string sensorName;
    std::string unit;
    std::vector<std::pair<double, double>> dataSeries;

public:
    WKV(const std::string& sensorName,
        const std::string& unit,
        const std::vector<std::pair<double, double>>& dataSeries);


    std::string getSensorName();

    std::string getUnit();

    std::vector<std::pair<double, double>> getDataSeries();

};

#endif // WKV_H
