#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QtNoidCommon/QtNoidCommon"
#include "QtNoidApp/QtNoidApp"


#include <QElapsedTimer>
#include <QFileDialog>

#include <QShortcut>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    restoreFromAppConfig();
    m_screenshotShortcut = QtNoid::App::Development::initFullDialogGrabShortcut(this);

    setWindowTitle("QtNoid Common Basic Usage");
}

MainWindow::~MainWindow()
{
    appConfig->saveValue("Geometry", saveGeometry());
    appConfig->saveValue("RecentList", txtFolderList());
    appConfig->saveValue("RecentListCurrent", ui->txtFolder->currentText());
    appConfig->saveValue("NameFilters", txtNameFilters());
    appConfig->saveValue("NameFiltersCurrent", ui->txtNameFilters->currentText());
    delete ui;
}

QStringList MainWindow::txtFolderList()
{
    QStringList items;
    auto itemCount = ui->txtFolder->count();
    items.reserve(itemCount);
    for (int ii = 0; ii < itemCount; ++ii) {
        items << ui->txtFolder->itemText(ii);
    }
    return items;
}

QStringList MainWindow::txtNameFilters()
{
    QStringList items;
    auto itemCount = ui->txtNameFilters->count();
    items.reserve(itemCount+1);
    for (int ii = 0; ii < itemCount; ++ii) {
        items << ui->txtNameFilters->itemText(ii);
    }
    auto current = ui->txtNameFilters->currentText();
    if(!current.isEmpty()) {
        items << current;
    }
    items.removeDuplicates();
    return items;
}

void MainWindow::update_txtPathList(const QStringList &list, qint64 elapsedTime)
{
    ui->txtPathList->appendPlainText(list.join('\n'));
    // ui->txtPathList->setPlainText(list.join('\n'));
    ui->txtStats->setText(QString("%1 Files - Elapsed time: %2")
                              .arg(QLocale().toString(list.count()),
                                   QtNoid::Common::Scale::nanoSecsUpToDays(elapsedTime)));
}


void MainWindow::on_cmdSelectFolder_clicked()
{
    QString folder = QFileDialog::getExistingDirectory(this, tr("Select Folder"),
                                                       ui->txtFolder->currentText());
    if (!folder.isEmpty()) {
        ui->txtFolder->addItem(folder);
        ui->txtFolder->setCurrentText(folder);
    }
}



void MainWindow::on_cmdListPathRecursively_clicked()
{
    QString msg = "Scanning folder...";
    auto nameFilters = txtNameFilters();
    if(!nameFilters.isEmpty()) {
        msg += "for files ending with: " + nameFilters.join("; ");
    }
    ui->txtPathList->setPlainText(msg);

    ui->txtStats->clear();
    QApplication::processEvents();

    QElapsedTimer ET;
    ET.start();

    const QStringList res = QtNoid::Common::File::listPathRecursively(
        ui->txtFolder->currentText(), nameFilters);
    ui->txtPathList->appendPlainText("Scanning folder... Done!\nCreating result text...");
    QApplication::processEvents();

    update_txtPathList(res, ET.nsecsElapsed());
}


void MainWindow::on_cmdListSubPathRecursively_clicked()
{
    QString msg = "Generating relative path list...";
    auto nameFilters = txtNameFilters();
    if(!nameFilters.isEmpty()) {
        msg += "for files ending with: " + nameFilters.join("; ");
    }
    ui->txtPathList->setPlainText(msg);
    ui->txtStats->clear();
    QApplication::processEvents();

    QElapsedTimer ET;
    ET.start();

    const QStringList res = QtNoid::Common::File::listSubPathRecursively(
        ui->txtFolder->currentText(), nameFilters);

    ui->txtPathList->appendPlainText("Generating sub folder list... Done!\nCreating result text...");
    QApplication::processEvents();

    update_txtPathList(res, ET.nsecsElapsed());
}


void MainWindow::restoreFromAppConfig()
{
    // Geometry
    restoreGeometry(appConfig->restoreAsByteArray("Geometry", saveGeometry()));

    // Recent folder list
    auto list = appConfig->restoreAsStringList("RecentList", {});
    list.removeDuplicates();
    ui->txtFolder->clear();
    ui->txtFolder->addItems(list);
    auto current = appConfig->restoreAsString("RecentListCurrent", ui->txtFolder->currentText());
    if(current.isEmpty() && list.isEmpty()) {
        // qDebug() << Q_FUNC_INFO;
        current = QtNoid::App::Settings::appExeOrAppBundleDirPath();
    }
    ui->txtFolder->setCurrentText(current);


    list = appConfig->restoreAsStringList("NameFilters", txtNameFilters());
    list.removeDuplicates();
    ui->txtNameFilters->clear();
    ui->txtNameFilters->addItems(list);
    current = appConfig->restoreAsString("NameFiltersCurrent", ui->txtNameFilters->currentText());
    ui->txtNameFilters->setCurrentText(current);

}



void MainWindow::on_cmdClearNameFilters_clicked()
{
    ui->txtNameFilters->clear();
}

