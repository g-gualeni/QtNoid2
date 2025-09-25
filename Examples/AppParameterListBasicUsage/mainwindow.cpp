#include "mainwindow.h"
#include "./ui_mainwindow.h"


#include <QDoubleSpinBox>
#include <QLabel>
#include <QLineEdit>
#include <QTabWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createList();
    createUiFromList();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createList()
{
    m_list.setName("Setup");

    QtNoid::App::Parameter* par = nullptr;

    par = m_list.emplace(123, "Counter");
    par->setReadOnly(true);

    par = m_list.emplace("test.ini", "FileName", "Configuration File Name");
    par->setPreset("Default", "test.ini");

    par = m_list.emplace(10.5, "Light", "Current Light Intensity");
    par->setTooltip("Light Intensity tooltip");
    par->setPreset("Default", 10.5);
    par->setRange(0,100);

    par = m_list.emplace(11, "Dark", "Dark Light Intensity");
    par->setPreset("Default", 11);
    par->setRange(0,100);

    par = m_list.emplace(12, "Bright", "Bright Light Intensity");
    par->setPreset("Default", 12);
    par->setRange(10,200);
}

void MainWindow::createUiFromList()
{
    QLayout* currentLayout = nullptr;
    QWidget* currentCentral = centralWidget();
    if (currentCentral) {
        // Salva il contenuto esistente se necessario
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

    const auto& parameters = m_list.parameters();
    for(const auto par : parameters) {
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
        connect(spinInt, &QSpinBox::valueChanged, par, &QtNoid::App::Parameter::onValueChanged);
        connect(par, &QtNoid::App::Parameter::valueChanged, this,
                [=](const QVariant &val){
                    spinInt->setValue(val.toInt());   });
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
        connect(spinDouble, &QDoubleSpinBox::valueChanged, par, &QtNoid::App::Parameter::onValueChanged);
        connect(par, &QtNoid::App::Parameter::valueChanged, this,
                [=](const QVariant &val){
                    spinDouble->setValue(val.toDouble());  });
        spin = spinDouble;
    }
    else {
        auto text = new QLineEdit();
        text->setText(par->value().toString());
        text->setReadOnly(par->readOnly());
        connect(text, &QLineEdit::textEdited, par, &QtNoid::App::Parameter::onValueChanged);
        connect(par, &QtNoid::App::Parameter::valueChanged, this,
                [=](const QVariant &val){
                    text->setText(val.toString());   });
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






void MainWindow::on_cmdQDebug_clicked()
{
    qDebug() << m_list;
}


void MainWindow::on_cmdDefault_clicked()
{
    m_list.applyPreset("Default");
}

