#include "presetsdialog.h"
#include "ui_presetdialog.h"

PresetsDialog::PresetsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

PresetsDialog::~PresetsDialog()
{
    delete ui;
}

void PresetsDialog::setPresetsText(const QString &text)
{
    ui->textEdit->setPlainText(text);
}

QString PresetsDialog::presetsText() const
{
    return ui->textEdit->toPlainText();
}

void PresetsDialog::on_cmdClear_clicked()
{
    ui->textEdit->clear();
}
