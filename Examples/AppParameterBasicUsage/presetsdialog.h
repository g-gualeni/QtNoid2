#ifndef PRESETSDIALOG_H
#define PRESETSDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class Dialog;
}
QT_END_NAMESPACE

class PresetsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PresetsDialog(QWidget *parent = nullptr);
    ~PresetsDialog();

    void setPresetsText(const QString &text);
    QString presetsText() const;

private slots:
    void on_cmdClear_clicked();

private:
    Ui::Dialog *ui;
};

#endif // PRESETSDIALOG_H
