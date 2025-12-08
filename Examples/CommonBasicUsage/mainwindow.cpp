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
    initFromAppConfig();
    m_screenshotShortcut = QtNoid::App::Development::initFullDialogGrabShortcut(this);

    setWindowTitle("Common Basic Usage");    
}

MainWindow::~MainWindow()
{
    appConfig->saveValue("Geometry", saveGeometry());
    appConfig->saveValue("RecentList", txtFolderList());
    delete ui;
}

QStringList MainWindow::txtFolderList()
{
    QStringList items;
    items.reserve(ui->txtFolder->count());
    for (int i = 0; i < ui->txtFolder->count(); ++i) {
        items << ui->txtFolder->itemText(i);
    }
    return items;
}

void MainWindow::update_txtPathList(const QStringList &list, qint64 elapsedTime)
{
    ui->txtPathList->setPlainText(list.join('\n'));
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
    }
}



void MainWindow::on_cmdListPathRecursively_clicked()
{
    ui->txtPathList->setPlainText("Scanning folder...");
    ui->txtStats->clear();
    QApplication::processEvents();

    QElapsedTimer ET;
    ET.start();

    const QStringList res = QtNoid::Common::File::listPathRecursively(ui->txtFolder->currentText());
    ui->txtPathList->setPlainText("Scanning folder... Done!\nCreating result text...");
    QApplication::processEvents();

    update_txtPathList(res, ET.nsecsElapsed());
}


void MainWindow::on_cmdListSubPathRecursively_clicked()
{
    ui->txtPathList->setPlainText("Generating sub folder list...");
    ui->txtStats->clear();
    QApplication::processEvents();

    QElapsedTimer ET;
    ET.start();

    const QStringList res = QtNoid::Common::File::listSubPathRecursively(ui->txtFolder->currentText());
    ui->txtPathList->setPlainText("Generating sub folder list... Done!\nCreating result text...");
    QApplication::processEvents();

    update_txtPathList(res, ET.nsecsElapsed());
}


void MainWindow::initFromAppConfig()
{
    // Geometry
    restoreGeometry(appConfig->restoreAsByteArray("Geometry", saveGeometry()));

    // Recent folder list
    auto list = appConfig->restoreAsStringList("RecentList", txtFolderList());
    list.append(qApp->applicationFilePath());
    list.removeDuplicates();
    ui->txtFolder->clear();
    ui->txtFolder->addItems(list);
}


