#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QtNoidCommon/QtNoidCommon"
#include "QtNoidApp/QtNoidApp"

#include <QIntValidator>
#include <QJsonArray>
#include <QJsonObject>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("App Parameter List Benchmark");

    QValidator *validator = new QIntValidator(0, 1000000, this);
    ui->txtIterationsNew->setValidator(validator);
    ui->txtIterationsJson->setValidator(validator);
    ui->txtParametersCountJson->setValidator(validator);


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
    ui->txtElapsedTimeNew->setEnabled(true);
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
    ui->txtElapsedTimeJson->setEnabled(true);
}
void MainWindow::on_cmdGOJsonList_clicked()
{
    int parametersCount = ui->txtParametersCountJson->text().toInt();
    auto ns = benchmarkParameterListUsingJSON(parametersCount);
    auto singleRunTime = ns / parametersCount;
    auto txt = QString("Total Time: %1, AverageParameterTime: %2")
                   .arg(QtNoid::Common::Scale::nanoSecsUpToDays(ns),
                        QtNoid::Common::Scale::nanoSecsUpToDays(singleRunTime));

    ui->txtElapsedTimeJsonList->setText(txt);
    ui->txtElapsedTimeJsonList->setEnabled(true);
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

quint64 MainWindow::benchmarkParameterListUsingJSON(int paramtersCount)
{

    // Create schema JSON for the ParameterList
    QJsonArray schemaArray;
    QJsonArray valueArray;

    for(int i = 0; i < paramtersCount; i++) {
        // Create schema for each parameter
        QJsonObject paramSchema;
        QJsonObject schemaDetails;
        schemaDetails["description"] = QString("Parameter %1 description").arg(i);
        schemaDetails["unit"] = "units";
        schemaDetails["readOnly"] = false;
        schemaDetails["min"] = -1000.0;
        schemaDetails["max"] = 1000.0;

        QString paramName = QString("Param_%1").arg(i);
        paramSchema[paramName] = schemaDetails;
        schemaArray.append(paramSchema);

        // Create value for each parameter
        QJsonObject paramValue;
        paramValue[paramName] = i * 10.0; // Some test value
        valueArray.append(paramValue);
    }

    // Create the main schema and value objects
    QJsonObject mainSchema;
    mainSchema["BenchmarkParameterList"] = schemaArray;

    QJsonObject mainValue;
    mainValue["BenchmarkParameterList"] = valueArray;

    QElapsedTimer ET;
    ET.start();
    // Create ParameterList using schema and values
    QtNoid::App::ParameterList paramList(mainSchema, mainValue, this);

    // Use the parameter list to prevent optimization
    Q_UNUSED(paramList)

    return ET.nsecsElapsed();
}



