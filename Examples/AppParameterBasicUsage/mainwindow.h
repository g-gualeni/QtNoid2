#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QtNoidApp/QtNoidApp"


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
    void on_cmdUpdate_clicked();
    void on_cmdToJson_clicked();
    void on_cmdFromJson_clicked();
    void on_cmdQDebug_clicked();
    void on_cmdUpdatePreset_clicked();
    void on_cboPresets_currentTextChanged(const QString &arg1);

private:
    void setRangeFromText(const QString& val);
    void setPresetsFromText(const QString& val);
    void updateStatusBar(const QString& msg);

private:
    Ui::MainWindow *ui;
    QShortcut *m_screenshotShortcut = nullptr;

    QtNoid::App::Parameter m_parameter;
    void updateFromGui();
    void updatePresetList();
};
#endif // MAINWINDOW_H
