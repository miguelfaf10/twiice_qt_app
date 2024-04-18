#include <vector>
#include <string>

#include <QString>
#include <QStringList>
#include <QProcess>
#include <QObject>



std::vector<double> getVectorDataFromPython()
{

    QObject *parent;
    QString program = "generate_sensor1_data.py";
    QStringList arguments;
    arguments << "sensor";

    QProcess *process = new QProcess(parent);

    process->start(program, arguments);

    // blocking until process has finished
    // timeout = 10s
    if (process->waitForFinished(10000)){

    };

    std::vector<double> result {10,10,10};

    return result;
}
