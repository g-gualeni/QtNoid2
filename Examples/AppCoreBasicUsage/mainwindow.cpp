#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QtNoidApp/QtNoidApp"

#include <QShortcut>
#include <QtNoidCommon/text.h>

using namespace QtNoid::App;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    QString appBundleFolderPath = Core::appExeOrAppBundleDirPath();
    ui->txtAppBundleDirPath->setText(appBundleFolderPath);

    QString appBundleFilePath = Core::appExeOrAppBundleFilePath();
    ui->txtAppBundleFilePath->setText(appBundleFilePath);

    setWindowTitle("QtNoid::App::Core Basic Usage");

    QString config = Core::filePathAsAppSibling();
    ui->txtConfigPath->setText(config);
    ui->txtFullDialogGrab->setText({});

    ui->txtGroupName->setText(Core::groupNameFromObjectOrClass(this));

    m_screenshotShortcut = Development::initFullDialogGrabShortcut(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_cmdGroupName_clicked()
{
    // qDebug() << Q_FUNC_INFO;
    auto newName = QtNoid::Common::Text::convertToCamelCase(ui->txtGroupName->text());
    this->setObjectName(newName);
    ui->txtGroupName->setText(Core::groupNameFromObjectOrClass(this));
}

void MainWindow::on_optUpdateWindowTitle_clicked(bool checked)
{
    // I use the central widget as the simplest way to get the main window.
    Core::updateMainWindowTitle(checked, ui->centralwidget);
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
        auto pixMap = Core::fullDialogGrab(this);
        ui->txtFullDialogGrab->setStyleSheet("border: 2px solid blue;");
        ui->txtFullDialogGrab->setScaledContents(true);
        ui->txtFullDialogGrab->setPixmap(QPixmap::fromImage(pixMap));
        ui->cmdFullDialogGrab->setText("Clear " + text);
    }
}
