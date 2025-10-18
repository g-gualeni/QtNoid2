#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <functional>

QT_BEGIN_NAMESPACE
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
    bool generateTestDataFromResource(const QString &resPath);

    Ui::MainWindow *ui;
    void addRecentFileList(QAction *recentFilesParent, QStringList &recentFiles,
                           std::function<void(const QString&)> onFileSelected);

    void loadYamlFile(const QString &filePath);
};
#endif // MAINWINDOW_H
