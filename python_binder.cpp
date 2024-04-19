#include <QString>
#include <QStringList>
#include <QProcess>
#include <QObject>
#include <QVector>
#include <QPair>
#include <QDebug>

QString PYTHON_SCRIPT_PATH = "C:\\Users\\miguel\\OneDrive\\Documentos\\Qt\\Hip_Joint_Dynamics\\";

std::pair<QVector<double>, QVector<double>> getSensorDataFromPython(QString python_script, QString sensor_name)
{

    QProcess *process = new QProcess();


    // Set the environment for the process
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

    // Required in order to used System's python installation instead of Qt one which may not be present
    env.remove("PYTHONHOME");
    env.remove("PYTHONPATH");
    process->setProcessEnvironment(env);

    process->setWorkingDirectory(PYTHON_SCRIPT_PATH);

    QString program = "python.exe";

    QStringList arguments;
    arguments << python_script << sensor_name;

    qDebug() << "Running system command: " << program << arguments.join((QString)" ") << "\n";

    process->start(program, arguments);

    // blocking until process has finished
    // timeout = 10s
    QString output_data;

    if (process->waitForFinished(10000))
    {
        output_data = process->readAllStandardOutput();
        qDebug() << process->readAllStandardError();
    }
    else
    {
        qDebug() << "Python script execution did not finish within the timeout";
    };

    QVector<double> time_series;
    QVector<double> sensor_data;

    // Read output produced by Python script
    QStringList lines = output_data.split("\n");

    // Separate lines into two columns (first: timestamp; second: data)
    for (const QString &line : lines) {
        QStringList values = line.split(",");
        if (values.size() == 2) {
            time_series.push_back(values[0].toDouble());
            sensor_data.push_back(values[1].toDouble());
        }
    }

    std::pair<QVector<double>, QVector<double>> result = std::make_pair(time_series, sensor_data);

    return result;
}
