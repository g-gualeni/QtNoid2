#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void on_cmdGONew_clicked();
    void on_cmdGOJson_clicked();
    void on_cmdGOJsonList_clicked();

private:
    quint64 benchmarkParameterUsingNewAndDelete(int iterations);
    quint64 benchmarkParameterUsingJSON(int iterations);
    quint64 benchmarkParameterListUsingJSON(int paramtersCount);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
