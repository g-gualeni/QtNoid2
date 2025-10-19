#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "recentfilesmanager.h"

QT_BEGIN_NAMESPACE

class QShortcut;
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_cmdConvertToJson_clicked();
    void on_cmdConvertToYAML_clicked();
    void on_actionLoadYAML_triggered();
    void on_actionTestSuite_2022_01_17_triggered();
    void on_actionTestDataFolder_triggered();

private:
    Ui::MainWindow *ui;
    QShortcut* m_screenshotShortcut;
    recentFilesManager* m_recentFilesManager;
private:
    bool generateTestDataFromResource(const QString &resPath);

    bool updateUI_loadYamlFile(const QString &filePath);

    void updateUI_recentFiles(QString fileName);
};
#endif // MAINWINDOW_H
