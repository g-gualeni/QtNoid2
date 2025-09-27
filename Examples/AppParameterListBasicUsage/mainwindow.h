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


    void on_cmdDefault01_clicked();
    void on_cmdDefault02_clicked();

    void on_cmdSaveToJson_clicked();

    void on_cmdLoadFromJson_clicked();


private:
    void createList();
    void createUiFromList();
    QWidget *createUiTabFromList();
    QLayout *createUiParamterGroupFromParameter(QtNoid::App::Parameter* par, int width);
    QWidget *createUiDescriptionFromParameter(QtNoid::App::Parameter* par);
    void updateStyleSheet(QWidget *spinInt, QtNoid::App::Parameter *par);

private:
    Ui::MainWindow *ui;
    QtNoid::App::ParameterList m_list;

    QTabWidget* m_tabWidget;
    
};
#endif // MAINWINDOW_H
