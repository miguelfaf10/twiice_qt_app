#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "wkv.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    // Declare sensor objects
    WKV hipAngleSensor;
    WKV hipGyroSensor;


    void setupUi();
    void setupGraphs();

    void onButtonAcquire();
    void onButtonZoom();
    void onButtonResample();
    void onButtonRegression();
};
#endif // MAINWINDOW_H
