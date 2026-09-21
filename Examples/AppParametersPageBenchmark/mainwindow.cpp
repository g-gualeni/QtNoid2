#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QtNoidCommon/QtNoidCommon"
#include "QtNoidApp/QtNoidApp"
#include "ui_mainwindow.h"

#include <QIntValidator>
#include <QJsonArray>
#include <QJsonObject>
#include <QElapsedTimer>
#include <QTimer>
#include <QStyle>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    restoreGeometry(appConfig->restoreAsByteArray("Geometry", saveGeometry()));
    setWindowTitle("QtNoid::App::ParametersPage Benchmark");
    m_screenshotShortcut = QtNoid::App::Development::initFullDialogGrabShortcut(this);

}

MainWindow::~MainWindow()
{
    appConfig->saveValue("Geometry", saveGeometry());
    delete ui;
}

void MainWindow::setCmdGORunning(QPushButton *btn, bool running)
{
    if(running) {
        btn->setStyleSheet(R"(
            background-color: #2e7d32;
            color: white;
            font-weight: bold;
            border: 1px solid #1b5e20;
            border-radius: 3px;
        )");
    }
    else {
        btn->setStyleSheet("");
    }
    btn->style()->unpolish(btn);   // force Qt to re-evaluate the stylesheet
    btn->style()->polish(btn);
    btn->repaint();
}


void MainWindow::on_cmdGONew_clicked()
{
    setCmdGORunning(ui->cmdGONew, true);
    QTimer::singleShot(300, this, [this] { setCmdGORunning(ui->cmdGONew, false); });

    int iterations = ui->txtIterationsNew->value();
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
    setCmdGORunning(ui->cmdGOJson, true);
    QTimer::singleShot(300, this, [this] { setCmdGORunning(ui->cmdGOJson, false); });

    int iterations = ui->txtIterationsJson->value();
    auto [ns1, ns2] = benchmarkParameterUsingJsonValueAndSchema(iterations);
    auto singleRunTime = ns2 / iterations;
    auto txt = QString("Preparation %1 Total Time: %2, singleTime: %3")
                   .arg(QtNoid::Common::Scale::nanoSecsUpToDays(ns1),
                       QtNoid::Common::Scale::nanoSecsUpToDays(ns2),
                        QtNoid::Common::Scale::nanoSecsUpToDays(singleRunTime));

    ui->txtElapsedTimeJson->setText(txt);
    ui->txtElapsedTimeJson->setEnabled(true);
}
void MainWindow::on_cmdGOJsonList_clicked()
{
    setCmdGORunning(ui->cmdGOJsonList, true);
    QTimer::singleShot(300, this, [this] { setCmdGORunning(ui->cmdGOJsonList, false); });

    int parametersCount = ui->txtParametersCountJson->text().toInt();
    auto ns = benchmarkParametersPageUsingJSON(parametersCount);
    auto singleRunTime = ns / parametersCount;
    auto txt = QString("Total Time: %1, AverageParameterTime: %2")
                   .arg(QtNoid::Common::Scale::nanoSecsUpToDays(ns),
                        QtNoid::Common::Scale::nanoSecsUpToDays(singleRunTime));

    ui->txtElapsedTimeJsonList->setText(txt);
    ui->txtElapsedTimeJsonList->setEnabled(true);
}

void MainWindow::on_cmdGOToJson_clicked()
{
    int parametersCount = ui->txtParametersCountJson->text().toInt();
    auto ns = benchmarkParameterListToJSON(parametersCount);
    auto singleRunTime = ns / parametersCount;
    auto txt = QString("Total Time: %1, AverageParameterTime: %2")
                   .arg(QtNoid::Common::Scale::nanoSecsUpToDays(ns),
                        QtNoid::Common::Scale::nanoSecsUpToDays(singleRunTime));

    ui->txtElapsedTimeToJson->setText(txt);
    ui->txtElapsedTimeToJson->setEnabled(true);

}

void MainWindow::on_cmdGOBinding_clicked()
{
    int iterations = ui->txtIterationsCountBinding->text().toInt();
    auto ns = benchmarkBindings(iterations);
    auto singleRunTime = ns / iterations;
    auto txt = QString("Total Time: %1, AverageTime: %2")
                   .arg(QtNoid::Common::Scale::nanoSecsUpToDays(ns),
                        QtNoid::Common::Scale::nanoSecsUpToDays(singleRunTime));

    ui->txtElapsedTimeBinding->setText(txt);
    ui->txtElapsedTimeBinding->setEnabled(true);

}

