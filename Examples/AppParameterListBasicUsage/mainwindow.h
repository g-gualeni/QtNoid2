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
    void on_cmdQDebug_clicked();


private:
    void createList();

private:
    Ui::MainWindow *ui;
    QtNoid::App::ParameterList m_list;

};
#endif // MAINWINDOW_H
