#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "presetsdialog.h"
#include <QJsonObject>
#include <QDebug>
#include <QScrollBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // ui->txtLog->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    setWindowTitle("QtNoid::App::Parameter Basic Usage");
    restoreGeometry(appConfig->restoreAsByteArray("geometry", saveGeometry()));
    ui->splitter->restoreState(appConfig->restoreAsByteArray("splitter", ui->splitter->saveState()));
    m_screenshotShortcut = QtNoid::App::Development::initFullDialogGrabShortcut(this);

    ui->optIsValueChanged->setEnabled(false);
    ui->optSchemaChanged->setEnabled(false);

    // Listening to UI modifications
    connect(ui->txtName, &QLineEdit::textChanged, this,
            [&](const QString& val){
                updateStatusBar("ui->txtName::textChanged", val);
                m_parameter.setName(val);
            });
    connect(ui->txtLabel, &QLineEdit::textChanged, this,
            [&](const QString& val){
                updateStatusBar("ui->txLabel::textChanged", val);
                m_parameter.setLabel(val);
            });

    connect(ui->txtDescription, &QLineEdit::textChanged, this,
            [&](const QString& val){
                updateStatusBar("ui->txtDescription::textChanged", val);
                m_parameter.setDescription(val);
            });
    connect(ui->txtTooltip, &QLineEdit::textChanged, this,
            [&](const QString& val){
                updateStatusBar("ui->txtTooltip::textChanged", val);
                m_parameter.setTooltip(val);
            });
    connect(ui->txtRange, &QLineEdit::textChanged, this,
            [&](const QString& val){
                updateStatusBar("ui->txtRange::textChanged", val);
                setRangeFromText(val);
            });
    connect(ui->txtUnit, &QLineEdit::textChanged, this,
            [&](const QString& val){
                updateStatusBar("ui->txtUnit::textChanged", val);
                m_parameter.setUnit(val);
            });
    connect(ui->txtValue, &QDoubleSpinBox::valueChanged, this,
            [&](double val){
                updateStatusBar("ui->txtValue::valueChanged", QString::number(val));
                m_parameter.setValue(val);
            });
    connect(ui->optReadOnly, &QCheckBox::clicked, this,
            [&](bool val){
                updateStatusBar("ui->optReadOnly::clicked", QString(val? "true" : "false"));
                m_parameter.setReadOnly(val);
            });
    connect(ui->optVisible, &QCheckBox::clicked, this,
            [&](bool val){
                updateStatusBar("ui->optVisible::clicked", QString(val? "true" : "false"));
                m_parameter.setVisible(val);
            });
    connect(ui->cboPresets, &QComboBox::activated, this,
             [&](){
                auto presetName = ui->cboPresets->currentText().split(" ").first();
                QVariant res = m_parameter.applyPreset(presetName);
                updateStatusBar("ui->cboPresets::currentIndexChanged Res ", res.toString());
            });

    // -------------------------------------
    // Listening to m_paramter modifications
    connect(&m_parameter, &QtNoid::App::Parameter::isSchemaModifiedChanged, this,
            [&](bool val){
                ui->optSchemaChanged->setChecked(val);
                updateStatusBar("isSchemaModifiedChanged signal fired", QString(val ? "true" : "false"));
            });
    connect(&m_parameter, &QtNoid::App::Parameter::nameChanged, this,
            [&](QString val){
                ui->txtName->setText((val));
                updateStatusBar("QtNoid::App::Parameter::nameChanged", val);
                convertToJson();
            });
    connect(&m_parameter, &QtNoid::App::Parameter::labelChanged, this,
            [&](QString val){
                ui->txtLabel->setText((val));
                updateStatusBar("QtNoid::App::Parameter::labelChanged", val);
                convertToJson();
            });
    connect(&m_parameter, &QtNoid::App::Parameter::descriptionChanged, this,
            [&](QString val){
                ui->txtDescription->setText((val));
                updateStatusBar("QtNoid::App::Parameter::descriptionChanged", val);
                convertToJson();
            });
    connect(&m_parameter, &QtNoid::App::Parameter::tooltipChanged, this,
            [&](QString val){
                ui->txtTooltip->setText((val));
                updateStatusBar("QtNoid::App::Parameter::tooltipChanged", val);
                convertToJson();
            });
    connect(&m_parameter, &QtNoid::App::Parameter::rangeChanged, this,
            [&](const QVariant& min, const QVariant& max){
                QString range = QString("%1 %2").arg(min.toString(), max.toString());
                ui->txtRange->setText(range);
                updateStatusBar("QtNoid::App::Parameter::rangeChanged", range);
                convertToJson();
            });
    connect(&m_parameter, &QtNoid::App::Parameter::unitChanged, this,
            [&](QString val){
                ui->txtUnit->setText((val));
                updateStatusBar("QtNoid::App::Parameter::unitChanged", val);
                convertToJson();
            });
    connect(&m_parameter, &QtNoid::App::Parameter::valueChanged, this,
            [&](QVariant val){
                ui->txtValue->setValue(val.toDouble());
                updateStatusBar("QtNoid::App::Parameter::valueChanged", val.toString());
                convertToJson();
            });
    connect(&m_parameter, &QtNoid::App::Parameter::readOnlyChanged, this,
            [&](bool val){
                ui->optReadOnly->setChecked(val);
                updateStatusBar("QtNoid::App::Parameter::readOnlyChanged", QString(val ? "true" : "false"));
                convertToJson();
            });    
    connect(&m_parameter, &QtNoid::App::Parameter::visibleChanged, this,
            [&](bool val){
                ui->optVisible->setChecked(val);
                updateStatusBar("QtNoid::App::Parameter::visibleChanged", QString(val ? "true" : "false"));
                convertToJson();
            });
    connect(&m_parameter, &QtNoid::App::Parameter::isValueModifiedChanged, this,
            [&](bool val){
                ui->optIsValueChanged->setChecked(val);
                updateStatusBar("QtNoid::App::Parameter::optIsValueChanged", QString(val? "true" : "false"));
            });
    connect(&m_parameter, &QtNoid::App::Parameter::presetsChanged, this,
            [&](const QVariantMap& presets){
                UiUpdatePresetList(presets);
                updateStatusBar("QtNoid::App::Parameter::presetsChanged", presets.keys().join(" "));
                convertToJson();
            });

    // Listening to m_paramter errors
    connect(&m_parameter, &QtNoid::App::Parameter::writeAttemptedWhileReadOnly, this,
            [&](const QString& val, const QVariant& current) {
            updateStatusBar("QtNoid::App::Parameter::writeAttemptedWhileReadOnly", current.toString());
            ui->txtValue->setValue(current.toFloat());
    });

    updateFromUi();
}

