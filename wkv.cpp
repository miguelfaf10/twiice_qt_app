#include "wkv.h"
#include <string>
#include <vector>
#include <utility>
#include <chrono>



using std::chrono::time_point;


// Constructor
WKV::WKV(const std::string& sensorName, const std::string& unit, const std::vector<std::pair<double, double>>& dataSeries)
    : sensorName(sensorName), unit(unit), dataSeries(dataSeries) {}

// Getter functions
std::string WKV::getSensorName()  {
    return sensorName;
}

std::string WKV::getUnit() {
    return unit;
}

std::vector<std::pair<double, double>> WKV::getDataSeries() {
    return dataSeries;
}


