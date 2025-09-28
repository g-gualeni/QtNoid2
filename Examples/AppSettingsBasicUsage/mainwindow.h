#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class QShortcut;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_cmdGroupName_clicked();
    void on_optUpdateWindowTitle_clicked(bool checked);
    void on_cmdFullDialogGrab_clicked();

private:
    Ui::MainWindow *ui;
    QShortcut *m_screenshotShortcut = nullptr;
};
#endif // MAINWINDOW_H
