#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QtNoidCommon/QtNoidCommon"
#include "QtNoidUiCommon/QtNoidUiCommon"
#include "QtNoidApp/QtNoidApp"


#include <QElapsedTimer>
#include <QFileDialog>

#include <QShortcut>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("QtNoid::Common Basic Usage");
    restoreFromAppConfig();
    m_screenshotShortcut = QtNoid::App::Development::initFullDialogGrabShortcut(this);

}

MainWindow::~MainWindow()
{
    appConfig->saveValue("geometry", saveGeometry());

    auto txtFolderJ = QtNoid::UiCommon::Core::saveComboBoxTextItems(ui->txtFolder);
    appConfig->saveValue("txtFolder", txtFolderJ);
    auto txtNameFiltersJ = QtNoid::UiCommon::Core::saveComboBoxTextItems(ui->txtNameFilters);
    appConfig->saveValue("txtNameFilters", txtNameFiltersJ);

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

QStringList MainWindow::txtNameFilters() const
{
    return QStringList(ui->txtNameFilters->currentText());
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
    restoreGeometry(appConfig->restoreAsByteArray("geometry", saveGeometry()));

    auto txtFolderJ = appConfig->restoreAsJson("txtFolder", {});
    QtNoid::UiCommon::Core::restoreComboBoxTextItems(ui->txtFolder, txtFolderJ);
    auto txtNameFiltersJ = appConfig->restoreAsJson("txtNameFilters", {});
    QtNoid::UiCommon::Core::restoreComboBoxTextItems(ui->txtNameFilters, txtNameFiltersJ);

    if(txtFolderJ.isEmpty()) {
        // qDebug() << Q_FUNC_INFO;
        auto current = QtNoid::App::Core::appExeOrAppBundleDirPath();
        ui->txtFolder->setCurrentText(current);
    }

}



void MainWindow::on_cmdClearNameFilters_clicked()
{
    ui->txtNameFilters->clear();
}

