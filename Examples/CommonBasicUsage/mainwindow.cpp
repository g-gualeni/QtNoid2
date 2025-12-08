#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QtNoidCommon/QtNoidCommon"
#include "QtNoidApp/QtNoidApp"


#include <QElapsedTimer>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initFromAppConfig();


    setWindowTitle("Common Basic Usage");
    ui->txtFolder->setText(qApp->applicationFilePath());
}

MainWindow::~MainWindow()
{
    appConfig->saveValue("Geometry", saveGeometry());
    delete ui;
}

void MainWindow::on_cmdSelectFolder_clicked()
{
    QString folder = QFileDialog::getExistingDirectory(this, tr("Select Folder"), ui->txtFolder->text());
    if (!folder.isEmpty()) {
        ui->txtFolder->setText(folder);
    }
}

void MainWindow::update_txtPathList(const QStringList &list, qint64 elapsedTime)
{
    ui->txtPathList->setPlainText(list.join('\n'));
    ui->txtStats->setText(QString("%1 Files - Elapsed time: %2")
                              .arg(QLocale().toString(list.count()),
                                   QtNoid::Common::Scale::nanoSecsUpToDays(elapsedTime)));
}


void MainWindow::on_cmdListPathRecursively_clicked()
{
    ui->txtPathList->setPlainText("Scanning folder...");
    ui->txtStats->clear();
    QApplication::processEvents();

    QElapsedTimer ET;
    ET.start();
    const QStringList res = QtNoid::Common::File::listPathRecursively(ui->txtFolder->text());
    update_txtPathList(res, ET.nsecsElapsed());
}


void MainWindow::on_cmdListSubPathRecursively_clicked()
{
    ui->txtPathList->setPlainText("Scanning folder...");
    ui->txtStats->clear();
    QApplication::processEvents();

}


void MainWindow::initFromAppConfig()
{
    // Geometry
    restoreGeometry(appConfig->restoreAsByteArray("Geometry", saveGeometry()));
}