MainWindow::~MainWindow()
{
    appConfig->saveValue("geometry", saveGeometry());
    appConfig->saveValue("splitter", ui->splitter->saveState());
    delete ui;
}

void MainWindow::updateFromUi()
{
    m_parameter.setName(ui->txtName->text());
    m_parameter.setLabel(ui->txtLabel->text());
    m_parameter.setDescription(ui->txtDescription->text());
    m_parameter.setTooltip(ui->txtTooltip->text());
    setRangeFromText(ui->txtRange->text());
    m_parameter.setUnit(ui->txtUnit->text());
    m_parameter.setValue(ui->txtValue->value());
    m_parameter.setReadOnly(ui->optReadOnly->checkState());
    m_parameter.setVisible(ui->optVisible->checkState());

    // Reset the modified flag
    m_parameter.setValueModified(false);
    m_parameter.setSchemaModified(false);

}

void MainWindow::UiUpdatePresetList(const QVariantMap& presets)
{
    const QSignalBlocker blocker(ui->cboPresets);
    QStringList lines{{"None"}};
    for (auto it = presets.constBegin(); it != presets.constEnd(); ++it) {
        lines << QString("%1 %2").arg(it.key(), it.value().toString());
    }
    ui->cboPresets->clear();
    ui->cboPresets->addItems(lines);
}

void MainWindow::on_cmdEditPresets_clicked()
{
    PresetsDialog dlg(this);
    const auto presets = m_parameter.presets();
    if(!presets.isEmpty()) {
        QStringList lines;
        for (auto it = presets.constBegin(); it != presets.constEnd(); ++it) {
            lines << QString("%1 %2").arg(it.key(), it.value().toString());
        }
        dlg.setPresetsText(lines.join('\n'));
    }
    if (dlg.exec() == QDialog::Accepted) {
        setPresetsFromText(dlg.presetsText());
    }
}


void MainWindow::convertToJson()
{
    QJsonDocument schemaDoc(m_parameter.toJsonSchema());
    QString schemaString = schemaDoc.toJson();  //
    ui->txtJsonSchema->setPlainText(schemaString);

    QJsonDocument valueDoc(m_parameter.toJsonValue());
    QString valueString = valueDoc.toJson();  //
    ui->txtJsonValue->setPlainText(valueString);
}

void MainWindow::on_cmdToJson_clicked()
{
    convertToJson();
    updateStatusBar("-> toJsonValue() - toJsonSchema()");
}


void MainWindow::on_cmdFromJson_clicked()
{
    // QSignalBlocker blocker(&m_parameter);

    QString resMsg("-> fromJson(schema, value) ");
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
    QVariantMap presets;
    const QStringList lines  = val.split('\n', Qt::SkipEmptyParts);

    for(const QString& line : lines) {
        const QStringList parts = line.simplified().split(' ', Qt::SkipEmptyParts);
        if (parts.size() < 2)
            continue;                       // malformed line

        bool ok = false;
        const double value = parts.last().toDouble(&ok);
        if (!ok)
            continue;       // it is not a numeric value

        presets.insert(parts.first(), value);
    }
    m_parameter.setPresets(presets);
}

void MainWindow::updateStatusBar(const QString &msg, const QString &value)
{
    QTextCursor cursor(ui->txtLog->document());
    cursor.movePosition(QTextCursor::End);

    if (!ui->txtLog->document()->isEmpty())
        cursor.insertBlock();  // Add a new row after

    QTextCharFormat msgFormat;
    msgFormat.setForeground(Qt::darkGray);
    msgFormat.setFontWeight(QFont::Bold);
    cursor.insertText(msg, msgFormat);

    if(!value.isEmpty()){
        cursor.insertText(": ", msgFormat);
    }

    QTextCharFormat valueFormat;
    valueFormat.setForeground(QColor(Qt::blue));
    cursor.insertText(value, valueFormat);

    // Scroll to the end without changing user selection
    auto *sb = ui->txtLog->verticalScrollBar();
    sb->setValue(sb->maximum());
}

void MainWindow::on_cmdQDebug_clicked()
{
    qDebug() << Q_FUNC_INFO << m_parameter;
}

void MainWindow::on_cmdResetValue_clicked()
{
    updateStatusBar("resetValueModifications triggered");
    m_parameter.resetValueModifications();
}

void MainWindow::on_cmdResetSchema_clicked()
{
    updateStatusBar("resetSchemaModifications triggered");
    m_parameter.resetSchemaModifications();
}

