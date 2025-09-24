#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QJsonObject>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("App Parameter Basic Usage");

    connect(ui->txtName, &QLineEdit::textChanged, this,
            [&](const QString& val){m_parameter.setName(val);
                updateStatusBar("txtName::textChanged");
            });
    connect(ui->txtDescription, &QLineEdit::textChanged, this,
            [&](const QString& val){m_parameter.setDescription(val);
                updateStatusBar("txtDescription::textChanged");
            });
    connect(ui->txtTooltip, &QLineEdit::textChanged, this,
            [&](const QString& val){m_parameter.setTooltip(val);
                updateStatusBar("txtTooltip::textChanged");
            });
    connect(ui->txtRange, &QLineEdit::textChanged, this,
            [&](const QString& val){setRangeFromText(val);
                updateStatusBar("txtRange::textChanged");
            });
    connect(ui->txtUnit, &QLineEdit::textChanged, this,
            [&](const QString& val){m_parameter.setUnit(val);
                updateStatusBar("txtUnit::textChanged");
            });
    connect(ui->txtValue, &QDoubleSpinBox::valueChanged, this,
            [&](double val){m_parameter.setValue(val);
                updateStatusBar("txtValue::valueChanged");
            });
    connect(ui->optReadOnly, &QCheckBox::clicked, this,
            [&](bool val){m_parameter.setReadOnly(val);
                updateStatusBar("optReadOnly::clicked");
            });
    connect(ui->optVisible, &QCheckBox::clicked, this,
            [&](bool val){m_parameter.setVisible(val);
                updateStatusBar("optVisible::clicked");
            });
    connect(ui->txtPresets, &QPlainTextEdit::textChanged, this,
            [&](){setPresetsFromText(ui->txtPresets->toPlainText());
                updateStatusBar("txtPresets::textChanged");
            });

    connect(&m_parameter, &QtNoid::App::Parameter::nameChanged, this,
            [&](QString val){
                ui->txtName->setText((val));
                updateStatusBar("QtNoid::App::Parameter::nameChanged");
            });
    connect(&m_parameter, &QtNoid::App::Parameter::descriptionChanged, this,
            [&](QString val){
                ui->txtDescription->setText((val));
                updateStatusBar("QtNoid::App::Parameter::descriptionChanged");
            });
    connect(&m_parameter, &QtNoid::App::Parameter::tooltipChanged, this,
            [&](QString val){
                ui->txtTooltip->setText((val));
                updateStatusBar("QtNoid::App::Parameter::tooltipChanged");
            });
    connect(&m_parameter, &QtNoid::App::Parameter::rangeChanged, this,
            [&](const QVariant& min, const QVariant& max){
                QString range = QString("%1 %2").arg(min.toString(), max.toString());
                ui->txtRange->setText(range);
                updateStatusBar("QtNoid::App::Parameter::rangeChanged");
            });
    connect(&m_parameter, &QtNoid::App::Parameter::unitChanged, this,
            [&](QString val){
                ui->txtUnit->setText((val));
                updateStatusBar("QtNoid::App::Parameter::unitChanged");
            });
    connect(&m_parameter, &QtNoid::App::Parameter::valueChanged, this,
            [&](QVariant val){
                ui->txtValue->setValue(val.toDouble());
                updateStatusBar("QtNoid::App::Parameter::valueChanged");
            });
    connect(&m_parameter, &QtNoid::App::Parameter::readOnlyChanged, this,
            [&](bool val){
                ui->optReadOnly->setChecked(val);
                updateStatusBar("QtNoid::App::Parameter::readOnlyChanged");
            });    
    connect(&m_parameter, &QtNoid::App::Parameter::visibleChanged, this,
            [&](bool val){
                ui->optVisible->setChecked(val);
                updateStatusBar("QtNoid::App::Parameter::visibleChanged");
            });

    connect(&m_parameter, &QtNoid::App::Parameter::presetsChanged, this,
            [&](const QVariantMap &presets){
                QString txt;
                for(auto it = presets.begin(); it != presets.end(); ++it) {
                    QString key = it.key();
                    QVariant value = it.value();
                    txt += QString("%1 %2\n").arg(key, value.toString());
                }
                ui->txtPresets->setPlainText(txt);
                updateStatusBar("QtNoid::App::Parameter::presetsChanged");
            });

    updateFromGui();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateFromGui()
{
    m_parameter.setName(ui->txtName->text());
    m_parameter.setDescription(ui->txtDescription->text());
    m_parameter.setTooltip(ui->txtTooltip->text());
    setRangeFromText(ui->txtRange->text());
    m_parameter.setUnit(ui->txtUnit->text());
    m_parameter.setValue(ui->txtValue->value());
    m_parameter.setReadOnly(ui->optReadOnly->checkState());
    m_parameter.setVisible(ui->optVisible->checkState());
    setPresetsFromText(ui->txtPresets->toPlainText());
}

void MainWindow::on_cmdUpdate_clicked()
{
    updateFromGui();
    updateStatusBar(__func__);
}


void MainWindow::on_cmdToJson_clicked()
{
    QJsonObject schemaObj = m_parameter.toJsonSchema();
    QJsonObject valueObj = m_parameter.toJsonValue();

    QJsonDocument schemaDoc(schemaObj);
    QString schemaString = schemaDoc.toJson();  //
    ui->txtJsonSchema->setPlainText(schemaString);

    QJsonDocument valueDoc(valueObj);
    QString valueString = valueDoc.toJson();  //
    ui->txtJsonValue->setPlainText(valueString);
    updateStatusBar(__func__);
}


void MainWindow::on_cmdFromJson_clicked()
{
    QString resMsg(__func__);
    QJsonParseError parseError;
    auto schemaString = ui->txtJsonSchema->toPlainText();
    QJsonDocument schemaDoc =  QJsonDocument::fromJson(schemaString.toUtf8(), &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        resMsg = "Schema Error:" + parseError.errorString();
    }


    auto valueString = ui->txtJsonValue->toPlainText();
    QJsonDocument valueDoc = QJsonDocument::fromJson(valueString.toUtf8(), &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        resMsg += "Value Error:" + parseError.errorString();
    }

    auto res = m_parameter.fromJson(schemaDoc.object(), valueDoc.object());
    if(!res) {
        resMsg += " Error calling fromJson";
    }
    updateStatusBar(resMsg);
}

void MainWindow::setRangeFromText(const QString &val)
{
    QStringList range = val.split(" ");
    auto min = range.first().toFloat();
    auto max = range.last().toFloat();
    m_parameter.setRange(min, max);
}

void MainWindow::setPresetsFromText(const QString &val)
{
    const QStringList presetTxtList = val.split('\n', Qt::SkipEmptyParts);
    for(const QString& preset : presetTxtList) {
        QStringList nameValue = preset.split(" ");
        double val = nameValue.last().toDouble();
        m_parameter.setPreset(nameValue.first(), val);
    }
}

void MainWindow::updateStatusBar(const QString &msg)
{
    ui->txtLog->appendPlainText(msg);
}


void MainWindow::on_cmdQDebug_clicked()
{
    qDebug() << m_parameter;
}

