#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QtNoidCommon/QtNoidCommon"
#include "QtNoidApp/QtNoidApp"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("QtNoid::Common::Text Basic Usage");
    // Geometry
    restoreGeometry(appConfig->restoreAsByteArray("Geometry", saveGeometry()));
    m_screenshotShortcut = QtNoid::App::Development::initFullDialogGrabShortcut(this);

    // convertToSnakeCase(const QString &text, int minNumBlockLen=2);


}

MainWindow::~MainWindow()
{
    appConfig->saveValue("Geometry", saveGeometry());
    delete ui;
}

void MainWindow::on_cmdTokenizeSnakeCase_clicked()
{
    auto txt = ui->txtInTokenizeSnakeCase->text();
    txt = QtNoid::Common::Text::tokenizeSnakeCase(txt).join(", ");
    ui->txtOutTokenizeSnakeCase->setText(txt);
}

void MainWindow::on_cmdTokenizeCamelCase_clicked()
{
    auto txt = ui->txtInTokenizeCamelCase->text();
    txt = QtNoid::Common::Text::tokenizeCamelCase(txt).join(", ");
    ui->txtOutTokenizeCamelCase->setText(txt);
}

void MainWindow::on_cmdTokenize_clicked()
{
    // tokenize(const QString &text, const QString &splittersString="_-. /\\", bool splitCamelCase=true, int minNumBlockLen=2);
    auto txt = ui->txtInTokenize->text();
    auto splittersString = ui->txtSplittersString->text();
    auto splitCamelCase = ui->optSplitCamelCase->checkState();
    auto minNumBlockLen = ui->txtMinNumBlockLen->value();

    txt = QtNoid::Common::Text::tokenize(txt, splittersString, splitCamelCase, minNumBlockLen).join(", ");
    ui->txtOutTokenize->setText(txt);
}


void MainWindow::on_cmdConvertToCamelCase_clicked()
{
    auto txt = ui->txtInConvertToCamelCase->text();
    txt = QtNoid::Common::Text::convertToCamelCase(txt);
    ui->txtOutConvertToCamelCase->setText(txt);
}


void MainWindow::on_cmdConvertToSnakeCase_clicked()
{
    auto minNumBlockLen = ui->txtMinNumBlockLen2->value();
    auto txt = ui->txtInConvertToSnakeCase->text();
    txt = QtNoid::Common::Text::convertToSnakeCase(txt, minNumBlockLen);
    ui->txtOutConvertToSnakeCase->setText(txt);
}

