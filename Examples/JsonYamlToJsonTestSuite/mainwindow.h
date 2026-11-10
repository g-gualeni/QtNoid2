#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNoidUiCommon/recentfiles.h>

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
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();




private slots:
    void on_cmdConvertToJson_clicked();
    void on_cmdConvertToYAML_clicked();
    void on_actionLoadYAML_triggered();
    void on_actionTestSuite_2022_01_17_triggered();
    void on_actionTxt2JsonTestSuite_QtNoid_2_2_0_triggered();
    void on_actionTestDataFolder_triggered();
    void on_actionManageProjectResources_triggered();
    void on_actionConvert_To_Json_triggered();
    void on_actionSave_Dataset_triggered();

    void on_actionOpen_JsonYamlToJsonTestSuite_md_triggered();

    void on_cmdBrowseSingleFile_clicked();

    void on_cmdSaveSingleFile_clicked();

private:
    Ui::MainWindow* ui;
    QShortcut* m_screenshotShortcut;
    QtNoid::UiCommon::RecentFiles m_recentFiles;

    QStringList m_yamlTestCollectionList;
    qsizetype m_yamlTestCollectionListCurrent = 0;

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
    void setCollectionFolderPath(const QString& path);
    QString collectionFolderPath() const;
    void setSingleFilePath(const QString& path);
    QString singleFilePath() const;
    QString yamlFullPathFromCollectionList();
    QString yamlStatusBarMsgFromCollectionList();
    void setCurrentSourceWidget(QWidget* widget);

private:
    bool isSourceChangeRequest(QObject *watched, QEvent *event) const;
    bool confirmDiscardYaml();
    void customizeUI();
    void customizeUI_initDevelopment();
    void tabWidgetSourceUiUpdate();
    void fileOpenSingleFile();

    void setYamlTestCollectionListCurrent(qsizetype newYamlTestCollectionListCurrent);

    //---- OLD to review
    bool generateTestDataFromResource(const QString& dataPrefix);
    void updateUI_loadYamlFile(const QString& yamlFilePath);
    void updateUI_loadExpectedFiles(const QString& yamlFilePath);

    void updateUI_convertYamlToJson();
    void updateUI_recentFiles(QString fileName);
    void updateUI_scanTestCollectionFolder(const QString& folder);
    void updateUI_progressBar();
    void updateUI_cmdPrevNext();
    void updateUI_txtCurrentFile();
    void updateUI_statusBar(const QString& msg = {});
    void updateUI_tabInputTitle();


    void onFolderComboBoxDoubleClicked();
    void onPreviousTest();
    void onNextTest();
    void onCmdSaveTestDataset();
    void onCreateNewTest();
    void initAppConfigFile();
    void restoreFromAppConfig();

};
#endif // MAINWINDOW_H
