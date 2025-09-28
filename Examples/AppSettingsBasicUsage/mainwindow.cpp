#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QtNoidApp/QtNoidApp"

#include <QShortcut>

using namespace QtNoid::App;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    QString appBundle = Settings::appExeOrAppBundlePath();
    ui->txtAppBundle->setText(appBundle);
    qApp->setApplicationDisplayName("This Is App Settings Basic Usage");
    QString config = Settings::filePathAsAppSibling();
    ui->txtConfigPath->setText(config);

    ui->txtGroupName->setText(Settings::groupNameFromObjectOrClass(this));

    m_screenshotShortcut = Settings::initFullDialogGrabShortcut(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_cmdGroupName_clicked()
{
    this->setObjectName("AppSettingsBasicUsage");
    ui->txtGroupName->setText(Settings::groupNameFromObjectOrClass(this));
}

void MainWindow::on_optUpdateWindowTitle_clicked(bool checked)
{
    // I use the central widget as the simplest way to get the main window.
    Settings::updateMainWindowTitle(checked, ui->centralwidget);
}

void MainWindow::on_cmdFullDialogGrab_clicked()
{
    // Label Toggle
    auto text = ui->cmdFullDialogGrab->text();
    if(text.startsWith("Clear")) {
        ui->cmdFullDialogGrab->setText(text.remove("Clear").trimmed());
        ui->txtFullDialogGrab->clear();
        ui->txtFullDialogGrab->setStyleSheet({});
    }
    else {
        auto pixMap = Settings::fullDialogGrab(this);
        ui->txtFullDialogGrab->setStyleSheet("border: 2px solid blue;");
        ui->txtFullDialogGrab->setScaledContents(true);
        ui->txtFullDialogGrab->setPixmap(pixMap);
        ui->cmdFullDialogGrab->setText("Clear " + text);
    }
}
