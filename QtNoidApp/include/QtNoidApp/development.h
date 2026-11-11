
#ifndef QTNOID_DEVELOPMENT_H
#define QTNOID_DEVELOPMENT_H

#include "global.h"

#include <QString>

class QWidget;
class QShortcut;
class QAction;
class QMenu;

class TestQtNoidAppDevelopment;   // forward declaration, in the global namespace


namespace QtNoid {
namespace App {


class QTNOID_APP_EXPORT Development
{
public:
    static QShortcut* initFullDialogGrabShortcut(QWidget* parent, const QString& keySequence = "Ctrl+Shift+S",
                                                 QString destinationFileOrPath = {}, bool saveToClipboard = true,
                                                 bool openDestinationPath = true);

    static QAction* createSaveConfigToProjectMenuAction(QMenu* developmentMenu, const QString& projectFolder);
    static QAction* createInitConfigFromResourcesMenuAction(QMenu* developmentMenu, const QString& resFileOrPrefix);

private:
    static QString screenshotFileName(QWidget* parent, const QString& destinationFileOrPath);
    friend class ::TestQtNoidAppDevelopment; // Only for White Box testing

};


} // namespace App
} // namespace QtNoid

#endif // QTNOID_DEVELOPMENT_H
