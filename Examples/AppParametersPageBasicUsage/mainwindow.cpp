#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QDoubleSpinBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QTabWidget>
#include <QStyle>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    restoreGeometry(appConfig->restoreAsByteArray("Geometry", saveGeometry()));
    setWindowTitle("QtNoid::App::ParametersPage Basic Usage");
    createList();
    createUiFromList();
    m_screenshotShortcut = QtNoid::App::Development::initFullDialogGrabShortcut(this);
}

MainWindow::~MainWindow()
{
    appConfig->saveValue("Geometry", saveGeometry());
    delete ui;
}

void MainWindow::createList()
{
    m_list.setName("App Setup Page");

    QtNoid::App::Parameter* par = nullptr;

    par = m_list.emplace(123, "Counter", "Integer Counter as read only");
    par->setReadOnly(true);
    par->setPreset("Default01", 123);
    par->setPreset("Default02", 223);
    par->setTooltip("This is a read only counter");

    par = m_list.emplace("test.ini", "FileName", "Configuration File Name as string");
    par->setPreset("Default01", "test.ini");
    par->setPreset("Default02", "results.ini");
    par->setTooltip("This is an example of string containing a file name");

    par = m_list.emplace(10.5, "Light", "Current Light Intensity as Double");
    par->setTooltip("Light Intensity tooltip");
    par->setPreset("Default01", 10.5);
    par->setPreset("Default02", 22.22);
    par->setRange(0,100);

    par = m_list.emplace(11.0, "Dark", "Dark Light Intensity as Double");
    par->setTooltip("This is an example parameter");
    par->setPreset("Default01", 11.0);
    par->setPreset("Default02", 21.0);
    par->setRange(0,100);

    par = m_list.emplace(12, "Font Size", "Font Size as Integer");
    par->setTooltip("This is another example paramter");
    par->setPreset("Default01", 12);
    par->setPreset("Default02", 16);
    par->setRange(10,20);
}

void MainWindow::createUiFromList()
{
    QLayout* currentLayout = nullptr;
    QWidget* currentCentral = centralWidget();
    if (currentCentral) {
        // Preserve existing content
        currentLayout = currentCentral->layout();
    }
    if(!currentLayout) {
        return;
    }
    m_tabWidget = new QTabWidget(this);
    currentLayout->addWidget(m_tabWidget);

    auto tab = createUiTabFromList();
    m_tabWidget->addTab(tab, m_list.name());

}

QWidget* MainWindow::createUiTabFromList()
{
    QWidget* tab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(tab);

    for(const auto par : m_list) {
        layout->addLayout(createUiParamterGroupFromParameter(par, 120));
        auto desc = createUiDescriptionFromParameter(par);
        if(desc != nullptr)
            layout->addWidget(desc);
    }
    layout->addStretch();
    return tab;
}

