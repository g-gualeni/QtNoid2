#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QtNoidCommon/QtNoidCommon"
#include "QtNoidApp/QtNoidApp"

#include <QJsonObject>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("App Parameter List Benchmark");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_cmdGONew_clicked()
{
    int iterations = ui->txtIterationsNew->text().toInt();
    auto ns = benchmarkParameterUsingNewAndDelete(iterations);
    auto singleRunTime = ns / iterations;
    auto txt = QString("Total Time: %1, singleTime: %2")
                   .arg(QtNoid::Common::Scale::nanoSecsUpToDays(ns),
                        QtNoid::Common::Scale::nanoSecsUpToDays(singleRunTime));

    ui->txtElapsedTimeNew->setText(txt);
}

quint64 MainWindow::benchmarkParameterUsingNewAndDelete(int iterations)
{
    QElapsedTimer ET;
    ET.start();

    for(int ii = 0; ii < iterations; ii++)
    {
        auto param = new QtNoid::App::Parameter("Name", "test object creation", ii, this);
        param->setRange(-10000000, 10000000);
        param->setReadOnly(true);
        delete param;
    }

    return ET.nsecsElapsed();
}





void MainWindow::on_cmdGOJson_clicked()
{
    int iterations = ui->txtIterationsJson->text().toInt();
    auto ns = benchmarkParameterUsingJSON(iterations);
    auto singleRunTime = ns / iterations;
    auto txt = QString("Total Time: %1, singleTime: %2")
                   .arg(QtNoid::Common::Scale::nanoSecsUpToDays(ns),
                        QtNoid::Common::Scale::nanoSecsUpToDays(singleRunTime));

    ui->txtElapsedTimeJson->setText(txt);

}
quint64 MainWindow::benchmarkParameterUsingJSON(int iterations)
{
    QElapsedTimer ET;
    ET.start();

    QJsonObject value;
    value["Test"] = 12345;

    QJsonObject schema;
    QJsonObject schemaParameters;
    schemaParameters["description"] = "Current temperature";
    schemaParameters["unit"] = "°C";
    schemaParameters["readOnly"] = false;
    schemaParameters["min"] = -273.15;
    schemaParameters["max"] = 1000.0;
    schema["Temperature"] = schemaParameters;

    for(int ii = 0; ii < iterations; ii++)
    {
        QtNoid::App::Parameter p(schema, value, this);
        Q_UNUSED(p)
    }

    return ET.nsecsElapsed();
}

