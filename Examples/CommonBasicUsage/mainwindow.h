#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void on_cmdSelectFolder_clicked();
    void on_cmdListPathRecursively_clicked();
    void on_cmdListSubPathRecursively_clicked();

private:
    void initFromAppConfig();
    QStringList txtFolderList();
    void update_txtPathList(const QStringList &list, qint64 elapsedTime);

private:
    Ui::MainWindow *ui;
    QShortcut *m_screenshotShortcut = nullptr;
};
#endif // MAINWINDOW_H
