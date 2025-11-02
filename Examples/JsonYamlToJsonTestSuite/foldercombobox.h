#ifndef FOLDERCOMBOBOX_H
#define FOLDERCOMBOBOX_H

#include <QComboBox>
#include <QMouseEvent>

class FolderComboBox : public QComboBox
{
    Q_OBJECT

public:
    explicit FolderComboBox(QWidget *parent = nullptr);

protected:
    bool event(QEvent *event) override;

signals:
    void doubleClicked();

private:
    bool m_ignoreNextMouseRelease = false;
};

#endif // FOLDERCOMBOBOX_H
