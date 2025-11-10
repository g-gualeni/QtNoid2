
#ifndef QTNOID_DEVELOPMENT_H
#define QTNOID_DEVELOPMENT_H

#include "global.h"

#include <QString>

class QWidget;
class QShortcut;
class QString;

namespace QtNoid {
namespace App {


class QTNOIDAPP_EXPORT Development
{
public:
    static QShortcut *initFullDialogGrabShortcut(QWidget *parent, const QString &keySequence = "Ctrl+Shift+S",
                                                 QString destinationFileOrPath = {}, bool saveToClipboard = true);

private:
};


} // namespace App
} // namespace QtNoid

#endif // QTNOID_DEVELOPMENT_H
