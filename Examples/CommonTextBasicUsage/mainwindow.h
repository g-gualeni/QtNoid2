#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QShortcut>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void on_cmdTokenizeSnakeCase_clicked();

    void on_cmdTokenizeCamelCase_clicked();

    void on_cmdTokenize_clicked();

    void on_cmdConvertToCamelCase_clicked();

    void on_cmdConvertToSnakeCase_clicked();

private:
    Ui::MainWindow *ui;
    QShortcut *m_screenshotShortcut = nullptr;

};
#endif // MAINWINDOW_H
