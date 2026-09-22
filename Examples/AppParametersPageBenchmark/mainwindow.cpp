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
    QSize sizehint = btn->sizeHint();
    if(running) {
        btn->setStyleSheet(R"(
            background-color: #2ead32;
            color: white;
            font-weight: bold;
            border: 2px solid #1bce20;
            border-radius: 3px;
        )");
    }
    else {
        btn->setStyleSheet("");
    }

    btn->style()->unpolish(btn);   // force Qt to re-evaluate the stylesheet
    btn->style()->polish(btn);

    btn->setMinimumSize(sizehint);

    btn->repaint();
}


void MainWindow::on_cmdGONew_clicked()
{
    setCmdGORunning(ui->cmdGONew, true);
    QTimer::singleShot(300, this, [this] { setCmdGORunning(ui->cmdGONew, false); });

    int iterations = ui->txtIterationsNew->value();
    auto ns = benchmarkParameterUsingNewAndDelete(iterations);
    auto singleRunTime = ns / iterations;
    auto txt = QString("TotalTime: %1, SingleTime: %2")
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
    auto txt = QString("Preparation %1 TotalTime: %2, SingleTime: %3")
                   .arg(QtNoid::Common::Scale::nanoSecsUpToDays(ns1),
                       QtNoid::Common::Scale::nanoSecsUpToDays(ns2),
                        QtNoid::Common::Scale::nanoSecsUpToDays(singleRunTime));

    ui->txtElapsedTimeJson->setText(txt);
    ui->txtElapsedTimeJson->setEnabled(true);
}


void MainWindow::on_cmdGOParametersPageFromJson_clicked()
{
    setCmdGORunning(ui->cmdGOParametersPageFromJson, true);
    QTimer::singleShot(300, this, [this] { setCmdGORunning(ui->cmdGOParametersPageFromJson, false); });

    int parametersCount = ui->txtParametersCountCreateFromJson->value();
    auto [preparation, ns] = benchmarkParametersPageUsingJSON(parametersCount);
    auto singleRunTime = ns / parametersCount;
    auto txt = QString("Preparation %1 TotalTime: %2, singleTime: %3")
                   .arg(QtNoid::Common::Scale::nanoSecsUpToDays(preparation),
                       QtNoid::Common::Scale::nanoSecsUpToDays(ns),
                        QtNoid::Common::Scale::nanoSecsUpToDays(singleRunTime));

    ui->txtElapsedTimeJsonList->setText(txt);
    ui->txtElapsedTimeJsonList->setEnabled(true);
}

void MainWindow::on_cmdGOToJson_clicked()
{
    setCmdGORunning(ui->cmdGOToJson, true);
    QTimer::singleShot(300, this, [this] { setCmdGORunning(ui->cmdGOToJson, false); });

    int parametersCount = ui->txtParametersCountToJson->value();
    auto [preparation, ns] = benchmarkParameterListToJSON(parametersCount);
    auto singleRunTime = ns / parametersCount;
    auto txt = QString("Preparation %1 TotalTime: %2, SingleTime: %3")
                   .arg(QtNoid::Common::Scale::nanoSecsUpToDays(preparation),
                       QtNoid::Common::Scale::nanoSecsUpToDays(ns),
                        QtNoid::Common::Scale::nanoSecsUpToDays(singleRunTime));

    ui->txtElapsedTimeToJson->setText(txt);
    ui->txtElapsedTimeToJson->setEnabled(true);
}

void MainWindow::on_cmdGOBinding_clicked()
{
    setCmdGORunning(ui->cmdGOBinding, true);
    QTimer::singleShot(300, this, [this] { setCmdGORunning(ui->cmdGOBinding, false); });

    int iterations = ui->txtIterationsCountBinding->value();
    auto ns = benchmarkBindings(iterations);
    auto singleRunTime = ns / iterations;
    auto txt = QString("TotalTime: %1, SingleTime: %2")
                   .arg(QtNoid::Common::Scale::nanoSecsUpToDays(ns),
                        QtNoid::Common::Scale::nanoSecsUpToDays(singleRunTime));

    ui->txtElapsedTimeBinding->setText(txt);
    ui->txtElapsedTimeBinding->setEnabled(true);

}

void MainWindow::on_cmdGOSignalAndSlots_clicked()
{
    setCmdGORunning(ui->cmdGOSignalAndSlots, true);
    QTimer::singleShot(300, this, [this] { setCmdGORunning(ui->cmdGOSignalAndSlots, false); });

    int iterations = ui->txtIterationsSignalsAndSlots->value();
    auto ns = benchmarkSignalsAndSlot(iterations);
    auto singleRunTime = ns / iterations;
    auto txt = QString("TotalTime: %1, SingleTime: %2")
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

std::pair<quint64, quint64> MainWindow::benchmarkParametersPageUsingJSON(int paramtersCount)
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


    // Create the main schema and value objects
    QJsonObject schemaMain;
    schemaMain["parameters"] = schemaArray;
    QJsonObject schemaJson;
    schemaJson["BenchmarkParameterList"] = schemaMain;

    QJsonObject valueMain;
    valueMain["parameters"] = valueArray;
    QJsonObject valueJson;
    valueJson["BenchmarkParameterList"] = valueMain;

    auto preparation = ET.nsecsElapsed();
    // qDebug() << __func__ << "Creating schema and value array" << QtNoid::Common::Scale::nanoSecsUpToDays(ET.nsecsElapsed());
    // qDebug() << __func__ << valueArray.last() << schemaArray.last();

    ET.start();
    // Create ParametersPage using schema and values
    QtNoid::App::ParametersPage paramsPage(schemaJson, valueJson, this);

    // qDebug() << __func__ << paramsPage.count() << *paramsPage.rbegin();

    // Use the parameter list to prevent optimization
    Q_UNUSED(paramsPage)

    return {preparation, ET.nsecsElapsed()};
}

std::pair<quint64, quint64> MainWindow::benchmarkParameterListToJSON(int paramtersCount)
{
    QElapsedTimer ET;
    QtNoid::App::ParametersPage paramList(this);

    ET.start();
    for(int ii=0; ii < paramtersCount; ii++) {
        paramList.emplace(ii, QString::number(ii), {});
    }
    auto preparation = ET.nsecsElapsed();

    ET.start();
    QJsonObject values = paramList.toJsonValues();

    // Use values to prevent optimization
    Q_UNUSED(values)


    return {preparation, ET.nsecsElapsed()};
}

quint64 MainWindow::benchmarkBindings(int iterations)
{
    QElapsedTimer ET;
    QtNoid::App::Parameter leader(0, "leader", this);
    QtNoid::App::Parameter follower(0, "follower", this);

    auto bindableLeader = leader.bindableValue();
    follower.bindableValue().setBinding([&]() { return bindableLeader.value(); });

    ET.start();
    for(int ii=0; ii<iterations; ++ii) {
        leader.setValue(ii);
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