QLayout *MainWindow::createUiParamterGroupFromParameter(QtNoid::App::Parameter *par, int width)
{
    QHBoxLayout* layout = new QHBoxLayout();
    auto label = new QLabel(par->name());
    label->setFixedWidth(width);
    label->setStyleSheet("font-weight: bold;");
    layout->addWidget(label);

    QWidget* spin = nullptr;
    QVariant val = par->value();
    QMetaType parType = val.metaType();
    if(parType == QMetaType::fromType<int>()){
        auto spinInt = new QSpinBox();
        if(par->rangeIsValid()) {
            spinInt->setMinimum(par->min().toInt());
            spinInt->setMaximum(par->max().toInt());
        }
        else {
            spinInt->setRange(INT_MIN, INT_MAX);
        }
        spinInt->setValue(par->value().toInt());
        spinInt->setReadOnly(par->readOnly());
        connect(spinInt, &QSpinBox::valueChanged, par, &QtNoid::App::Parameter::setValue);
        connect(par, &QtNoid::App::Parameter::valueChanged, this, [=, this](const QVariant &val){
            updateStyleSheet(spinInt, par);
            if(spinInt->value() != val.toInt()){
                spinInt->setValue(val.toInt());
            }
        });
        spin = spinInt;
    }
    else if(parType == QMetaType::fromType<double>()){
        auto spinDouble = new QDoubleSpinBox();
        if(par->rangeIsValid()) {
            spinDouble->setMinimum(par->min().toDouble());
            spinDouble->setMaximum(par->max().toDouble());
        }
        else {
            spinDouble->setRange(-std::numeric_limits<double>::infinity(),
                                 std::numeric_limits<double>::infinity());
        }
        spinDouble->setValue(par->value().toDouble());
        spinDouble->setReadOnly(par->readOnly());
        connect(spinDouble, &QDoubleSpinBox::valueChanged, par, &QtNoid::App::Parameter::setValue);
        connect(par, &QtNoid::App::Parameter::valueChanged, this, [=, this](const QVariant &val){
            updateStyleSheet(spinDouble, par);
            if(spinDouble->value() != val.toDouble()) {
                spinDouble->setValue(val.toDouble());
            }
        });
        spin = spinDouble;
    }
    else {
        auto text = new QLineEdit();
        text->setText(par->value().toString());
        text->setReadOnly(par->readOnly());
        connect(text, &QLineEdit::textEdited, par, &QtNoid::App::Parameter::setValue);
        connect(par, &QtNoid::App::Parameter::valueChanged, this, [=, this](const QVariant &val){
            updateStyleSheet(text, par);
            if(text->text() != val.toString()) {
                text->setText(val.toString());
            }
        });
        spin = text;
    }

    spin->setToolTip(par->tooltip());

    layout->addWidget(spin);
    return layout;
}

QWidget *MainWindow::createUiDescriptionFromParameter(QtNoid::App::Parameter *par)
{
    if(par->description().isEmpty())
        return nullptr;

    QWidget* label = new QLabel(par->description());
    label->setStyleSheet("font-style: italic;");

    return label;
}

void MainWindow::updateStyleSheet(QWidget *spinInt, QtNoid::App::Parameter *par)
{
    QString styleSheet = par->isValueChanged()? "font-weight: bold;" : "font-weight: normal;";
    if(spinInt->styleSheet() != styleSheet){
        spinInt->setStyleSheet(styleSheet);
    }
}

void MainWindow::on_cmdQDebug_clicked()
{
    qDebug() << __func__ << m_list;
}


void MainWindow::on_cmdDefault01_clicked()
{
    m_list.applyPreset("Default01");
}

void MainWindow::on_cmdDefault02_clicked()
{
    m_list.applyPreset("Default02");
}


void MainWindow::on_cmdSaveToJson_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(
        this, "Save values as JSON", "ParametersPage.json",
        "File JSON (*.json)"
        );

    if(fileName.isEmpty())
        return;

    QJsonObject val = m_list.toJsonValues();
    QJsonDocument doc(val);
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
        QString msg = QString("Data saved to %1").arg(fileName);
        QMessageBox::information(this, "Success", msg);
    }
    else {
        QString msg = QString("Error writing to %1").arg(fileName);
        QMessageBox::warning(this, "Error", msg);
    }
}


void MainWindow::on_cmdLoadFromJson_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Load values from JSON",
        "",
        "File JSON (*.json);;Any file (*.*)"
        );

    if(fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QString msg = QString("Error reading to %1").arg(fileName);
        QMessageBox::warning(this, "Error", msg);
        return;
    }

    QByteArray data = file.readAll();
    file.close();
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    if (error.error != QJsonParseError::NoError) {
        QMessageBox::warning(this, "Error",
                             "Invalid JSON file: " + error.errorString());
    }

    bool res = m_list.valuesFromJson(doc.object());
    if (!res) {
        QMessageBox::warning(this, "Error", "Invalid JSON file");
    }

}



