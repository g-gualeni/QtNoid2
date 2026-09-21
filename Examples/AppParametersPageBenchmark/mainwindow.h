#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE

class QShortcut;
class QPushButton;

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
    void on_cmdGOToJson_clicked();
    void on_cmdGOBinding_clicked();
    void on_cmdGOSignalAndSlots_clicked();

private:
    void setCmdGORunning(QPushButton *btn, bool running);
    quint64 benchmarkParameterUsingNewAndDelete(int iterations);
    std::pair<quint64, quint64> benchmarkParameterUsingJsonValueAndSchema(int iterations);
    quint64 benchmarkParametersPageUsingJSON(int paramtersCount);
    quint64 benchmarkParameterListToJSON(int paramtersCount);
    quint64 benchmarkBindings(int iterations);
    quint64 benchmarkSignalsAndSlot(int iterations);

private:
    Ui::MainWindow *ui;
    QShortcut *m_screenshotShortcut = nullptr;

};
#endif // MAINWINDOW_H