void MainWindow::on_cmdGOSignalAndSlots_clicked()
{
    int iterations = ui->txtIterationsSignalsAndSlots->text().toInt();
    auto ns = benchmarkSignalsAndSlot(iterations);
    auto singleRunTime = ns / iterations;
    auto txt = QString("Total Time: %1, AverageTime: %2")
                   .arg(QtNoid::Common::Scale::nanoSecsUpToDays(ns),
                        QtNoid::Common::Scale::nanoSecsUpToDays(singleRunTime));

    ui->txtElapsedTimeSignalsAndSlot->setText(txt);
    ui->txtElapsedTimeSignalsAndSlot->setEnabled(true);

}





quint64 MainWindow::benchmarkParameterUsingNewAndDelete(int iterations)
{
    QElapsedTimer ET;
    ET.start();

    for(int ii = 0; ii < iterations; ii++)
    {
        auto param = new QtNoid::App::Parameter(ii, "Name", "test object creation", this);
        param->setRange(-10000000, 10000000);
        param->setReadOnly(true);
        delete param;
    }

    return ET.nsecsElapsed();
}





std::pair<quint64, quint64> MainWindow::benchmarkParameterUsingJsonValueAndSchema(int iterations)
{
    QElapsedTimer ET;
    ET.start();

    QList<QJsonObject> valueList;
    valueList.reserve(iterations);
    for(int ii = 0; ii<iterations; ii++) {
        QJsonObject value;
        value["Test-" + QString::number(ii)] = ii;
        valueList.append(std::move(value));   // o emplace_back
    }

    QJsonObject schema;
    QJsonObject schemaParameters;
    schemaParameters["description"] = "Current temperature";
    schemaParameters["unit"] = "°C";
    schemaParameters["readOnly"] = false;
    schemaParameters["min"] = -273.15;
    schemaParameters["max"] = 1000.0;
    schema["Temperature"] = schemaParameters;

    auto preparation = ET.nsecsElapsed();

    ET.start();
    for(const QJsonObject &val : std::as_const(valueList))
    {
        QtNoid::App::Parameter p(schema, val, this);
        Q_UNUSED(p)
    }

    return {preparation, ET.nsecsElapsed()};
}

quint64 MainWindow::benchmarkParametersPageUsingJSON(int paramtersCount)
{
    QElapsedTimer ET;

    // Create schema JSON for the ParameterList
    QJsonArray schemaArray;
    QJsonArray valueArray;

    ET.start();
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
    qDebug() << __func__ << "Creating schema and value array" << QtNoid::Common::Scale::nanoSecsUpToDays(ET.nsecsElapsed());


    // Create the main schema and value objects
    QJsonObject mainSchema;
    mainSchema["BenchmarkParameterList"] = schemaArray;

    QJsonObject mainValue;
    mainValue["BenchmarkParameterList"] = valueArray;

    ET.start();
    // Create ParametersPage using schema and values
    QtNoid::App::ParametersPage paramList(mainSchema, mainValue, this);

    // Use the parameter list to prevent optimization
    Q_UNUSED(paramList)

    return ET.nsecsElapsed();
}

quint64 MainWindow::benchmarkParameterListToJSON(int paramtersCount)
{
    QElapsedTimer ET;
    QtNoid::App::ParametersPage paramList(this);

    ET.start();
    for(int ii=0; ii < paramtersCount; ii++) {
        paramList.emplace(ii, QString::number(ii), {});
    }
    qDebug() << __func__ << "Creating ParameterList" << QtNoid::Common::Scale::nanoSecsUpToDays(ET.nsecsElapsed());

    ET.start();
    QJsonObject values = paramList.toJsonValues();
    // Use values to prevent optimization
    Q_UNUSED(values)

    return ET.nsecsElapsed();
}

quint64 MainWindow::benchmarkBindings(int iterations)
{
    QElapsedTimer ET;
    QtNoid::App::Parameter leader(0, "leader", this);
    QtNoid::App::Parameter follower(0, "follower", this);

    auto bindableLeader = leader.bindableValue();
    // externalProperty.setBinding([&]() { return par.bindableValue().value(); });
    follower.bindableValue().setBinding([&]() { return bindableLeader.value(); });
    // qDebug() << __func__ << "\n" << leader << "\n" << follower;

    ET.start();
    for(int ii=0; ii<iterations; ++ii) {
        leader.setValue(ii);
        // Check the value
        // if(follower.value() != ii) break;
    }
    return ET.nsecsElapsed();
}

quint64 MainWindow::benchmarkSignalsAndSlot(int iterations)
{
    QElapsedTimer ET;
    QtNoid::App::Parameter leader(0, "leader", this);
    QtNoid::App::Parameter follower(0, "follower", this);

    connect(&leader, &QtNoid::App::Parameter::valueChanged, &follower, &QtNoid::App::Parameter::setValue);

    ET.start();
    for(int ii=0; ii<iterations; ++ii) {
        leader.setValue(ii);
        // if(follower.value() != ii) break;
    }
    return ET.nsecsElapsed();
}








